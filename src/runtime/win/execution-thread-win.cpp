#include <arancini/runtime/exec/win/execution-thread-win.h>
#include <arancini/runtime/exec/win/execution-context-win.h>

#include <stdexcept>

using namespace arancini::runtime::exec;
execution_thread_win::execution_thread_win(execution_context_win &owner, size_t state_size)
    : cpu_state_size_(state_size), cpu_state_(nullptr), owner_(owner), thread_execution_block_(nullptr) {
    cpu_state_ = std::malloc(state_size);
    if(!cpu_state_) {
        throw std::runtime_error("unable to allocate storage for CPU state!\n");
    }

    thread_execution_block_ = (struct _TEB*)std::malloc(sizeof(struct _TEB));
    if(!thread_execution_block_) {
        throw std::runtime_error("unable to allocate storage for TEB\n");
    }

    setup_teb();
}

// see https://ntdoc.m417z.com/teb
void execution_thread_win::setup_teb() {
    //copy host TEB
    memcpy(thread_execution_block_, NtCurrentTeb(), sizeof(struct _TEB));

    auto ptr_teb = (void**)thread_execution_block_;
    ptr_teb[6] = thread_execution_block_;   //self pointer in TIB
    *(ptr_teb + 0x0060) = owner_.get_peb();   //pointer to the PEB of the current process

    // host process does not have TLS stuff (OR???) so that will need to be allocated:
    if(*(ptr_teb + 0x0058) == NULL) {
        //if no TLS is there this is NULL, but DLLs might have so this is probably wrong
        //allocate TLS heap here...
    }

    if(*(ptr_teb + 0x025c) == NULL) {
        //should be the heap for Fiber local data, does not appear in wine howver
        //TODO
    }
    //TODO for setup, update stack pointers, done in setup_tib
}

void execution_thread_win::setup_tib(void *stack_base, void*stack_top) {
    auto ptr_teb = (void**)thread_execution_block_;
    *(ptr_teb + 0x0008) = stack_top; //stackBase in the TIB refers to the highest address of the stack
    *(ptr_teb[2] + 0x0010) = stack_base;    //bottom of currently commited stack region one below guard page
    *(ptr_teb + 0x1478) = stack_base;    //offset from wine TEB struct for win64
}

execution_thread_win::~execution_thread_win() {
    std::free(cpu_state_);
    std::free(thread_execution_block_);
}