# 🔍 Basic Process Monitor in C++

A simple real-time process monitor written in C++ that shows running processes, CPU usage, and memory consumption — like a simplified version of `top`.

## 🖥️ Features

- Lists top 10 running processes
- Shows PID, command name, CPU%, and memory usage
- Updates every second
- Lightweight and dependency-free (uses only standard libraries and `/proc`)

## 🧰 Requirements

- Linux OS (this uses `/proc` filesystem)
- g++ compiler
- Make sure `/proc` is mounted (standard on all desktop/server Linux systems)

## 📦 Files

| File | Description |
|------|-------------|
| `process_monitor.cpp` | Main source file |
| `README.md` | This file |

## 🛠️ How to Compile

```bash
g++ process.cpp main.cpp -o monitor
```
## How to Run
```bash
./monitor
```
