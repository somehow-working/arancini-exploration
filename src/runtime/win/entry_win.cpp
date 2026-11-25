#include <arancini/runtime/exec/x86/x86-cpu-state.h>
#include <arancini/runtime/exec/win/execution-context-win.h>
#include <arancini/runtime/exec/win/execution-thread-win.h>
#include <arancini/runtime/linux_ctx_server.h>

#include <unordered_map>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <fileapi.h>
#include <processthreadsapi.h>
#include <synchapi.h>
#include <handleapi.h>
#include <stdio.h>
#include <stdint.h>
#include <cstdlib>

#include <memoryapi.h>
#include <winternl.h>
#include <winnt.h>

//local
static std::unordered_map<unsigned long, void *> fn_addrs;
static SOCKET client_socket;
static void *guest_teb;
static arancini::runtime::exec::execution_context_win *ctx_;

//asm trampoline
struct native_call_result {
    int exit_code;
    uint64_t chain_address;
};

extern "C" native_call_result call_native(void *, void *);

using namespace arancini::runtime::exec;

static x86::x86_cpu_state *__current_state;

void start_linux_server(){
    STARTUPINFO si;
	PROCESS_INFORMATION pi;

	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	ZeroMemory(&pi, sizeof(pi));

	auto thread_created = CreateProcessA("linux-server", NULL, NULL, NULL, false, 0, NULL, NULL, &si, &pi);
	if(!thread_created) {
		printf("server thread was not started!\n");
		return;
	}
	WaitForSingleObject(pi.hProcess, INFINITE);

	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);

    //address where the linux rt server should be listening: localhost::2010
    SOCKADDR_IN serverAddr;
	ZeroMemory(&serverAddr, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = LINUX_RT_PORT;
	serverAddr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

    int cnt = 1;
    for(; cnt < 5; cnt++) {
        client_socket = socket(AF_INET, SOCK_DGRAM, 0); //udp socket
        if(client_socket == INVALID_SOCKET) {
            printf("Error at socket creation: %ld\n", WSAGetLastError());
            WSACleanup();
            abort();    //no way to do this else??
        }

        if(connect(client_socket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) != -1) {
            break;
        }

        printf("connecting to server failed: %ld\n", WSAGetLastError());
        Sleep(cnt*500); //give the server some time to start up
    }

    if(cnt == 5) {
        printf("All connecting attempts failed\n");
        WSACleanup();
        abort();
    }
}



extern "C" void *initialise_dynamic_runtime(unsigned long entry_point, int argc, char **argv) {
    //start_linux_server();
    //Get ptr to current thread execution block/ process environment block
    /*guest_teb = (void *)malloc(sizeof(struct _TEB));
    void *ret = memcpy(guest_teb, (void *)NtCurrentTeb(), sizeof(struct _TEB));

    *(void**)(guest_teb+1) = nullptr; //ptr to stack base
    *(void**)(guest_teb+2) = 0; //stack size (last address of this threads stack)
    *(void**)(guest_teb+6) = guest_teb; //self pointer to teb*/
    ctx_ = new execution_context_win();

    unsigned long stack_size = 0x100000;    //1MB is default stack size
    auto stack_base = ctx_->add_memory_region(0x10000000 - stack_size, stack_size);
    auto stack_top = stack_base + stack_size;

    auto main_thread = ctx_->create_execution_thread();

    // Initialise the CPU state structure with the PC set to the entry point of
    // the guest program, and an emulated stack pointer at the top of the
    // emulated address space.
    x86::x86_cpu_state *x86_state = (x86::x86_cpu_state *)main_thread->get_cpu_state();
    __current_state = x86_state;
    x86_state->PC = entry_point;
    x86_state->RSP = reinterpret_cast<uint64_t>(stack_top);

    main_thread->setup_tib(stack_base, stack_top);

    x86_state->X87_STACK_BASE = VirtualAlloc(NULL, 64, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    x86_state->X87_TAG = std::uint16_t(0xFFFF);
    x86_state->X87_CTRL = std::uint16_t(0x037F);

    return main_thread->get_cpu_state();
}

/**
 * Register a static function so it can be called from the main loop
 */
extern "C" void register_static_fn_addr(unsigned long guest_addr,
                                        void *fn_addr) {
    fn_addrs.emplace(guest_addr, fn_addr);
}

/**
 * Look up a static function. Returns nullptr if the function was not found.
 */
extern "C" void *lookup_static_fn_addr(unsigned long guest_addr) {
    printf("Looking up guest address: %ld\n", guest_addr);
    if (auto item = fn_addrs.find(guest_addr); item != fn_addrs.end()) {
        return item->second;
    }

    return nullptr;
}

extern "C" void poison(char *s) {
    std::cerr << "Unimplemened Instr: " << s << "\n";
    abort();
}

extern "C" void alert() { std::cout << "Top of MainLoop!\n"; }

/*std::string construct_msg(remote_funcs func) {

}*/

extern "C" void clk(void *cpu_state, char *s) {
    printf("CLK not yet implemented!\n");
}

extern "C" int execute_internal_call(void *cpu_state, int call) {
    printf("internal call not yet implemented!\n");
    return 0;
}

extern "C" void finalize() {
    delete ctx_;
    WSACleanup();
    exit(0);
}

extern "C" int invoke_code(void *cpu_state) {
    uint64_t pc = ((arancini::runtime::exec::x86::x86_cpu_state *)cpu_state)->PC;
    printf("Got the following PC to look up: %d\n", pc);
    
    char msg[5000]; //enough space
    wchar_t path_buf[2000];
    msg[0] = static_cast<char>(remote_funcs::invoke_code);
    auto n_copied = GetModuleFileNameW(NULL, path_buf, sizeof(path_buf) / sizeof(wchar_t));
    if(n_copied == 0) {
        printf("Could not get the path to the current executable!\n");
        abort();
    }
    n_copied = wcstombs(msg+1, path_buf, sizeof(msg) - 2);
    if(n_copied == static_cast<size_t>(-1)) {
        printf("Path of executable contains weird unicode signs. Path is: %ls\n", path_buf);
        abort();
    }

    char *pc_ptr = reinterpret_cast<char*>(&pc);
    memcpy(msg + n_copied + 2, pc_ptr, sizeof(uint64_t));   // +2, 1 for the first byte, 1 for the terminating \0 of the exe path

    if(send(client_socket, msg, n_copied+2+sizeof(uint64_t), MSG_DONTROUTE) == -1) {
        printf("Client: Not able to send the request to the server!\n");
    }

    auto base_ = VirtualAlloc(nullptr, 0x100000000, MEM_RESERVE | MEM_COMMIT, PAGE_EXECUTE_READWRITE);
    if(!base_) {
        printf("Not possible to allocate enough memory to put the generated code into!");
    }

    auto code_size = recv(client_socket, base_, 0x100000000, 0);
    if(code_size == -1) {
        printf("Error when receiving the generated code.\n");
    }

    //release some commited pages, no idea if that actually impacts performance
    void *aligned_end_ptr = (base_ + code_size + 0xfff) & ~0xfff;
    auto aligned_freed_size = 0x100000000 - code_size;
    if(VirtualFree(aligned_end_ptr, aligned_freed_size, MEM_DECOMMIT)) {
        printf("error when freeing ");
    }

    int ret = call_native(base_, cpu_state);
    return 0;
}