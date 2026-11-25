#include <arancini/runtime/exec/win/execution-context-win.h>
#include <arancini/runtime/exec/win/execution-thread-win.h>
#include <arancini/runtime/exec/x86/x86-cpu-state.h>

#include <memoryapi.h>
#include <winternl.h>
#include <stdexcept>
#include <errhandlingapi.h>

using namespace arancini::runtime::exec;

execution_context_win::execution_context_win() 
    : memory_(nullptr), memory_size_(0x10000000ull) {
    allocate_guest_memory();
    process_env_block_ = (struct _PEB *)std::malloc(sizeof(struct _PEB));
    if(!process_env_block_) {
        throw std::runtime_error("Not enough memory to allocate PEB!\n");
    }

    setup_peb();
}

// see https://ntdoc.m417z.com/peb
void execution_context_win::setup_peb() {
    auto ptr_teb = NtCurrentTeb();
    auto ptr_peb = ptr_teb->ProcessEnvironmentBlock;

    memcpy(process_env_block_, ptr_peb, sizeof(struct _PEB));
    
    //Not sure what to change here, maybe replace the default heap from the main process
    //with one specific to the guest process (and alter ProcessHeap, NumberOfHeaps, etc.)
    //maybe copy some of these values over from the guest program header
    //maybe also change the ImageBaseAddress to the one where the guest sections are loaded into memory

    //process loader data - no changes? info on loaded modules, should be handled by host
    //process params - no changes (command line params of the current process)
}

void execution_context_win::allocate_guest_memory() {
    memory_ = VirtualAlloc(nullptr, memory_size_, 
        MEM_RESERVE | MEM_COMMIT, PAGE_NOACCESS);
    if (memory_ == NULL) {
        throw std::runtime_error("Unable to allocate guest memory.\n");
    }
}

void *execution_context_win::add_memory_region(off_t base_address, size_t size) {
    if((base_address + size) > memory_size_) {
        throw std::runtime_error("mem region out of bounds");
    }

    uintptr_t base_ptr = (uintptr_t)memory_ + base_address;
    uintptr_t aligned_base_ptr = base_ptr & ~0xfffull;  //aligned to 4kb page size
    uintptr_t base_ptr_off = base_ptr & 0xfffull;
    uintptr_t aligned_size = (size + base_ptr_off + 0xfff) & ~0xfffull;

    VirtualProtect((void *)aligned_base_ptr, aligned_size, PAGE_READWRITE, NULL);

    return (void *)base_ptr;
}

std::shared_ptr<execution_thread_win> 
execution_context_win::create_execution_thread() {
    auto et = std::make_shared<execution_thread_win>(*this, sizeof(x86::x86_cpu_state));
    threads_[et->get_cpu_state()] = et;

    return et;
}

std::shared_ptr<execution_thread_win> 
execution_context_win::get_thread(void *cpu_state) {
    return threads_.at(cpu_state);
}

execution_context_win::~execution_context_win() {
    std::free(process_env_block_);
}