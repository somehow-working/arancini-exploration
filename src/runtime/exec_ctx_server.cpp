#include <sys/socket.h>
#include <sys/un.h>
#include <stdio.h>
#include <unistd.h>
#include <netinet/in.h>
#include <errno.h>
#include <string>

#include <arancini/runtime/linux_ctx_server.h>

int main() {
    printf("Server startup\n");
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

    bool not_called_kill = true;

    while(not_called_kill) {
        //auto cfd = accept(server_socket, NULL, NULL);
        char buf[5000];
        memset(buf, '\0', sizeof(buf));
        auto nread = recv(server_socket, buf, sizeof(buf)-1, 0);
        if(nread == -1) {
            printf("Failed to read from source! Continueing...\n");
            //close(cfd);
            continue;
        }

        printf("Linux Server: Read string: %s\n", buf);

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
            printf("Translating Instr at PC: %d\n", *pc_ptr);
            break;
        }
        case remote_funcs::initialise_dynamic_rt:
            //call some init func
            break;
        case remote_funcs::execute_internal_call:
            //not sure yet whether we need anything here, syscalls are just dll calls
            break;
        case remote_funcs::finalize:
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