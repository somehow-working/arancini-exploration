#include <arancini/runtime/dbt/translation.h>
#include <arancini/runtime/exec/x86/x86-cpu-state.h>
#include <arancini/runtime/exec/execution-context.h>

#include <arancini/input/x86/x86-input-arch.h>

#include <arancini/elf/reader.h>

#include <sys/socket.h>
#include <sys/un.h>
#include <stdio.h>
#include <unistd.h>
#include <netinet/in.h>
#include <errno.h>
#include <string>

#include <arancini/runtime/linux_ctx_server.h>

using namespace arancini::runtime::exec;
using namespace arancini::runtime::dbt;

#ifndef NDEBUG
static arancini::input::x86::x86_input_arch
    ia(true, arancini::input::x86::disassembly_syntax::intel);
#else
static arancini::input::x86::x86_input_arch
    ia(false, arancini::input::x86::disassembly_syntax::intel);
#endif

#if defined(ARCH_X86_64)
static arancini::output::dynamic::x86::x86_dynamic_output_engine oe;
#elif defined(ARCH_AARCH64)
static arancini::output::dynamic::arm64::arm64_dynamic_output_engine oe;
#elif defined(ARCH_RISCV64)
static arancini::output::dynamic::riscv64::riscv64_dynamic_output_engine oe;
#else
#error "Unsupported dynamic output architecture"
#endif

int main() {
    printf("Server startup\n");
    //this execution ctx does nothing and only the get_mem_ptr variable will get called at some point
    auto dummy_exec_ctx = execution_context(ia, oe, true);
    translation_engine &te_ = dummy_exec_ctx.get_te_ref();

    auto server_socket = socket(AF_INET, SOCK_DGRAM, 0);
    if(server_socket == -1) {
        printf("Something went wrong when creating the socket!\n");
        return 1;
    }
    const int i = 1;
    if(setsockopt(server_socket, SOL_SOCKET, SO_REUSEPORT, &i, sizeof(i)) < 0) {
        printf("Something went wronmg when setting the correct socket option %d!\n", errno);
    }
    if(setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &i, sizeof(i)) < 0) {
        printf("Something went wronmg when setting the correct socket option %d!\n", errno);
    }

    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = LINUX_RT_PORT;
    server_addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

    if(bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        printf("Problem when binding at server init: %d\n", errno);
        return 1;
    }

    struct sockaddr_in client_addr;
    memset(&server_addr, 0, sizeof(client_addr));
    socklen_t client_addr_size = sizeof(client_addr);

    bool not_called_kill = true;

    while(not_called_kill) {
        //auto cfd = accept(server_socket, NULL, NULL);
        char buf[5000];
        memset(buf, '\0', sizeof(buf));
        auto nread = recvfrom(server_socket, buf, sizeof(buf)-1, 0, (struct sockaddr*)&client_addr, &client_addr_size);
        if(nread == -1) {
            printf("Server: Failed to read from source! Continueing...\n");
            //close(cfd);
            continue;
        }

        printf("Linux Server: Read string: %s\n", buf);
        if(connect(server_socket, (struct sockaddr*)&client_addr, sizeof(client_addr)) == -1) {
            printf("Server: Failed to connect to client. No return values possible.\n");
        }

        remote_funcs func_type = static_cast<remote_funcs>(buf[0]);

        switch (func_type)
        {
        case remote_funcs::kill:
            not_called_kill = false;
            break;
        
        case remote_funcs::invoke_code:
        {
            std::string exec_path{buf+1};
            uint64_t *pc_ptr = reinterpret_cast<uint64_t*>(buf + exec_path.size() + 2); //+2, jump over first byte and terminating \0 of c-string
            printf("Translating executable %s and Instr at PC: %d\n", exec_path, *pc_ptr);
            auto exec_file_reader = unmanaged_file(exec_path);
            auto exec_ptr = exec_file_reader.map();

            auto txln = te_.get_translation(reinterpret_cast<unsigned long>(exec_ptr + *pc_ptr));

            if(send(server_socket, txln->get_code_ptr(), txln->get_code_size(), 0) == -1) {
                printf("Server: Sending the generated code was a failure!\n");
            }
            break;
        }
        case remote_funcs::initialise_dynamic_rt:
            //call some init func
            break;
        case remote_funcs::execute_internal_call:
            //not sure yet whether we need anything here, syscalls are just dll calls
            break;
        case remote_funcs::finalize:
            not_called_kill = false;
            //delete anythign necessary; windows execution context
            break;
        default:
            printf("Unknown func_type was transmitted!: %c, Ignore fo now\n", (char)func_type);
            break;
        }

        printf("Read string: %s\n", buf);
        //close(cfd);
    }
    return 0;
}