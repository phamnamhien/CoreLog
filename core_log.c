/**
 * @file core_log.c
 * @brief CoreLog implementation.
 */

#include "core_log.h"
#include <string.h>
#include <stdio.h>

/*============================================================================
 * Private state
 *============================================================================*/

static core_log_output_fn    s_output_fn    = NULL;
static core_log_timestamp_fn s_timestamp_fn = NULL;
static core_log_level_t      s_log_level    = CORE_LOG_VERBOSE;
static int                   s_color_enable = 0;

/*============================================================================
 * Level-to-string / color lookup
 *============================================================================*/

static const char *level_to_letter(core_log_level_t level)
{
    switch (level) {
        case CORE_LOG_ERROR:   return "E";
        case CORE_LOG_WARN:    return "W";
        case CORE_LOG_INFO:    return "I";
        case CORE_LOG_DEBUG:   return "D";
        case CORE_LOG_VERBOSE: return "V";
        default:               return "?";
    }
}

static const char *level_to_color(core_log_level_t level)
{
    switch (level) {
        case CORE_LOG_ERROR:   return CORE_LOG_COLOR_E;
        case CORE_LOG_WARN:    return CORE_LOG_COLOR_W;
        case CORE_LOG_INFO:    return CORE_LOG_COLOR_I;
        case CORE_LOG_DEBUG:   return CORE_LOG_COLOR_D;
        case CORE_LOG_VERBOSE: return CORE_LOG_COLOR_V;
        default:               return "";
    }
}

/*============================================================================
 * Public API
 *============================================================================*/

void core_log_init(core_log_output_fn output_fn)
{
    s_output_fn = output_fn;
}

void core_log_set_timestamp(core_log_timestamp_fn timestamp_fn)
{
    s_timestamp_fn = timestamp_fn;
}

void core_log_set_color(int enable)
{
    s_color_enable = enable ? 1 : 0;
}

void core_log_set_level(core_log_level_t level)
{
    s_log_level = level;
}

core_log_level_t core_log_get_level(void)
{
    return s_log_level;
}

void core_log_write(core_log_level_t level, const char *tag, const char *fmt, ...)
{
    /* No output function configured – silently discard */
    if (s_output_fn == NULL) {
        return;
    }

    /* Runtime level filter */
    if (level > s_log_level) {
        return;
    }

    const char *letter = level_to_letter(level);

    /*
     * Print the log prefix:
     *   With timestamp + color : "\033[0;31mE (12345) TAG: "
     *   With timestamp no color: "E (12345) TAG: "
     *   No timestamp + color   : "\033[0;31mE TAG: "
     *   No timestamp no color  : "E TAG: "
     */
    if (s_color_enable) {
        s_output_fn("%s", level_to_color(level));
    }

    if (s_timestamp_fn != NULL) {
        uint32_t ts = s_timestamp_fn();
        s_output_fn("%s (%lu) %s: ", letter, (unsigned long)ts, tag ? tag : "");
    } else {
        s_output_fn("%s %s: ", letter, tag ? tag : "");
    }

    /* Print the user message via va_list ---------------------------------- */
    {
        char buf[CORE_LOG_MAX_MSG_LEN];
        va_list args;
        va_start(args, fmt);
        vsnprintf(buf, sizeof(buf), fmt, args);
        va_end(args);
        s_output_fn("%s", buf);
    }

    /* Reset color and newline */
    if (s_color_enable) {
        s_output_fn("%s\n", CORE_LOG_COLOR_RESET);
    } else {
        s_output_fn("\n");
    }
}
