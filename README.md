# External Roblox Dumper - Linux/Flatpak Port

## What Is Roblox Dumper?

Roblox Dumper is a tool that connects to a running Roblox game and automatically scans memory to find offsets for various classes and properties. It generates offset files in multiple formats.

**This is a Linux port** that targets the Sober Roblox client running on Linux, including Flatpak installations.

## Setup Guide
- Original: https://dumper.jonah.cool/setup-guide

## Linux-Specific Requirements

### Dependencies
- CMake 3.20+
- GCC/Clang with C++20 support
- OpenSSL development libraries
- libcurl development libraries
- Standard development tools (make, pkg-config, etc.)

### Ubuntu/Debian
```bash
sudo apt-get install cmake g++ libssl-dev libcurl4-openssl-dev pkg-config
```

### Arch Linux
```bash
sudo pacman -S cmake gcc openssl curl
```

### Fedora
```bash
sudo dnf install cmake gcc-c++ openssl-devel libcurl-devel
```

## Building

```bash
mkdir build
cd build
cmake ..
cmake --build . --config Release
```

## Running

The dumper attaches to the Sober Roblox process via ptrace. Depending on your system and Flatpak configuration, you may need elevated privileges:

```bash
# Without Flatpak (if Sober is installed natively)
./roblox-dumper

# With Flatpak
sudo ./roblox-dumper
```

**Important:** Make sure Roblox/Sober is already running and fully loaded before executing the dumper.

## Offset Files

Once completed, the dumper generates offset files in multiple formats:

- C++ Header: `offsets.h`
- Python: `offsets.py`
- C#: `offsets.cs`
- JSON: `offsets.json`

## Key Changes from Windows Version

### Process Attachment
- **Windows:** Uses `CreateToolhelp32Snapshot` and `NtOpenProcess` (Windows-specific APIs)
- **Linux:** Uses `ptrace(2)` syscall for process debugging/memory access

### Module Discovery
- **Windows:** Reads module base from Windows PE headers
- **Linux:** Reads `/proc/[pid]/maps` to locate executable memory regions

### Process Lookup
- **Windows:** Uses `CreateToolhelp32Snapshot` with `Process32FirstW`
- **Linux:** Scans `/proc` directory for process cmdline matching

### Memory Reading
- **Windows:** Uses `ReadProcessMemory`
- **Linux:** Uses `ptrace(PTRACE_PEEKDATA)` for word-sized memory reads

### Window Handling
- **Windows:** Uses Win32 API (`FindWindowA`, `GetClientRect`, etc.)
- **Linux:** Placeholder for X11/Wayland implementation

## Limitations

- Requires sufficient privileges to attach to processes (typically needs `sudo` or capabilities)
- Flatpak confinement may require additional setup or host permissions
- X11/Wayland window dimension detection not yet fully implemented

## Contact

Original author: Discord jonahw / 315412038455132160
Linux port: See repository for contributor information

## License

Same as original project
