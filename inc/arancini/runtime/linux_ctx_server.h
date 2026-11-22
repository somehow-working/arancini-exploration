#pragma once

#include <cstdint>

#define LINUX_RT_PORT 2010
enum class remote_funcs : uint8_t {
    invoke_code = 0,
    execute_internal_call,
    initialise_dynamic_rt,
    finalize,
    kill,   //used to shutdown the server
};
