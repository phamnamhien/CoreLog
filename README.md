# CoreLog

CoreLog is a lightweight, flexible logging library for microcontrollers, supporting any output interface (UART, Ethernet, SWD, RTT, etc.) via user-defined function pointers.

Inspired by **ESP-IDF ESP_LOG** — but designed to work on **any microcontroller** platform.

## Features

- **Platform-independent** — works on any MCU (STM32, nRF, AVR, PIC, RISC-V, etc.)
- **Function-pointer based init** — plug in your own output function (`printf`, UART write, RTT, etc.)
- **Optional timestamp** — provide a timestamp function or leave it out; logs adapt automatically
- **ANSI color support** — colored output when your terminal supports it
- **5 log levels** — Error, Warning, Info, Debug, Verbose
- **Compile-time filtering** — strip out unwanted log levels at build time for zero overhead
- **Runtime filtering** — change log verbosity on the fly
- **Minimal footprint** — just 2 files (`core_log.h` + `core_log.c`)

## Log Levels

| Level   | Macro    | Color   | Description           |
|---------|----------|---------|-----------------------|
| ERROR   | `LOG_E`  | Red     | Critical errors       |
| WARN    | `LOG_W`  | Yellow  | Warnings              |
| INFO    | `LOG_I`  | Green   | Informational         |
| DEBUG   | `LOG_D`  | Cyan    | Debug details         |
| VERBOSE | `LOG_V`  | Magenta | Verbose/trace output  |

## Quick Start

### 1. Add files to your project

Copy `core_log.h` and `core_log.c` into your source tree.

### 2. Initialize

```c
#include "core_log.h"

// Initialize with your output function (printf-like)
core_log_init(printf);

// Optional: provide a timestamp function
core_log_set_timestamp(HAL_GetTick);  // STM32 example

// Optional: enable ANSI colors
core_log_set_color(1);
```

### 3. Use logging macros

```c
#define TAG "MAIN"

LOG_E(TAG, "Sensor read failed: err=%d", err);
LOG_W(TAG, "Temperature high: %d C", temp);
LOG_I(TAG, "System initialized");
LOG_D(TAG, "Buffer allocated: %u bytes", size);
LOG_V(TAG, "Entering main loop");
```

### Output examples

**With timestamp + color enabled:**
```
E (1234) MAIN: Sensor read failed: err=-1
W (1235) MAIN: Temperature high: 42 C
I (1236) MAIN: System initialized
```

**Without timestamp, no color:**
```
E MAIN: Sensor read failed: err=-1
W MAIN: Temperature high: 42 C
I MAIN: System initialized
```

## API Reference

| Function                    | Description                                      |
|-----------------------------|--------------------------------------------------|
| `core_log_init(fn)`         | Set the output function (required)               |
| `core_log_set_timestamp(fn)`| Set timestamp provider (optional, NULL to disable)|
| `core_log_set_color(en)`    | Enable (1) or disable (0) ANSI colors            |
| `core_log_set_level(level)` | Set runtime log level filter                     |
| `core_log_get_level()`      | Get current log level                            |

## Compile-time Configuration

| Define                  | Default                | Description                          |
|-------------------------|------------------------|--------------------------------------|
| `CORE_LOG_LEVEL_MAX`    | `CORE_LOG_VERBOSE` (5) | Max level compiled in (strips higher)|
| `CORE_LOG_MAX_MSG_LEN`  | `256`                  | Max formatted message buffer size    |

Example: build with only errors and warnings:
```
-DCORE_LOG_LEVEL_MAX=CORE_LOG_WARN
```

## Platform Examples

**STM32 (HAL + UART printf redirect):**
```c
core_log_init(printf);
core_log_set_timestamp(HAL_GetTick);
```

**Arduino:**
```c
int my_printf(const char *fmt, ...) {
    char buf[128];
    va_list args;
    va_start(args, fmt);
    int len = vsnprintf(buf, sizeof(buf), fmt, args);
    va_end(args);
    Serial.print(buf);
    return len;
}

core_log_init(my_printf);
core_log_set_timestamp(millis);
```

**SEGGER RTT:**
```c
core_log_init(SEGGER_RTT_printf);  // or a wrapper
```

## License

MIT
