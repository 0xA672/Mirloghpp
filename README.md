# Mirloghpp
A lightweight single-header logger for small C++ projects.
[![C++11](https://img.shields.io/badge/C%2B%2B-11-blue)]
```cpp
#include "mirror.h"
```
Example:
```cpp
#include "mirror.h"

int main() {
    Mirror_Fast();  // optional: speed up I/O

    mirror::mirprint(mirror::Lv::Info,  "Server started",    mirror::Mode::coloredstr);
    mirror::mirprint(mirror::Lv::Warn,  "High memory usage", mirror::Mode::coloredstr);
    mirror::mirprint(mirror::Lv::Error, "Connection lost",   mirror::Mode::coloredstr);

    // Custom color
    mirror::mirprint(mirror::Lv::Info, "Special message", 
                     mirror::Mode::coloredstr, mirror::Colors::blue);

    return 0;
}
```
Output:
```rust
2025-04-11 12:34:56 UTC [INFO] Server started      
2025-04-11 12:34:56 UTC [WARN] High memory usage   
2025-04-11 12:34:56 UTC [ERROR] Connection lost    
2025-04-11 12:34:56 UTC [INFO] Special message     
```

On Windows, the library automatically enables virtual terminal processing to support ANSI escape codes in the console.

## API Reference
### I/O Optimization Macro

#### `Mirror_Fast()`
Disables synchronization between C++ and C standard streams, and unties `std::cin` from `std::cout`.  
Call once at the beginning of `main()` to speed up console I/O.

