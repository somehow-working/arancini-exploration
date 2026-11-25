#pragma once

#include <cstdlib>
#include <memory>
#include <map>

//windows stuff
#include <memoryapi.h>
#include <winternl.h>

namespace arancini::runtime::exec {
class execution_thread_win;

class execution_context_win {
    public:
        execution_context_win();
        ~execution_context_win();

        void *add_memory_region(off_t base_address, size_t size);

        void* get_peb() { return process_env_block_; };

        std::shared_ptr<execution_thread_win> create_execution_thread();
        std::shared_ptr<execution_thread_win> get_thread(void *cpu_state);

        //some version of internal call will be necessary to ensure that allocations are correct
        //i.e. will probably have to intercept calls to:
        // arancini used to just intercept calls to mmap or brk, now have to intercept equivalent windows calls
        /*
        *   NtAllocateVirtualMemory for memory allocs
        *   RtlCreateUserThread for thread creation
        */
        //int internal_call()

    private:
        void* memory_;
        size_t memory_size_;
        struct _PEB *process_env_block_; //every windows process has one of these
        //per thread you also have a thread execution block

        std::map<void *, std::shared_ptr<execution_thread_win>> threads_;
        void allocate_guest_memory();
        void setup_peb();
};
}