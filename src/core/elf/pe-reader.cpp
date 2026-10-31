#include <arancini/elf/pe-reader.h>
#include <arancini/elf/windef.h>

#include <climits>
#include <cstdio>
#include <cstdlib>
#include <elf.h>
#include <fcntl.h>
#include <iostream>
#include <set>
#include <stdexcept>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <vector>
#include <cassert>
#include <fmt/core.h>
#include <bit>

//#define SWAP_32(value) (((value & 0x0000ffff) << 16) | ((value & 0xffff0000) >> 16))

using namespace arancini::pe;


pe_reader::pe_reader(const std::string &filename) : 
        bin_reader(filename),
        data_dirs_{},
        type_{pe_type::unk},
        coff_file_header_{},
        coff_strtbl_offset{0},
        coff_relocations_{std::nullopt},
        coff_symbols_{std::nullopt},
        attr_cert_tbl_{std::nullopt},
        export_dir_tbl_{std::nullopt},
        export_addr_tbl_{std::nullopt},
        export_names_ordinals_{std::nullopt},
        imported_dlls_{std::nullopt},
        pdata_exception_funcs_{std::nullopt},
        base_reloc_blocks_{std::nullopt}
{
    
}

void pe_reader::parse(){
    std::printf("start parsing\n");
    uint16_t msdos_magic = read16(0);
    if(msdos_magic != 0x5A4D){
        throw std::runtime_error(fmt::format("expected MSDOS 2.0 compatible header at the beginning, got {:#x}", msdos_magic));
    }

    uint32_t lfanew = read32(60); // address of the PE header after the MSDOS stub in the PE file
    uint32_t signature = read32(lfanew);

    if(signature != 0x4550) {
        //parse_coff_header(lfanew);
        throw std::runtime_error(fmt::format("expected PE format image file, got {:#x}", signature));  //not sure if this should be an error, should we also translate object files/DLL's?
    }   else {
        parse_coff_header(lfanew+4);
    }

    parse_section_data();
}

void pe_reader::parse_coff_header(uint32_t address) {
    std::printf("parsing coff header...\n");
    auto coff_header = read<IMAGE_FILE_HEADER>(address);

    uint16_t nr_sections = coff_header.NumberOfSections;
    uint16_t opt_header_size = coff_header.SizeOfOptionalHeader;
    uint16_t machine_magic = coff_header.Machine;
    uint16_t characteristics = coff_header.Characteristics;

    //should always be 0 in image files, dont think object files should be accepted
    //uint32_t coff_symtab_off = coff_header.PointerToSymbolTable;
    //uint32_t coff_symtab_nr_syms = coff_header.NumberOfSymbols;

    if(machine_magic != 0x8664) {
        throw std::runtime_error(fmt::format("Expected x64 source file, got {:#x}", machine_magic));
    }

    //check a couple of characteristic flags that should be set IMO
    //IMAGE_FILE_EXECUTABLE_IMAGE
    if(characteristics & 0x0002) {
        type_ = pe_type::exec;
    }
    //IMAGE_FILE_SYSTEM 
    if(characteristics & 0x1000) {
        type_ = pe_type::system;
    }
    //IMAGE_FILE_DLL 
    if(characteristics & 0x2000) {
        type_ = pe_type::dll;
    }
    if(type_ == pe_type::unk) {
        throw std::runtime_error("Did not recognize file type, is neither executbale (user or sys) nor is it DLL");
    }
    //IMAGE_FILE_LARGE_ADDRESS_AWARE should be set
    if(!(characteristics & 0x0020)) {
        throw std::runtime_error("Expected binary executable to support >2GB addr: IMAGE_FILE_LARGE_ADDRESS_AWARE, but flag is not set");
    }
    //IMAGE_FILE_32BIT_MACHINE should not be set
    if(characteristics & 0x0100) {
        throw std::runtime_error("Expected 64bit binary but IMAGE_FILE_32BIT_MACHINE is set, indicating 32b addresses");
    }
    // Rest of the flags are not relevant (only relevant for the loader, 
    // like whether it should copy the entire file into a swap file or on what kind of hardware it can run)

    //now points to one byte past the file header
    uint32_t opt_coff_header = address + sizeof(IMAGE_FILE_HEADER);

    //compute address of the coff symbol table (if it exists)
    coff_strtbl_offset = coff_header.PointerToSymbolTable + coff_header.NumberOfSymbols * sizeof(IMAGE_SYMBOL);

    if(opt_header_size > 0){
        parse_optional_header(opt_coff_header, opt_header_size);
    }

    //confusing name: now points to first byte of section headers
    opt_coff_header += opt_header_size;

    parse_section_headers(opt_coff_header, nr_sections);

    //the following things are not part of the header anymore but instead tables within the COFF table we need to read out
    //must be called after the section headers were parsed because we got a relocation table per section and the section-header holds the pointer to its relocation table
    parse_coff_relocations();   

    //iff the pointer to the sym tab is not 0 (should only be the case in object files) read it in as well
    if(coff_header.PointerToSymbolTable) {
        parse_coff_symtabs(coff_header.PointerToSymbolTable, coff_header.NumberOfSymbols);
    }

    //parse the attribute certificate table
    auto attr_cert_table_dir = get_data_dir(data_dirs::cert_table);
    if(attr_cert_table_dir.has_value()) {
        if(attr_cert_table_dir->VirtualAddress != 0) {
            parse_attribute_cert_table(attr_cert_table_dir->VirtualAddress, attr_cert_table_dir->Size);
        }
    }   else {
        //cannot use std::println because this uses std::format which expects constexpr format strings
#if __cplusplus >= 202603L
        std::println(std::dynamic_format(attr_cert_table_dir.error()));  //requires c++26
#else
        std::cout << "Error when accessing data directory: Cert Table Dir does not exist" << std::endl;
#endif
    }

    //parse delay load import tables
    /*auto delay_load_import_tbl = get_data_dir(data_dirs::delay_import_dsrcp);
    if(delay_load_import_tbl.has_value()) {
        parse_delay_load_import_table(delay_load_import_tbl->VirtualAddress, delay_load_import_tbl->Size);
    }   else {
        std::println(std::dynamic_format(delay_load_import_tbl.error()));  //requires c++26
        //std::cout << delay_load_import_tbl.error() << std::endl;;
    }*/
}

void pe_reader::parse_optional_header(uint32_t offset, size_t header_size){
    std::printf("parsing optional headers...\n");
    if(sizeof(IMAGE_OPTIONAL_HEADER64) > header_size) {
        throw std::runtime_error("Maybe the 32bit PE32 (instead of PE32+) header is there but there is not enough space");
    }

    auto opt_header = read<IMAGE_OPTIONAL_HEADER64>(offset);

    if(opt_header.Magic != 0x20b) {
        throw std::runtime_error(fmt::format("Image file has not correct state, expected 0x20B for PE32+ (64bit) executable, got {:#x}", opt_header.Magic));
    }

    // I dont think the version of the linker is too important, will see later

    entrypoint_ = opt_header.AddressOfEntryPoint;

    coff_file_header_.code_base = opt_header.BaseOfCode;
    coff_file_header_.code_size = opt_header.SizeOfCode;
    coff_file_header_.data_size = opt_header.SizeOfInitializedData;
    coff_file_header_.bss_size = opt_header.SizeOfUninitializedData;

    coff_file_header_.image_base = opt_header.ImageBase;
    coff_file_header_.sector_align = opt_header.SectionAlignment;
    coff_file_header_.file_align = opt_header.FileAlignment;

    //Skip a bunch of stuff, mostly windows specific version numbers but a few could be interesting:
    /*
    uint32_t loaded_image_size = opt_header.SizeOfImage;
    uint32_t headers_size = opt_header.SizeOfHeaders; //includes MSDOS, PE, Optional Header, Data dirs and section headers
    uint16_t subsystem_info = opt_header.Subsystem; //determines which windows subsystem has to run the file, should always be Win32 (dont care about stff like Xbox, or EFI stuff)
    uint16_t dll_info = opt_header.DllCharacteristics; // If image is a DLL this contains Loader information, e.g.. for relocationg, CFG
    */

    coff_file_header_.stack_reserved = opt_header.SizeOfStackReserve;
    coff_file_header_.stack_commited = opt_header.SizeOfStackCommit;
    coff_file_header_.heap_reserved = opt_header.SizeOfHeapReserve;
    coff_file_header_.heap_commited = opt_header.SizeOfHeapCommit;

    parse_data_dirs(offset + sizeof(IMAGE_OPTIONAL_HEADER64), opt_header.NumberOfRvaAndSizes);
}

void pe_reader::parse_data_dirs(uint32_t offset, uint32_t nr_data_dirs) {
    std::printf("parsing data dirs...\n");

    IMAGE_DATA_DIRECTORY entry{};
    for (size_t i = 0; i < nr_data_dirs; i++)
    {
        entry = read<IMAGE_DATA_DIRECTORY>(offset + i * sizeof(IMAGE_DATA_DIRECTORY));

        data_dirs_.push_back(entry);
    }
}

void pe_reader::parse_section_headers(uint32_t offset, size_t nr_sections){
    std::printf("parsing section headers...\n");

    IMAGE_SECTION_HEADER entry{};

    for(size_t i = 0; i < nr_sections; i++, offset += sizeof(IMAGE_SECTION_HEADER)) {
        entry = read<IMAGE_SECTION_HEADER>(offset);


        std::string name;
        name.assign(reinterpret_cast<char*>(entry.Name), 8);
        if(entry.Name[0] == '/' && type_ != pe_type::exec) {
            name.assign(reinterpret_cast<char*>(entry.Name) + 1, 7);
            int str_tbl_idx = std::stoi(name);
            name = readstr(coff_strtbl_offset + str_tbl_idx);
        }   else {
            name.assign(reinterpret_cast<char*>(entry.Name), 8);
        }

        section_headers_.push_back({name,
            entry.VirtualSize, 
            entry.VirtualAddress, 
            entry.SizeOfRawData, 
            entry.PointerToRawData, 
            entry.PointerToRelocations, 
            entry.PointerToLinenumbers, 
            entry.NumberOfRelocations, 
            entry.NumberOfLinenumbers, 
            entry.Characteristics});
    }
}

void pe_reader::parse_section_data() {
    std::printf("parsing section data...\n");

    //parse edata section
    auto edata_data_dir = get_data_dir(data_dirs::export_table);
    if(edata_data_dir.has_value()) {
        if(edata_data_dir->VirtualAddress != 0) {
            parse_edata_section(edata_data_dir->VirtualAddress, edata_data_dir->Size);
        }
    }   else {
#if __cplusplus >= 202603L
        std::println(std::dynamic_format(edata_data_dir.error()));  //requires c++26
#else
        std::cout << "Error when accessing data directory: Export Table Dir does not exist" << std::endl;
#endif
    }

    auto idata_data_dir = get_data_dir(data_dirs::import_table);
    if(idata_data_dir.has_value()) {
        if(idata_data_dir->VirtualAddress != 0) {
            parse_idata_section(idata_data_dir->VirtualAddress);
        }
    }   else {
#if __cplusplus >= 202603L
        std::println(std::dynamic_format(idata_data_dir.error()));  //requires c++26
#else
        std::cout << "Error when accessing data directory: Import Table Dir does not exist" << std::endl;
#endif
    }

    auto pdata_data_dir = get_data_dir(data_dirs::exception_table);
    if(pdata_data_dir.has_value()) {
        if(pdata_data_dir->VirtualAddress != 0) {
            parse_pdata_section(pdata_data_dir->VirtualAddress, pdata_data_dir->Size);
        }
    }   else {
#if __cplusplus >= 202603L
        std::println(std::dynamic_format(pdata_data_dir.error()));  //requires c++26
#else
        std::cout << "Error when accessing data directory: Exception (ptable) Table Dir does not exist" << std::endl;
#endif
    }

    auto reloc_data_dir = get_data_dir(data_dirs::base_reloc_table);
    if(reloc_data_dir.has_value()) {
        if(reloc_data_dir->VirtualAddress != 0) {
            parse_reloc_section(reloc_data_dir->VirtualAddress, reloc_data_dir->Size);
        }
    }   else {
#if __cplusplus >= 202603L
        std::println(std::dynamic_format(reloc_data_dir.error()));  //requires c++26
#else
        std::cout << "Error when accessing data directory: Base Reloc Table Dir does not exist" << std::endl;
#endif
    }

    //would rn only throw error; not implemented
    /*auto tls_data_dir = get_data_dir(data_dirs::tls_table);
    parse_tls_section(tls_data_dir.VirtualAddress);

    auto lcr_data_dir = get_data_dir(data_dirs::load_cfg_table);
    parse_load_config_dir(lcr_data_dir.VirtualAddress);

    auto rsrc_data_dir = get_data_dir(data_dirs::resource_table);
    parse_rsrc_section(rsrc_data_dir.VirtualAddress);*/
}

void pe_reader::parse_coff_relocations() {
    std::printf("parsing coff relocations...\n");

    //coff_relocations_ = decltype(coff_relocations_)::value_type{};
    //coff_relocations_ = std::make_shared<decltype(coff_relocations_)::element_type>();
    coff_relocations_.emplace();
    
    for(auto& sec_header : section_headers_) {
        uint32_t ptr_relocs = sec_header.get_reloc_ptr();
        uint16_t nr_relocs = sec_header.get_reloc_nr();
        coff_relocations_->push_back({});
        auto& section_relocs = coff_relocations_->back();

        if(ptr_relocs) {
            for(size_t i = 0; i < nr_relocs; i++) {
                IMAGE_RELOCATION reloc = read<IMAGE_RELOCATION>(ptr_relocs + i * sizeof(IMAGE_RELOCATION));
                section_relocs.push_back(reloc);
            }
        }
    }

    if(section_headers_.size() == 0){
        std::printf("Tried parsing coff relocations but there were no sections, maybe the functionwas called too early!\n");
    }
}

void pe_reader::parse_coff_symtabs(uint32_t offset, size_t nr_symbols){
    std::printf("parsing coff symbol table...\n");
    if(type_ == pe_type::exec) {
        std::printf("Found coff symtbl in an executable image. This should not be the case according to the docs. Skipping therefore for now...\n");
        return;
    }

    //coff_symbols_ = std::make_shared<decltype(coff_symbols_)::element_type>();
    coff_symbols_.emplace();
    for (size_t i = 0; i < nr_symbols; i++)
    {
        IMAGE_SYMBOL sym = read<IMAGE_SYMBOL>(offset);

        std::string name;
        if(!sym.N.LongName[0]) {
            //first 4 bytes are 0, so the name is actually in the string table
            name = readstr(coff_strtbl_offset + sym.N.LongName[1]);
        }   else {
            name = std::string{reinterpret_cast<char*>(sym.N.ShortName)};   //casting from uint8_t (unsigned char) to char
        }

        coff_symbols_->push_back({name, sym.Value, sym.SectionNumber, sym.Type, sym.StorageClass, sym.NumberOfAuxSymbols});
        auto& symm = coff_symbols_->back();

        for (size_t j = 0; j < sym.NumberOfAuxSymbols; j++)
        {
            using aux_sym = std::variant<struct aux_sym_func_defs, struct aux_sym_bf_ef, struct aux_sym_weak_ext, struct aux_sym_file, struct aux_sym_section_def, struct aux_sym_clr_token_def>;
            aux_sym sy;

            // determine the type of the aux symbol
            // +1 because we need to point past 1 the current symbol entry to get the first auxiliary entry for this symbol
            if(symm.is_func_def()) {
                sy = read<struct aux_sym_func_defs>(offset + (j+i+1) * sizeof(IMAGE_SYMBOL));
            }   else if (symm.is_bf_ef()) {
                sy = read<struct aux_sym_bf_ef>(offset + (j+i+1) * sizeof(IMAGE_SYMBOL));
            }   else if (symm.is_weak_ext()) {
                sy = read<struct aux_sym_weak_ext>(offset + (j+i+1) * sizeof(IMAGE_SYMBOL));
            }   else if (symm.is_file()) {
                sy = read<struct aux_sym_file>(offset + (j+i+1) * sizeof(IMAGE_SYMBOL));
            }   else if (symm.is_section_def()) {
                sy = read<struct aux_sym_section_def>(offset + (j+i+1) * sizeof(IMAGE_SYMBOL));
            }   else if (symm.is_clr_token_sym()){
                sy = read<struct aux_sym_clr_token_def>(offset + (j+i+1) * sizeof(IMAGE_SYMBOL));
            }   else {
                throw std::runtime_error(fmt::format("Unknown auxiliary coff symbol entry at index {} in the coff symbo, table", j+i+1));
            }

            symm.add_aux_sym(sy);
        }
        i += sym.NumberOfAuxSymbols;
        
    }
    
}

void pe_reader::parse_attribute_cert_table(uint32_t offset, uint32_t size) {
    std::printf("parsing attribute certificate table...\n");

    //attr_cert_tbl_ = std::make_shared<decltype(attr_cert_tbl_)::element_type>();
    attr_cert_tbl_.emplace();
    uint32_t curr_elem = offset;
    while(curr_elem < offset + size) {
        struct attr_cert_tbl read_entry = read<struct attr_cert_tbl>(offset);
        //correct quadword (= 8byte) alignment
        uint32_t rounded_size = (read_entry.dwLength + 7) &~ uint32_t(7);

        attr_cert_tbl_->push_back({read_entry.dwLength, read_entry.wRevision, read_entry.wCertificateType});
        struct attr_cert_tbl& cert_data = attr_cert_tbl_->back();

        //Copy the cert data from the file in the struct, size needs to be reduced by 8 because it includes the first 8 bytes of the cert table
        char* first_cert_ptr = (char*) get_data_ptr(offset + 8);    //First 8 bytes of the struct are metadata
        std::copy(first_cert_ptr, first_cert_ptr + rounded_size - 8, std::back_inserter(cert_data.bCertificate));

        curr_elem += rounded_size;
    }
}

void pe_reader::parse_delay_load_import_table(uint32_t offset, uint32_t size) {
    uint32_t file_offset = get_file_offset(offset);

    while(true) {
        IMAGE_DELAYLOAD_DESCRIPTOR delayload_dir_entry = read<IMAGE_DELAYLOAD_DESCRIPTOR>(file_offset);

        if(delayload_dir_entry.AllAttributes == 0 &&
            delayload_dir_entry.DllNameRVA == 0 &&
            delayload_dir_entry.ModuleHandleRVA == 0 &&
            delayload_dir_entry.ImportAddressTableRVA == 0 &&
            delayload_dir_entry.ImportNameTableRVA == 0 &&
            delayload_dir_entry.BoundImportAddressTableRVA == 0 &&
            delayload_dir_entry.UnloadInformationTableRVA == 0 &&
            delayload_dir_entry.TimeDateStamp == 0)
        {
            break;
        }
    }
    return;
}

uint32_t pe_reader::get_offset_by_section_name(const std::string& section_name, uint32_t rva) {
    //get section header of the section where the rva points to
    for(const auto& sec_header : section_headers_) {
        if (sec_header.get_name() == section_name)
        {
            return rva - sec_header.get_virt_addr() + sec_header.get_raw_ptr();
        }
        
    }
    throw std::runtime_error(fmt::format("Did not find section with name {} ...", section_name));
}

//does not take a name but instead goes through all sections to determine in which the rva points
//reason, msvc for example stuffs the import tables in the rdata section (and does not create a idata section)
uint32_t pe_reader::get_file_offset(uint32_t rva) {
    for (const auto& sec_header : section_headers_) {
        if(sec_header.get_virt_addr() <= rva && rva < (sec_header.get_virt_addr() + sec_header.get_virt_size())){
            return rva - sec_header.get_virt_addr() + sec_header.get_raw_ptr();
        }
    }
    throw std::runtime_error(fmt::format("Did not find the section where the rva {} should belong to...", rva));
}

//export data section, contains syms that other images can access through dyn linking
void pe_reader::parse_edata_section(uint32_t rva, uint32_t section_size) {
    std::printf("parsing edata section...\n");

    //export_dir_tbl_ = std::make_shared<decltype(export_dir_tbl_)::element_type>();
    //export_addr_tbl_ = std::make_shared<decltype(export_addr_tbl_)::element_type>();
    //export_names_ordinals_ = std::make_shared<decltype(export_names_ordinals_)::element_type>();
    export_dir_tbl_.emplace();
    export_addr_tbl_.emplace();
    export_names_ordinals_.emplace();
    std::printf("parsing edata section2...\n");
    //uint32_t file_offset = get_offset_by_section_name(".edata", rva);
    uint32_t file_offset = get_file_offset(rva);

    IMAGE_EXPORT_DIRECTORY export_dir_tbl = read<IMAGE_EXPORT_DIRECTORY>(file_offset);

    std::printf("parsing edata section3...\n");
    
    export_dir_tbl_->name_ptr_rva = export_dir_tbl.AddressOfNames;
    export_dir_tbl_->export_addr_tbl_rva = export_dir_tbl.AddressOfFunctions;
    export_dir_tbl_->ordinal_tbl_rva = export_dir_tbl.AddressOfNameOrdinals;
    export_dir_tbl_->ordinal_base = export_dir_tbl.Base;
    export_dir_tbl_->address_tbl_entries_size = export_dir_tbl.NumberOfFunctions;
    export_dir_tbl_->name_ptr_size = export_dir_tbl.NumberOfNames;

    //uint32_t dll_name_ptr = get_offset_by_section_name(".edata", export_dir_tbl.Name);
    uint32_t dll_name_ptr = get_file_offset(export_dir_tbl.Name);
    export_dir_tbl_->name = readstr(dll_name_ptr);

    std::printf("parsed export dir table...\n");

    //Table that holds the address of exported functions, gets accessed by using ordinal numbers as indices
    //uint32_t export_address_table_ptr = get_offset_by_section_name(".edata", export_dir_tbl.AddressOfFunctions);
    uint32_t export_address_table_ptr = get_file_offset(export_dir_tbl.AddressOfFunctions);
    for (uint32_t i = 0; i < export_dir_tbl.NumberOfFunctions; i++){
        uint32_t export_addr = read32(export_address_table_ptr + i * sizeof(uint32_t));
        //if rva is not within the current section, it points to an actual function
        if(rva > export_addr || export_addr > (rva + section_size)) {
            export_addr_tbl_->push_back(export_addr);
        }   
        else {
            //in this case its a forwarder rva which points to a string in the .edata section
            //uint32_t name_ptr = get_offset_by_section_name(".edata", export_addr);
            uint32_t name_ptr = get_file_offset(export_addr);
            export_addr_tbl_->push_back(readstr(name_ptr));
        }
    }

    //ordinal and name table are parallel arrays where the i-th entrsy in the name ptr table holds the rva to the name of the exported symbol
    //and the i-th entry in the ordinal table the same symbols unbiased ordinal number
    //store both together in a vector

    //we only have the RVAs of these two tables so first we need to get the actual file offsets
    //uint32_t export_name_ptr_tbl_ptr = get_offset_by_section_name(".edata", export_dir_tbl.AddressOfNames);
    //uint32_t ordinal_tbl_ptr = get_offset_by_section_name(".edata", export_dir_tbl.AddressOfNameOrdinals);
    uint32_t export_name_ptr_tbl_ptr = get_file_offset(export_dir_tbl.AddressOfNames);
    uint32_t ordinal_tbl_ptr = get_file_offset(export_dir_tbl.AddressOfNameOrdinals);
    for (size_t i = 0; i < export_dir_tbl.NumberOfNames; i++)
    {
        //go through the entries in both tables, in the case of the name ptr table, resolve the addr to the file offset of the string and store it
        uint32_t name_ptr_rva = read32(export_name_ptr_tbl_ptr + i * sizeof(uint32_t));
        //uint32_t name_ptr = get_offset_by_section_name(".edata", name_ptr_rva);
        uint32_t name_ptr = get_file_offset(name_ptr_rva);
        std::string export_name = readstr(name_ptr);

        uint16_t export_ordinal = read16(ordinal_tbl_ptr + i * sizeof(uint16_t));
        export_names_ordinals_->push_back({export_ordinal, export_name});
    }
    std::printf("finished parsing edata section...\n");
}

void pe_reader::parse_idata_section(uint32_t rva) {
    std::printf("parsing idata section...\n");

    //imported_dlls_ = std::make_shared<decltype(imported_dlls_)::element_type>();
    imported_dlls_.emplace();
    //uint32_t file_offset = get_offset_by_section_name(".idata", rva);
    uint32_t file_offset = get_file_offset(rva);

    while(true) {
        IMAGE_IMPORT_DESCRIPTOR import_dir_entry = read<IMAGE_IMPORT_DESCRIPTOR>(file_offset);

        if(import_dir_entry.FirstThunk == 0
            && import_dir_entry.TimeDateStamp == 0
            && import_dir_entry.ForwarderChain == 0
            && import_dir_entry.Name == 0
            && import_dir_entry.FirstThunk == 0) 
        {
            break;
        }

        //uint32_t dll_name_ptr = get_offset_by_section_name(".idata", import_dir_entry.Name);
        uint32_t dll_name_ptr = get_file_offset(import_dir_entry.Name);
        std::string dll_name = readstr(dll_name_ptr);

        imported_dlls_->push_back({dll_name, import_dir_entry.ForwarderChain});
        auto& current_dll = imported_dlls_->back();

        //uint32_t import_lookup_tbl_ptr = get_offset_by_section_name(".idata", import_dir_entry.OriginalFirstThunk);
        uint32_t import_lookup_tbl_ptr = get_file_offset(import_dir_entry.OriginalFirstThunk);
        uint32_t import_address_tbl_rva = import_dir_entry.FirstThunk;
        while(true) {
            uint64_t import_lookup_tbl = read64(import_lookup_tbl_ptr);
            //last entry is 0 (indicates end of table)
            if(import_lookup_tbl == 0) {
                break;
            }

            if(import_lookup_tbl >> 63) {
                //import by ordinal
                uint16_t ordinal = import_lookup_tbl & 0xffff;
                current_dll.imported_symbols.push_back(std::make_pair(import_address_tbl_rva, ordinal));
            }   else {
                uint32_t name_ptr_rva = import_lookup_tbl & 0xffffffff;
                //uint32_t name_ptr = get_offset_by_section_name(".idata", name_ptr_rva);
                uint32_t name_ptr = get_file_offset(name_ptr_rva);
                //import by name
                uint16_t hint = read16(name_ptr);
                std::string import_name = readstr(name_ptr + sizeof(uint16_t));
                current_dll.imported_symbols.push_back(std::make_pair(import_address_tbl_rva, import_name));
            }

            import_address_tbl_rva += sizeof(uint64_t);
            import_lookup_tbl_ptr += sizeof(uint64_t);
        }

        file_offset += sizeof(IMAGE_IMPORT_DESCRIPTOR);
    }
}

void pe_reader::parse_pdata_section(uint32_t rva, uint32_t size) {
    std::printf("parsing pdata section...\n");

    //pdata_exception_funcs_ = std::make_shared<decltype(pdata_exception_funcs_)::element_type>();
    pdata_exception_funcs_.emplace();
    //uint32_t file_offset = get_offset_by_section_name(".pdata", rva);
    uint32_t file_offset = get_file_offset(rva);

    uint32_t curr_elem = file_offset;
    while(curr_elem < rva + size) {
        struct x86_func_tbl_entry entry = read<struct x86_func_tbl_entry>(curr_elem);
        pdata_exception_funcs_->push_back(entry);

        curr_elem += sizeof(struct x86_func_tbl_entry);
    }
}

void pe_reader::parse_reloc_section(uint32_t rva, uint32_t size) {
    std::printf("parsing reloc section...\n");

    //base_reloc_blocks_ = std::make_shared<decltype(base_reloc_blocks_)::element_type>();
    base_reloc_blocks_.emplace();
    //uint32_t file_offset = get_offset_by_section_name(".reloc", rva);
    uint32_t file_offset = get_file_offset(rva);

    uint32_t curr_block = file_offset;
    while(curr_block < file_offset + size) {
        uint32_t page_rva = read32(curr_block);
        uint32_t block_size = read32(curr_block + 4) - 8;

        //std::println("got page rva {:#x} and block size {:#x} at address {:#x} ...", page_rva, block_size + 8, curr_block);

        curr_block += 8;

        base_reloc_blocks_->push_back({page_rva});
        auto& latest_reloc_block = base_reloc_blocks_->back();

        uint32_t r = curr_block;

        while(r < curr_block + block_size) {
            uint16_t type_offset_field = read16(r);
            latest_reloc_block.offsets.push_back(type_offset_field);

            r += sizeof(uint16_t);
        }
        curr_block += block_size;
        //Correct alignment???
        curr_block = (curr_block + 3) &~ uint32_t(3);
    }
    std::printf("finished parsing reloc section...\n");
}

void pe_reader::parse_tls_section(uint32_t rva) {
    throw std::logic_error("TLS Not yet implemented!");
}

void pe_reader::parse_load_config_dir(uint32_t rva) {
    //not sure how to get the offset in the file because I dont know the section where the table is located
    throw std::logic_error("Load config dir not yet implemented!");
}

void pe_reader::parse_rsrc_section(uint32_t rva) {
    //uint32_t file_offset = get_offset_by_section_name(".rsrc", rva);
    uint32_t file_offset = get_file_offset(rva);

    throw std::logic_error("Parsing of rsrc not yet implemented!");
}