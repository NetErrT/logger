#if !defined(LOGGER_H)
#define LOGGER_H

#if defined(__cplusplus)
extern "C" {
#endif /* !defined(__cplusplus) */

#include <stdbool.h>
#include <stdio.h>

#define LOGGER_VERSION_MAJOR 0
#define LOGGER_VERSION_MINOR 1

#define LOGGER_COLOR_RED     "\x1B[31m"
#define LOGGER_COLOR_GREEN   "\x1B[32m"
#define LOGGER_COLOR_YELLOW  "\x1B[33m"
#define LOGGER_COLOR_BLUE    "\x1B[34m"
#define LOGGER_COLOR_MAGENTA "\x1B[35m"
#define LOGGER_COLOR_CYAN    "\x1B[36m"
#define LOGGER_COLOR_RESET   "\033[0m"

#define LOGGER_LIMIT_MESSAGE 256 /*!< limit max message size with zero symbol */
#define LOGGER_LIMIT_DATE    32  /*!< limit max date size with zero symbol */

typedef enum {
    LOGGER_LOG_TRACE = 0,
    LOGGER_LOG_DEBUG,
    LOGGER_LOG_INFO,
    LOGGER_LOG_WARN,
    LOGGER_LOG_ERROR,
    LOGGER_LOG_FATAL,
} logger_log_level_t;

/*! \brief Controls coloring formats */
typedef enum {
    LOGGER_COLORING_DISABLE = 0, /*!< disable coloring */
    LOGGER_COLORING_FULL,        /*!< message coloring */
    LOGGER_COLORING_TAG,         /*!< tag coloring */
} logger_coloring_control_t;

/*! \brief Controls date formats */
typedef enum {
    LOGGER_DATE_DISABLE = 0, /*!< disable date */
    LOGGER_DATE_TIME_ONLY,   /*!< show time with milliseconds */
    LOGGER_DATE_ONLY,        /*!< show date */
    LOGGER_DATE_FULL,        /*!< show date and time */
} logger_date_control_t;

/*! This configuration is set by \ref logger_set_config */
typedef struct {
    logger_coloring_control_t coloring;
    logger_date_control_t date;
    FILE* log_file;
} logger_config_t;

/*! \brief Print message to console
 *
  If the log level is `error` or `fatal` the message printed to `stderr`
 * \param log_level loggin level
 * \param file_name current file name
 * \param file_line current file line
 * \param format format message
 */
void logger_print(logger_log_level_t log_level, const char* file_name, int file_line, const char* format, ...);

/*! \brief Opens a log file
 *
 * This function opens log file. If the file does not exist, it may be created.
 * If the log file is already open, it will be closed before opening the new log file.
 *
 * \param path path to log file
 */
void logger_file_open(const char* path);

/*! \brief Closes the most recently opened log file
 */
void logger_file_close(void);

/*! \brief Sets the configuration in runtime
 * \param config the new configuration
 */
void logger_set_config(logger_config_t config);

#define logger_trace(...) logger_print(LOGGER_LOG_TRACE, __FILE__, __LINE__, __VA_ARGS__);
#define logger_debug(...) logger_print(LOGGER_LOG_DEBUG, __FILE__, __LINE__, __VA_ARGS__);
#define logger_info(...)  logger_print(LOGGER_LOG_INFO, __FILE__, __LINE__, __VA_ARGS__);
#define logger_warn(...)  logger_print(LOGGER_LOG_WARN, __FILE__, __LINE__, __VA_ARGS__);
#define logger_error(...) logger_print(LOGGER_LOG_ERROR, __FILE__, __LINE__, __VA_ARGS__);
#define logger_fatal(...) logger_print(LOGGER_LOG_FATAL, __FILE__, __LINE__, __VA_ARGS__);

#if defined(__cplusplus)
}
#endif /* !defined(__cplusplus) */

#endif /* defined(LOGGER_H) */
