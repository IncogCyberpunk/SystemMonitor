# 🚀 Basic Process Monitor in C++

A lightweight **real-time system monitor** written in C++ — think of it as a minimal, dependency-free `top` clone.  
It uses the **Linux `/proc` filesystem** to display CPU usage, memory stats, system uptime, load averages, and the top running processes.

---

## ✨ Features

✅ **System Information**
- 🖥 **Total CPU Usage**
- 💾 **Memory Usage**
- ⏳ **System Uptime**
- 🔢 **Recent Process ID**
- 📊 **Processes Since Boot**
- ⚡ **Active Running Processes**
- 📈 **Load Averages** (1, 5, 15 minutes)

✅ **Process Table**
- Lists **Top 25** running processes
- Shows:
  - **PID**
  - **Command Name**
  - **CPU %**
  - **Memory Usage**

✅ **Other**
- Refreshes **every second**
- **Dependency-free** (only standard C++ + `/proc`)

---

## 📦 Project Structure

| File              | Description |
|-------------------|-------------|
| `main.cpp`        | Entry point for the application |
| `process.cpp`     | Retrieves and processes running process info |
| `process.hpp`     | Function & struct definitions for process handling |
| `colors.cpp`      | Functions for colored terminal output |
| `colors.hpp`      | Header file for terminal color handling |
| `CMakeLists.txt`  | Build configuration |
| `README.md`       | This file |
| `LICENSE`         | License (GNU GPL v3) |

---

## 🛠 Requirements

- **Linux Kernel** (uses `/proc`)
- `g++` compiler 
- `cmake` for building the project(minimum version 3.10)
- `make` for compilation
- `/proc` filesystem mounted (default on all Linux distros)
- **Terminal** with ANSI color support (for colored output)
- **C++17** support (for modern C++ features)

---

## ⚙️ Build Instructions

```bash

# 1️⃣ Create and enter build directory
mkdir build && cd build

# 2️⃣ Generate Makefiles using CMake
cmake ..

# 3️⃣ Compile the monitor
make
``````

## Running the monitor
```bash

# Run the compiled monitor executable
./monitor
```
