/**
 * @file main.c
 * @brief Example usage of CoreLog library.
 *
 * This example demonstrates how to use CoreLog on a generic platform.
 * Replace printf / millis with your platform-specific implementations.
 */

#include <stdio.h>
#include <stdint.h>
#include "core_log.h"

/* --------------------------------------------------------------------------
 * Platform-specific function examples
 * Replace these with your actual UART/SWD/RTT output and timer functions.
 * -------------------------------------------------------------------------- */

/**
 * @brief Example timestamp function.
 *        On a real MCU this could return HAL_GetTick(), xTaskGetTickCount(), etc.
 */
static uint32_t my_timestamp(void)
{
    /* Placeholder: return 0 or a real tick counter */
    static uint32_t fake_tick = 0;
    return fake_tick++;
}

/* --------------------------------------------------------------------------
 * Application
 * -------------------------------------------------------------------------- */

#define TAG "MAIN"

int main(void)
{
    /*
     * Step 1: Initialize CoreLog with an output function.
     *         printf works for desktop / semihosting / UART-redirected stdout.
     *         For bare-metal UART you can pass your own printf-like wrapper.
     */
    core_log_init(printf);

    /*
     * Step 2 (optional): Provide a timestamp function.
     *         If you skip this step, logs will print without a timestamp.
     */
    core_log_set_timestamp(my_timestamp);

    /*
     * Step 3 (optional): Enable ANSI colors.
     *         Only enable if your terminal/console supports ANSI escape codes.
     */
    core_log_set_color(1);

    /*
     * Step 4 (optional): Set runtime log level filter.
     *         Default is CORE_LOG_VERBOSE (show everything).
     */
    core_log_set_level(CORE_LOG_VERBOSE);

    /* ----- Now use the logging macros ----- */

    LOG_E(TAG, "This is an error message: code=%d", -1);
    LOG_W(TAG, "This is a warning: value=%d exceeds limit", 100);
    LOG_I(TAG, "System initialized successfully");
    LOG_D(TAG, "Debug info: buffer_size=%u", 512u);
    LOG_V(TAG, "Verbose trace: entering main loop");

    /* Example: change level at runtime to suppress debug & verbose */
    core_log_set_level(CORE_LOG_INFO);
    LOG_D(TAG, "This debug message will NOT appear");
    LOG_V(TAG, "This verbose message will NOT appear");
    LOG_I(TAG, "This info message WILL appear");

    /* Example: logging without timestamp */
    core_log_set_timestamp(NULL);
    LOG_W(TAG, "Warning without timestamp");

    /* Example: logging without color */
    core_log_set_color(0);
    LOG_E(TAG, "Error without color and without timestamp");

    return 0;
}
