/**
 * @file core_log.h
 * @brief CoreLog - Lightweight, flexible logging library for microcontrollers.
 *
 * Inspired by ESP-IDF ESP_LOG. Supports any output interface via function pointers.
 * Optional timestamp and ANSI color support.
 *
 * Usage:
 *   1. Call core_log_init() with your output function.
 *   2. Optionally call core_log_set_timestamp() for timestamp support.
 *   3. Optionally call core_log_set_color() to enable/disable colors.
 *   4. Use LOG_E, LOG_W, LOG_I, LOG_D, LOG_V macros to log messages.
 */

#ifndef CORE_LOG_H
#define CORE_LOG_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdarg.h>

/*============================================================================
 * Configuration
 *============================================================================*/

/**
 * @brief Maximum length of a single log message (including formatted output).
 *        Override at compile time with -DCORE_LOG_MAX_MSG_LEN=xxx
 */
#ifndef CORE_LOG_MAX_MSG_LEN
#define CORE_LOG_MAX_MSG_LEN 256
#endif

/*============================================================================
 * Log Levels
 *============================================================================*/

typedef enum {
    CORE_LOG_NONE    = 0, /**< No log output */
    CORE_LOG_ERROR   = 1, /**< Critical errors */
    CORE_LOG_WARN    = 2, /**< Warnings */
    CORE_LOG_INFO    = 3, /**< Informational messages */
    CORE_LOG_DEBUG   = 4, /**< Debug messages */
    CORE_LOG_VERBOSE = 5  /**< Verbose/trace messages */
} core_log_level_t;

/**
 * @brief Compile-time maximum log level.
 *        Messages above this level are removed at compile time (zero overhead).
 *        Override with -DCORE_LOG_LEVEL_MAX=CORE_LOG_DEBUG etc.
 */
#ifndef CORE_LOG_LEVEL_MAX
#define CORE_LOG_LEVEL_MAX CORE_LOG_VERBOSE
#endif

/*============================================================================
 * Function pointer types
 *============================================================================*/

/**
 * @brief Output function pointer type.
 *        Must behave like printf: format string + variadic args, return chars written.
 */
typedef int (*core_log_output_fn)(const char *fmt, ...);

/**
 * @brief Timestamp function pointer type.
 *        Should return a 32-bit timestamp value (e.g. milliseconds since boot).
 */
typedef uint32_t (*core_log_timestamp_fn)(void);

/*============================================================================
 * API Functions
 *============================================================================*/

/**
 * @brief Initialize the logging library with an output function.
 *
 * @param output_fn  Function pointer for output (printf-like). Must not be NULL.
 */
void core_log_init(core_log_output_fn output_fn);

/**
 * @brief Set the timestamp provider function (optional).
 *        If not set or set to NULL, logs will be printed without a timestamp.
 *
 * @param timestamp_fn  Function that returns a uint32_t timestamp, or NULL to disable.
 */
void core_log_set_timestamp(core_log_timestamp_fn timestamp_fn);

/**
 * @brief Enable or disable ANSI color output.
 *        Disabled by default. Enable only if the console supports ANSI escape codes.
 *
 * @param enable  1 to enable colors, 0 to disable.
 */
void core_log_set_color(int enable);

/**
 * @brief Set the runtime log level filter.
 *        Messages with a level above this value will be suppressed.
 *
 * @param level  Maximum log level to display.
 */
void core_log_set_level(core_log_level_t level);

/**
 * @brief Get the current runtime log level.
 *
 * @return Current log level.
 */
core_log_level_t core_log_get_level(void);

/**
 * @brief Core logging function. Prefer using the LOG_x macros instead.
 *
 * @param level   Log level of this message.
 * @param tag     Module/component tag string.
 * @param fmt     printf-style format string.
 * @param ...     Format arguments.
 */
void core_log_write(core_log_level_t level, const char *tag, const char *fmt, ...);

/*============================================================================
 * ANSI Color Codes
 *============================================================================*/

#define CORE_LOG_COLOR_RED     "\033[0;31m"
#define CORE_LOG_COLOR_GREEN   "\033[0;32m"
#define CORE_LOG_COLOR_YELLOW  "\033[0;33m"
#define CORE_LOG_COLOR_BLUE    "\033[0;34m"
#define CORE_LOG_COLOR_MAGENTA "\033[0;35m"
#define CORE_LOG_COLOR_CYAN    "\033[0;36m"
#define CORE_LOG_COLOR_RESET   "\033[0m"

/** Color mapping per log level (matches ESP-IDF style) */
#define CORE_LOG_COLOR_E       CORE_LOG_COLOR_RED
#define CORE_LOG_COLOR_W       CORE_LOG_COLOR_YELLOW
#define CORE_LOG_COLOR_I       CORE_LOG_COLOR_GREEN
#define CORE_LOG_COLOR_D       ""  /* No color (same as ESP-IDF) */
#define CORE_LOG_COLOR_V       ""  /* No color (same as ESP-IDF) */

/*============================================================================
 * Logging Macros
 *============================================================================*/

/**
 * Macros are conditionally compiled based on CORE_LOG_LEVEL_MAX.
 * At runtime they also check the configured log level.
 *
 * Usage:
 *   LOG_E("MY_TAG", "Something failed: %d", error_code);
 *   LOG_I("MAIN",   "System started");
 */

#if CORE_LOG_LEVEL_MAX >= CORE_LOG_ERROR
#define LOG_E(tag, fmt, ...) \
    core_log_write(CORE_LOG_ERROR, tag, fmt, ##__VA_ARGS__)
#else
#define LOG_E(tag, fmt, ...) ((void)0)
#endif

#if CORE_LOG_LEVEL_MAX >= CORE_LOG_WARN
#define LOG_W(tag, fmt, ...) \
    core_log_write(CORE_LOG_WARN, tag, fmt, ##__VA_ARGS__)
#else
#define LOG_W(tag, fmt, ...) ((void)0)
#endif

#if CORE_LOG_LEVEL_MAX >= CORE_LOG_INFO
#define LOG_I(tag, fmt, ...) \
    core_log_write(CORE_LOG_INFO, tag, fmt, ##__VA_ARGS__)
#else
#define LOG_I(tag, fmt, ...) ((void)0)
#endif

#if CORE_LOG_LEVEL_MAX >= CORE_LOG_DEBUG
#define LOG_D(tag, fmt, ...) \
    core_log_write(CORE_LOG_DEBUG, tag, fmt, ##__VA_ARGS__)
#else
#define LOG_D(tag, fmt, ...) ((void)0)
#endif

#if CORE_LOG_LEVEL_MAX >= CORE_LOG_VERBOSE
#define LOG_V(tag, fmt, ...) \
    core_log_write(CORE_LOG_VERBOSE, tag, fmt, ##__VA_ARGS__)
#else
#define LOG_V(tag, fmt, ...) ((void)0)
#endif

#ifdef __cplusplus
}
#endif

#endif /* CORE_LOG_H */
