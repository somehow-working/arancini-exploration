#pragma once

#include <cstdint>
#include <memory>
#include <iostream>

#include <winternl.h>

namespace arancini::runtime::exec {
class execution_context_win;

class execution_thread_win {
    public:
        execution_thread_win(execution_context_win &owner, size_t state_size);
        ~execution_thread_win();

        void *get_cpu_state() const { return cpu_state_; }

        void setup_tib(void *stack_base, void*stack_top);

        struct _TEB *get_teb() const { return thread_execution_block_; }

        void clk(char *s) {
            struct timespec ts;
            clock_gettime(CLOCK_THREAD_CPUTIME_ID, &ts);
            std::cerr << std::dec << s << ":\t"
                    << ts.tv_sec * 1000000000 + ts.tv_nsec << std::endl;
        }
    
    private:
        struct _TEB *thread_execution_block_;
        execution_context_win &owner_;
        void *cpu_state_;
        size_t cpu_state_size_;
        void setup_teb();
};
}