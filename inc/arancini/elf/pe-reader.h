#pragma once

#include <string>
#include <memory>
#include <vector>
#include <stdexcept>
#include <fstream>
#include <cstdint>
#include <array>
#include <variant>
#include <expected>
#include <fmt/core.h>
#include <format>
#include <optional>

#include "windef.h"
#include "reader.h"

namespace arancini::pe
{

enum class pe_type : uint8_t { unk = 0, exec, dll, system };

enum class data_dirs : uint32_t {
    export_table = 0, 
    import_table,
    resource_table,
    exception_table,
    cert_table,
    base_reloc_table,
    debug,
    arch,   //should always be zero
    global_ptr, //size 0, points to the value to be pointed in the global pointer register, points to a images static data area (usually not used???)
    tls_table,
    load_cfg_table,
    bound_import,
    import_address_table,
    delay_import_dsrcp,
    clr_rt_header,
    reserved,
};

#pragma pack(1)
struct aux_sym_func_defs {
    uint32_t tag_index;
    uint32_t total_size;
    uint32_t ptr_line_number;
    uint32_t ptr_next_fn;
    char padding[2];
};

#pragma pack(1)
struct aux_sym_bf_ef {
    char padding1[4];
    uint16_t linenumber;
    char padding2[6];
    uint32_t ptr_next_fn;   //only used for bf (begin function) symbols
    char padding3[2];
};

#pragma pack(1)
struct aux_sym_weak_ext {
    uint32_t tag_idx;
    uint32_t characteristics;
    char padding[10];
};

#pragma pack(1)
struct aux_sym_file {
    char name[18];
};

#pragma pack(1)
struct aux_sym_section_def {
    uint32_t length;
    uint16_t nr_relocs;
    uint16_t nr_line_numbers;
    uint32_t checksum;
    uint16_t number;
    uint8_t selection;
    char padding[3];
};

#pragma pack(1)
struct aux_sym_clr_token_def {
    uint8_t bAuxType;
    uint8_t bReserved;
    uint32_t sym_table_idx;
    char padding[12];
};

using aux_sym = std::variant<struct aux_sym_func_defs, struct aux_sym_bf_ef, struct aux_sym_weak_ext, struct aux_sym_file, struct aux_sym_section_def, struct aux_sym_clr_token_def>;

class symbol {
    public:
    symbol(const std::string& name, 
        uint32_t val, 
        int16_t section_number, 
        uint16_t type, 
        uint8_t storage_class,
        uint8_t nr_aux_syms) 
        : name_{name}, val_(val), section_number_{section_number}, type_{type}, storage_class_{storage_class}, auxiliary_symbols_{} 
    {
        auxiliary_symbols_.reserve(nr_aux_syms);
    }

    void add_aux_sym(const aux_sym& sym) {
        auxiliary_symbols_.push_back(sym);
    }

    bool is_func_def() {
        //considered func definitio if section number is non zero, storage class is external and the MSB of type is set to 0x20
        return (section_number_ > 0) && (storage_class_ == 2) && ((type_ & 0xff00) >> 8 == 2);
    }

    bool is_bf_ef() {
        //not sure, after .lf there should not be an aux entry so this should be fine without testing the names
        return (storage_class_ == 101);// && (name_ == ".bf" || name == ".ef");
    }

    bool is_weak_ext() {
        return (storage_class_ == 2) && (val_ == 0) && (section_number_ == 0);
    }

    bool is_file() {
        return (storage_class_ == 103) && (name_ == ".file");
    }

    bool is_section_def() {
        return (storage_class_ == 3);
    }

    bool is_clr_token_sym() {
        return (storage_class_ == 107);
    }

    std::string get_name() const {
        return name_;
    }

    uint32_t get_value() const {
        return val_;
    }

    int16_t get_section_idx() const {
        return section_number_;
    }

    uint16_t get_type() const {
        return type_;
    }

    uint8_t get_storage_class() const {
        return storage_class_;
    }

    std::vector<aux_sym> get_aux_symbols() const {
        return auxiliary_symbols_;
    }

    private:
    std::string name_;
    uint32_t val_;
    int16_t section_number_;
    uint16_t type_;
    uint8_t storage_class_;

    std::vector<aux_sym> auxiliary_symbols_;
};

class section_header {
    public:
    section_header(const std::string& name,
        uint32_t VirtualSize,
        uint32_t VirtualAddress,
        uint32_t SizeOfRawData,
        uint32_t PointerToRawData,
        uint32_t PointerToRelocations,
        uint32_t PointerToLinenumbers,
        uint16_t NumberOfRelocations,
        uint16_t NumberOfLinenumbers,
        uint32_t Characteristics
    ) 
    : name_(name), VirtualSize_(VirtualSize), VirtualAddress_(VirtualAddress), SizeOfRawData_(SizeOfRawData), PointerToRawData_(PointerToRawData), PointerToRelocations_(PointerToRelocations), PointerToLinenumbers_(PointerToLinenumbers), NumberOfRelocations_(NumberOfRelocations), NumberOfLinenumbers_(NumberOfLinenumbers), Characteristics_(Characteristics) {}
    
    std::string get_name() const {
        return name_;
    }

    uint32_t get_virt_size() const {
        return VirtualSize_;
    }

    uint32_t get_virt_addr() const {
        return VirtualAddress_;
    }

    uint32_t get_raw_size() const {
        return SizeOfRawData_;
    }

    uint32_t get_raw_ptr() const {
        return PointerToRawData_;
    }

    uint32_t get_reloc_ptr() const {
        return PointerToRelocations_;
    }

    uint32_t get_linenumber_ptr() const {
        return PointerToLinenumbers_;
    }

    uint16_t get_reloc_nr() const {
        return NumberOfRelocations_;
    }

    uint16_t get_linenumber_nr() const {
        return NumberOfLinenumbers_;
    }

    uint32_t get_characteristics() const {
        return Characteristics_;
    }

    private:
    std::string name_;
    uint32_t VirtualSize_;
    uint32_t VirtualAddress_;
    uint32_t SizeOfRawData_;
    uint32_t PointerToRawData_;
    uint32_t PointerToRelocations_;
    uint32_t PointerToLinenumbers_;
    uint16_t NumberOfRelocations_;
    uint16_t NumberOfLinenumbers_;
    uint32_t Characteristics_;
};

struct attr_cert_tbl {
    uint32_t dwLength;
    uint16_t wRevision;
    uint16_t wCertificateType;
    std::vector<char> bCertificate{};
};

struct export_dir_tbl {
    std::string name;
    uint32_t ordinal_base;
    uint32_t address_tbl_entries_size;
    uint32_t name_ptr_size;
    uint32_t export_addr_tbl_rva;
    uint32_t name_ptr_rva;
    uint32_t ordinal_tbl_rva;
};

struct export_names_ordinals {
    uint16_t export_ordinal_unbiased;
    std::string export_name;
};

struct imported_dll {
    std::string name;
    uint32_t forwarder_chain;   //no idea what this does
    std::vector<std::variant<uint16_t, std::string>> imported_symbols{};
};

struct x86_func_tbl_entry {
    uint32_t begin_addr_rva;
    uint32_t end_addr_rva;
    uint32_t unwind_info_rva;
};

struct base_reloc_block {
    uint32_t page_rva;      //image base plus this page rva gets added to the offsets in the reloc block to create VA where the reloc is to be applied
    std::vector<uint16_t> offsets{};
};

struct coff_file_header {
    off_t code_base;    //offset in loaded memory where the code section starts (.text section), fo through that with xed 
    size_t code_size;   //size of the code section (sum of all code sections, no idea if they are contigouos)
    size_t data_size;   //size of initialized data section
    size_t bss_size;    //size of uninitialized data section

    uint64_t image_base;    //preferred address where the DLL/image is loaded into memory (might be relevant for relocations)
    uint32_t sector_align;  //alignment between sections in memory
    uint32_t file_align;    //the same but for the file

    uint64_t stack_reserved;    //reserved in virtual address space of program
    uint64_t stack_commited;    //actual space allocated by the OS (i.e. #of physical pages)
    uint64_t heap_reserved;
    uint64_t heap_commited;
};

struct delay_load_dir_table {
    std::string dll_name;
    uint32_t attr;
};

class pe_reader : public bin_reader {
    public:
        pe_reader(const std::string &filename);
        ~pe_reader() {};

        void parse();

        auto get_data_dir(data_dirs dir_type) const -> std::optional<IMAGE_DATA_DIRECTORY> {
            if(data_dirs_.size() > static_cast<uint32_t>(dir_type)) {
                return data_dirs_.at(static_cast<u_int32_t>(dir_type));
            }
            uint32_t idx = static_cast<uint32_t>(dir_type);
            return std::nullopt;
            //return std::unexpected(std::vformat("Tried to access data dir {} but the data dir was not in the vector", std::make_format_args(idx)));
            //return std::unexpected("Hi");
        }

        pe_type type() const { return type_; }

        const std::vector<IMAGE_DATA_DIRECTORY>& get_data_dirs() const {
            return data_dirs_;
        }

        const std::vector<section_header>& get_section_headers() const {
            return section_headers_;
        }

        //maybe use shared_ptr so that we can return quasi references instead of a entire copy??? no dangling references
        const section_header& get_section_header(int index) const {
            return section_headers_[index];
        }

        const struct coff_file_header& get_coff_file_header() const {
            return coff_file_header_;
        }

        auto get_coff_relocations() const -> const std::optional<std::vector<std::vector<IMAGE_RELOCATION>>>& {
            return coff_relocations_;
        }

        auto get_coff_symtable() const -> const std::optional<std::vector<symbol>>& {
            return coff_symbols_;
        }

        auto get_cert_table() const -> const std::optional<std::vector<struct attr_cert_tbl>>& {
            return attr_cert_tbl_;
        }

        auto get_export_dir_tbl() const -> const std::optional<struct export_dir_tbl>& {
            return export_dir_tbl_;
        }

        auto get_export_addr_tbl() const -> const std::optional<std::vector<std::variant<std::string, uint32_t>>>& {
            return export_addr_tbl_;
        }

        auto get_export_names_ordinals() const -> const std::optional<std::vector<struct export_names_ordinals>>& {
            return export_names_ordinals_;
        }

        auto get_import_directories() const -> const std::optional<std::vector<struct imported_dll>>& {
            return imported_dlls_;
        }

        auto get_procedure_data_funcs() const -> const std::optional<std::vector<struct x86_func_tbl_entry>>& {
            return pdata_exception_funcs_;
        }

        auto get_reloc_blocks() const -> const std::optional<std::vector<struct base_reloc_block>>& {
            return base_reloc_blocks_;
        }        

    private:
        pe_type type_;

        std::vector<IMAGE_DATA_DIRECTORY> data_dirs_;
        std::vector<section_header> section_headers_;

        //stuff that i think could be useful later on
        struct coff_file_header coff_file_header_;

        //needed because in non executables the section headers have offsets into the string table
        uint32_t coff_strtbl_offset;

        void parse_coff_header(uint32_t address);

        void parse_optional_header(uint32_t offset, size_t header_size);

        void parse_data_dirs(uint32_t offset, uint32_t nr_data_dirs);

        void parse_section_headers(uint32_t offset, size_t nr_sections);

        void parse_section_data();

        //Object file only
        void parse_coff_relocations();
        std::optional<std::vector<std::vector<IMAGE_RELOCATION>>> coff_relocations_;

        void parse_coff_symtabs(uint32_t offset, size_t nr_symbols);
        std::optional<std::vector<symbol>> coff_symbols_;

        void parse_attribute_cert_table(uint32_t offset, uint32_t size);
        std::optional<std::vector<struct attr_cert_tbl>> attr_cert_tbl_;

        void parse_delay_load_import_table(uint32_t offset, uint32_t size);

        //parse section data

        //export table, the symbols being exported (also indirectly) by the image file
        void parse_edata_section(uint32_t rva, uint32_t section_size);
        std::optional<struct export_dir_tbl> export_dir_tbl_;
        std::optional<std::vector<std::variant<std::string, uint32_t>>> export_addr_tbl_;
        std::optional<std::vector<struct export_names_ordinals>> export_names_ordinals_;

        //import table, all symbols from all dlls that need to be imported
        void parse_idata_section(uint32_t rva);
        std::optional<std::vector<struct imported_dll>> imported_dlls_;

        //pdata = procedure data; contains array of function table entries that are used for exception handling
        void parse_pdata_section(uint32_t rva, uint32_t size);
        std::optional<std::vector<struct x86_func_tbl_entry>> pdata_exception_funcs_;

        void parse_reloc_section(uint32_t rva, uint32_t size);
        std::optional<std::vector<struct base_reloc_block>> base_reloc_blocks_;

        //Not yet implemented, maybe later
        void parse_tls_section(uint32_t rva);

        void parse_load_config_dir(uint32_t rva);

        void parse_rsrc_section(uint32_t rva);

        //.cormeta has no defined layout and is object only
        //.sxdata only holds exception handlers, sounds pretty uninteresting to me

        uint32_t get_offset_by_section_name(const std::string& section_name, uint32_t rva);

        uint32_t get_file_offset(uint32_t rva);
};

} // namespace arancini::pe


/*
//to deal with std::variant
template<class... Ts> struct overloaded : Ts... { using Ts::operator()...; };

template<>
struct std::formatter<arancini::pe::pe_type> {
    constexpr auto parse(std::format_parse_context& ctx) {
        return ctx.begin();
    }

    auto format(const arancini::pe::pe_type& obj, std::format_context& ctx) const {
        switch (obj)
        {
        case arancini::pe::pe_type::unk:
            return format_to(ctx.out(), "unknown");
            break;
        case arancini::pe::pe_type::exec:
            return format_to(ctx.out(), "executable");
            break;
        case arancini::pe::pe_type::dll:
            return format_to(ctx.out(), "DLL");
            break;
        case arancini::pe::pe_type::system:
            return format_to(ctx.out(), "system file");
        }
    }
};

template<>
struct std::formatter<arancini::pe::pe_reader> : std::formatter<string_view> {
    //for now nothing w.r.t to the format, might change
    constexpr auto parse(std::format_parse_context& ctx) {
        return ctx.begin();
    }

    auto format(const arancini::pe::pe_reader& obj, std::format_context& ctx) const {
        std::string temp = "Read PE File:\n";

        std::format_to(std::back_inserter(temp), "File type: {}\n", obj.type());
        std::format_to(std::back_inserter(temp), "entrypoint: {:#x}\n", obj.get_entrypoint());

        const auto& coff_header = obj.get_coff_file_header();
        std::format_to(std::back_inserter(temp), "Code base (rva, offset from load address): {:#x}\nCode size {:#x}\n", coff_header.code_base, coff_header.code_size);
        std::format_to(std::back_inserter(temp), "Size of initialized data section (.data) {:#x}\nSize of uninitialized data section (.bss): {:#x}\n", coff_header.data_size, coff_header.bss_size);
        std::format_to(std::back_inserter(temp), "Image Base (preferred load addr): {:#x}\nSection alignments in mem: {:#x}\nSection alignments on the file: {:#x}\n", coff_header.image_base, coff_header.sector_align, coff_header.file_align);
        std::format_to(std::back_inserter(temp), "Amount of bytes reserved in virtual addr space for stack: {:#x}\nAmount of physical bytes reserved for stack: {:#x}\n", coff_header.stack_reserved, coff_header.stack_commited);
        std::format_to(std::back_inserter(temp), "Amount of bytes reserved in virtual addr space for heap: {:#x}\nAmount of physical bytes reserved for heap: {:#x}\n", coff_header.heap_reserved, coff_header.heap_commited);
        
        std::format_to(std::back_inserter(temp), "Data directories:\n");
        
        auto& data_dirs = obj.get_data_dirs();
        for (const auto& dir : data_dirs) {
            std::format_to(std::back_inserter(temp), "RVA: {:#x}\nSize: {:#x}\n", dir.VirtualAddress, dir.Size);
        }

        std::format_to(std::back_inserter(temp), "Sections:\n");

        auto& section_headers = obj.get_section_headers();
        auto section_relocations = obj.get_coff_relocations();
        for(size_t i = 0; i < section_headers.size(); i++) {
            auto header = section_headers[i];
            auto relocs = section_relocations->at(i);

            std::format_to(std::back_inserter(temp), "Section name: {}\n", header.get_name());
            std::format_to(std::back_inserter(temp), "Virtual (RVA) address: {:#x} and Virtual size in memory: {:#x}\n", header.get_virt_addr(), header.get_virt_size());
            std::format_to(std::back_inserter(temp), "Raw address: {:#x} and raw size: {:#x}\n", header.get_raw_ptr(), header.get_raw_size());
            std::format_to(std::back_inserter(temp), "Coff relocations for this section: {:#x} and their amount: {:#x}\n", header.get_reloc_ptr(), header.get_reloc_nr());
            std::format_to(std::back_inserter(temp), "Characteristics: {:#x}\nRelocations for this section:\n", header.get_characteristics());
            for(const auto& reloc : relocs) {
                std::format_to(std::back_inserter(temp), "RVA of where the reloc is to be applied: {:#x}, Symbol index: {:#x}, Type: {:#x}\n", reloc.VirtualAddress, reloc.SymbolTableIndex, reloc.Type);
            }
        }

        std::format_to(std::back_inserter(temp), "Coff symbol table:\n");

        auto coff_symbols = obj.get_coff_symtable();
        if(coff_symbols) {
            for (auto symbol : *coff_symbols) {
                std::format_to(std::back_inserter(temp), "Symbol name: {}\n", symbol.get_name());
                std::format_to(std::back_inserter(temp), "Value: {:#x}, Section Index: {:#x}, Type: {:#x}, Storage class: {:#x} and following aux symbols:\n", symbol.get_value(), symbol.get_section_idx(), symbol.get_type(), symbol.get_storage_class());

                const auto aux_symbols = symbol.get_aux_symbols();
                for (auto aux_sym : aux_symbols)
                {
                    auto type = overloaded{
                        [&temp](const struct arancini::pe::aux_sym_func_defs& a) { std::format_to(std::back_inserter(temp), "Func def aux symbol: symtbl idx of corresponding .bf symbol: {:#x}\nSize of function {:#x}\nIdx of next func-def sym: {:#x}\n", a.tag_index, a.total_size, a.ptr_next_fn); },
                        [&temp](const struct arancini::pe::aux_sym_bf_ef& a) { std::format_to(std::back_inserter(temp), "BF/EF symbol, ordinal line number in src file: {:#x}\nSymbol table idx to next function (only for .bf, .ef has 0 here): {:#x}\n", a.linenumber, a.ptr_next_fn); },
                        [&temp](const struct arancini::pe::aux_sym_weak_ext& a) { std::format_to(std::back_inserter(temp), "Weak external symbol, Symtbl idx of the replacement symbol: {:#x}\nCharacteristics (look up semantics in docs): {:#x}\n", a.tag_idx, a.characteristics); },
                        [&temp](const struct arancini::pe::aux_sym_section_def& a) { std::format_to(std::back_inserter(temp), "Section def. symbol, Size of the section data: {:#x}\nNr of relocs: {:#x}\nNr of line-numbers: {:#x}\n", a.length, a.nr_relocs, a.nr_line_numbers); },
                        [&temp](const struct arancini::pe::aux_sym_file& a) { std::format_to(std::back_inserter(temp), "File symbol, name of src file: {}", a.name); },
                        [&temp](const struct arancini::pe::aux_sym_clr_token_def& a) { std::format_to(std::back_inserter(temp), "CLR token def, bAuxType must be IMAGE_AUX_SYMBOL...: {:#x}\n Symtbl index of the COFF symbol this CLR token refers to: {:#x}\n", a.bAuxType, a.sym_table_idx); },
                        [&temp](std::monostate) { std::format_to(std::back_inserter(temp), "Not able to identify auxiliary symbol."); },   //somehow necessary, even if I use std::monoplate here it wouldn't accept it
                    };
                    std::visit(type, aux_sym);
                }   
            }
        }   else {
            std::format_to(std::back_inserter(temp), "The binary/object file has no coff symbol table.\n");
        }

        std::format_to(std::back_inserter(temp), "Print certificate table if it exists:\n");

        auto attr_cert_tbl = obj.get_cert_table();
        if(attr_cert_tbl) {
            for (const auto& cert_entry : *attr_cert_tbl) {
                std::format_to(std::back_inserter(temp), "Cert entry, Length: {:#x}, certificate version: {:#x}, certificate type: {:#x}\nbCertificate: ", cert_entry.dwLength, cert_entry.wRevision, cert_entry.wCertificateType);
            }
        }   else {
            std::format_to(std::back_inserter(temp), "Thie binary has no certificate tables.\n");
        }

        //export stuff
        std::format_to(std::back_inserter(temp), "Printing export information, if it exists.\n");
        
        auto export_dir_tbl = obj.get_export_dir_tbl();
        auto export_addr_tbl = obj.get_export_addr_tbl();
        auto export_names_ordinals = obj.get_export_names_ordinals();
        if(export_dir_tbl) {
            std::format_to(std::back_inserter(temp), "Name of DLL: {}\nBase-ordinal: {}, NR of entries in export addr table: {}, NR of entries in name/ordinal table: {}\nExport-addr-tbl RVA: {:#x}, name-ptr-tbl RVA: {:#x}, ordinal-table RVA: {:#x}\n", export_dir_tbl->name, export_dir_tbl->ordinal_base, export_dir_tbl->address_tbl_entries_size, export_dir_tbl->name_ptr_size, export_dir_tbl->export_addr_tbl_rva, export_dir_tbl->name_ptr_rva, export_dir_tbl->ordinal_tbl_rva);
            std::format_to(std::back_inserter(temp), "Now printing either an RVA of an exported symbol or its forwarder RVA string:\n");
            for(const auto& entry_point_addr : *export_addr_tbl) {
                std::visit(overloaded{
                    [&temp](const std::string& a){ std::format_to(std::back_inserter(temp), "Forwarder RVA (points to symbol defined in another DLL), name of symbol: {}", a); },
                    [&temp](uint32_t a) { std::format_to(std::back_inserter(temp), "RVA of exported symbol is {:#x}", a); },
                    [&temp](std::monostate) { std::format_to(std::back_inserter(temp), "exported symbols address entry has unknown type? Error while parsing? "); }
                }, entry_point_addr);
            }
            std::format_to(std::back_inserter(temp), "Now printing the ordinal name pairs of the expected symbols:\n");
            for(const auto& name_ordinal_pair : *export_names_ordinals) {
                std::format_to(std::back_inserter(temp), "Exported symbol with name: {} has unbiased ordinal number: {}\n", name_ordinal_pair.export_name, name_ordinal_pair.export_ordinal_unbiased);
            }
        }   else {
            std::format_to(std::back_inserter(temp), "This binary has no export directory table.\n");
        }

        //Import stuff
        std::format_to(std::back_inserter(temp), "Now printing the import dir entries for every imported DLL and the name/ordinal of every imported symbol:\n");
        auto import_dir_tbl = obj.get_import_directories();
        if(import_dir_tbl) {
            for(const auto& imported_dll : *import_dir_tbl) {
                std::format_to(std::back_inserter(temp), "Name of imported DLL: {}, forwarder chain value: {:#x}\n", imported_dll.name, imported_dll.forwarder_chain);
                for(const auto& imported_sym : imported_dll.imported_symbols) {
                    std::visit(overloaded{
                        [&temp](uint16_t ordinal) { std::format_to(std::back_inserter(temp), "Import by ordinal: Ordinal of imported symbol: {}\n", ordinal); },
                        [&temp](const std::string& name) { std::format_to(std::back_inserter(temp), "Import by name: Name of imported symbol: {}\n", name); },
                        [&temp](std::monostate) { std::format_to(std::back_inserter(temp), "Couldn't determine the type of the imported symbol identifier, neither name nor ordinal.\n"); },
                    }, imported_sym);
                }
            }
        }

        //Pdata/Exception handling stuff
        std::format_to(std::back_inserter(temp), "Now printing pdata contents:\n");
        auto pdata_funcs = obj.get_procedure_data_funcs();
        if(pdata_funcs) {
            for(const auto& func_entry : *pdata_funcs){
                std::format_to(std::back_inserter(temp), "Begin address (RVA) of the corresponding exception handling func: {:#x}, End Address (RVA): {:#x}, RVA of unwind information: {:#x}\n", func_entry.begin_addr_rva, func_entry.end_addr_rva, func_entry.unwind_info_rva);
            }
        }

        //reloc section
        std::format_to(std::back_inserter(temp), "Printing the reloc information from the .reloc section for the base relocs in the image:\n");
        auto base_reloc_blocks = obj.get_reloc_blocks();
        if(base_reloc_blocks) {
            for(const auto& reloc_block : *base_reloc_blocks) {
                std::format_to(std::back_inserter(temp), "Page RVA (add this to the the offset of each reloc to get the VA where the reloc will be applied): {:#x}, Type + Offsets\n", reloc_block.page_rva);
                for(const auto& type_offset : reloc_block.offsets) {
                    std::format_to(std::back_inserter(temp), "Type + Offset: (highest 4 bits/half byte determine the type, lower 12 the offset): {:#x}\n", type_offset);
                }
            }
        }
        return std::formatter<string_view>::format(temp, ctx);
    }
};
*/  //Needs >c++23 to function