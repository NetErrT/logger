# Logger
Logger is simple logger written in C

## Overview
For set configuration see [here](https://github.com/NetErrT/logger_documentation).

### Log functions:
* logger_trace
* logger_debug
* logger_info
* logger_warn
* logger_error
* logger_fatal

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
