# Documentation for logger

## Overview
For set configuration see [here](@ref logger_set_config).

### Log functions:
* [logger_trace](@ref logger_trace)
* [logger_debug](@ref logger_debug)
* [logger_info](@ref logger_info)
* [logger_warn](@ref logger_warn)
* [logger_error](@ref logger_error)
* [logger_fatal](@ref logger_fatal)

## Examples
### Hello World
#### main.c:
```c
#include <logger.h>

int
main(void) {
    logger_config_t config = {
        .date = LOGGER_DATE_FULL,
        .coloring = LOGGER_COLORING_FULL,
        .log_file = NULL, /* use logger_file_open instread */
    };

    logger_set_config(config);
    logger_file_open("logger.log");

    logger_info("Hello World!");

    return 0;
}
```
#### output:
info 2024.08.05 14.10.01.262 main.c:14: Hello World!
