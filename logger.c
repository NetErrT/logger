#include "logger.h"

#include <assert.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#ifdef _WIN32
#include <windows.h>
#else
#include <errno.h>
#endif

static logger_config_t g_config;

static const char*
get_tag(logger_log_level_t log_level) {
    const char* tag;

    switch (log_level) {
        case LOGGER_LOG_TRACE: tag = "trace"; break;
        case LOGGER_LOG_DEBUG: tag = "debug"; break;
        case LOGGER_LOG_INFO: tag = "info"; break;
        case LOGGER_LOG_WARN: tag = "warn"; break;
        case LOGGER_LOG_ERROR: tag = "error"; break;
        case LOGGER_LOG_FATAL: tag = "fatal"; break;
    }

    return tag;
}

static const char*
get_color(logger_log_level_t log_level) {
    const char* color;

    switch (log_level) {
        case LOGGER_LOG_TRACE: color = LOGGER_COLOR_GREEN; break;
        case LOGGER_LOG_INFO: color = LOGGER_COLOR_CYAN; break;
        case LOGGER_LOG_DEBUG: color = LOGGER_COLOR_BLUE; break;
        case LOGGER_LOG_WARN: color = LOGGER_COLOR_YELLOW; break;
        case LOGGER_LOG_ERROR: color = LOGGER_COLOR_RED; break;
        case LOGGER_LOG_FATAL: color = LOGGER_COLOR_MAGENTA; break;
    }

    return color;
}

static FILE*
get_stream(logger_log_level_t log_level) {
    switch (log_level) {
        case LOGGER_LOG_ERROR:
        case LOGGER_LOG_FATAL: return stderr;
        default: return stdout;
    }
}

static int
get_time(char date[], int offset, struct tm* info, unsigned long ns) {
    assert(date != NULL);
    assert(offset <= LOGGER_LIMIT_DATE);
    assert(info != NULL);

    return snprintf(date + offset, LOGGER_LIMIT_DATE - offset, " %02d.%02d.%02d.%03d", info->tm_hour, info->tm_min,
                    info->tm_sec, (uint16_t)(ns / 1000000));
}

static int
get_date_only(char date[], int offset, struct tm* info) {
    assert(date != NULL);
    assert(offset <= LOGGER_LIMIT_DATE);
    assert(info != NULL);

    return snprintf(date + offset, LOGGER_LIMIT_DATE - offset, " %04d.%02d.%02d", info->tm_year + 1900,
                    info->tm_mon + 1, info->tm_mday);
}

static void
get_date(char date[]) {
    assert(date != NULL);

    date[0] = '\0';

    if (LOGGER_DATE_DISABLE) {
        return;
    }

    struct timespec tm;
    clock_gettime(CLOCK_REALTIME, &tm);

    struct tm info;
#ifdef _WIN32
    localtime_s(&info, &tm.tv_sec);
#else
    localtime_r(&tm.tv_sec, &info);
#endif

    switch (g_config.date) {
        case LOGGER_DATE_TIME_ONLY: get_time(date, 0, &info, tm.tv_nsec); break;
        case LOGGER_DATE_ONLY: get_date_only(date, 0, &info); break;
        case LOGGER_DATE_FULL: {
            int offset = get_date_only(date, 0, &info);
            get_time(date, offset, &info, tm.tv_nsec);
            break;
        }
            // For supress compiler warnings
        default: break;
    }
}

static void
get_message(char message[], const char* format, va_list args) {
    assert(message != NULL);
    assert(format != NULL);

    vsnprintf(message, LOGGER_LIMIT_MESSAGE, format, args);
}

void
logger_print(logger_log_level_t log_level, const char* file_name, int file_line, const char* format, ...) {
    assert(file_name != NULL);
    assert(format != NULL);

    char date[LOGGER_LIMIT_DATE];
    char message[LOGGER_LIMIT_MESSAGE];
    const char* tag = get_tag(log_level);

    get_date(date);

    {
        va_list args;
        va_start(args, format);

        get_message(message, format, args);

        va_end(args);
    }

    FILE* stream = get_stream(log_level);

    switch (g_config.coloring) {
        case LOGGER_COLORING_FULL: {
            const char* color = get_color(log_level);

            fprintf(stream, "%s%s%s %s:%d: %s%s\n", color, tag, date, file_name, file_line, message,
                    LOGGER_COLOR_RESET);
            break;
        }
        case LOGGER_COLORING_TAG: {
            const char* color = get_color(log_level);

            fprintf(stream, "%s%s%s%s %s:%d: %s\n", color, tag, LOGGER_COLOR_RESET, date, file_name, file_line,
                    message);
            break;
        }
        case LOGGER_COLORING_DISABLE:
            fprintf(stream, "%s%s %s:%d: %s\n", tag, date, file_name, file_line, message);
            break;
    }

    if (g_config.log_file) {
        fprintf(g_config.log_file, "%s%s %s:%d: %s\n", tag, date, file_name, file_line, message);
    }
}

void
logger_file_open(const char* path) {
    if (path == NULL)
        return;

#ifdef _WIN32
    FILE* log_file;
    errno_t error = fopen_s(&log_file, path, "a");
#else
    FILE* log_file = fopen(path, "a");
#endif

    if (log_file == NULL) {
        char buffer[64] = {0};
#ifdef _WIN32
        strerror_s(buffer, sizeof(buffer), error);
#else
        strerror_r(errno, buffer, sizeof(buffer));
#endif
        fprintf(stderr, "Logger: failed to open file: %s\n", buffer);
    }

    logger_file_close();

    g_config.log_file = log_file;
}

void
logger_file_close(void) {
    if (g_config.log_file == NULL)
        return;

    fclose(g_config.log_file);
    g_config.log_file = NULL;
}

void
logger_set_config(logger_config_t config) {
    g_config = config;
}
