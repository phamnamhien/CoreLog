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

| Level   | Macro    | Color      | Description                        |
|---------|----------|------------|------------------------------------|
| ERROR   | `LOG_E`  | Red        | Critical errors, system cannot continue normally |
| WARN    | `LOG_W`  | Yellow     | Warnings, something unexpected but recoverable  |
| INFO    | `LOG_I`  | Green      | General informational, key events                |
| DEBUG   | `LOG_D`  | *(no color)* | Debug details for development                  |
| VERBOSE | `LOG_V`  | *(no color)* | Verbose trace, very detailed output            |

## Quick Start

### 1. Add files to your project

Copy `core_log.h` and `core_log.c` into your source tree.

### 2. Initialize

```c
#include "core_log.h"

// Step 1 (required): Set output function — any printf-like function
core_log_init(printf);

// Step 2 (optional): Set timestamp function
core_log_set_timestamp(HAL_GetTick);  // STM32 example

// Step 3 (optional): Enable ANSI colors (only if terminal supports it)
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
E (1234) MAIN: Sensor read failed: err=-1       ← Red
W (1235) MAIN: Temperature high: 42 C           ← Yellow
I (1236) MAIN: System initialized               ← Green
D (1237) MAIN: Buffer allocated: 512 bytes       ← No color
V (1238) MAIN: Entering main loop                ← No color
```

**Without timestamp, no color:**
```
E MAIN: Sensor read failed: err=-1
W MAIN: Temperature high: 42 C
I MAIN: System initialized
```

## When to Use Each Log Level

### `LOG_E` — Error
System failures that need immediate attention. The program may not work correctly after this.
```c
LOG_E(TAG, "Flash write failed at addr 0x%08X", addr);
LOG_E(TAG, "I2C timeout on bus %d", bus_id);
LOG_E(TAG, "Out of memory: requested %u bytes", size);
```

### `LOG_W` — Warning
Something unexpected happened, but the system can continue. Often indicates a condition that might become an error.
```c
LOG_W(TAG, "Retry %d/%d: sensor not responding", attempt, max_retries);
LOG_W(TAG, "Battery low: %d%%", battery_pct);
LOG_W(TAG, "Queue almost full: %d/%d", count, capacity);
```

### `LOG_I` — Info
Key events and state changes. Useful for understanding program flow in production.
```c
LOG_I(TAG, "System initialized, firmware v%s", FW_VERSION);
LOG_I(TAG, "WiFi connected: %s", ssid);
LOG_I(TAG, "OTA update completed, rebooting...");
```

### `LOG_D` — Debug
Detailed information useful during development. Typically disabled in release builds.
```c
LOG_D(TAG, "Received %d bytes from UART", len);
LOG_D(TAG, "Task stack remaining: %u bytes", stack_free);
LOG_D(TAG, "ADC raw=%d, voltage=%dmV", raw, mv);
```

### `LOG_V` — Verbose
Very detailed trace output. High frequency, used for deep debugging of specific issues.
```c
LOG_V(TAG, "SPI TX: [%02X %02X %02X %02X]", buf[0], buf[1], buf[2], buf[3]);
LOG_V(TAG, "State machine: %s -> %s", state_names[old], state_names[new]);
LOG_V(TAG, "Timer ISR fired, counter=%lu", counter);
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
// 1. Init hardware first (application responsibility)
HAL_UART_Init(&huart1);

// 2. Then init CoreLog
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

void setup() {
    // 1. Init hardware first
    Serial.begin(115200);

    // 2. Then init CoreLog
    core_log_init(my_printf);
    core_log_set_timestamp(millis);
}
```

**SEGGER RTT:**
```c
// 1. Init RTT first
SEGGER_RTT_Init();

// 2. Then init CoreLog
core_log_init(SEGGER_RTT_printf);  // or a wrapper
```

## License

MIT
