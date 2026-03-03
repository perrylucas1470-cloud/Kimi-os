# Kimi-OS v0.2

A 32-bit operating system built from scratch with keyboard input, timer interrupts, and an interactive shell. Built automatically via GitHub Actions.

![Version](https://img.shields.io/badge/version-0.2.0-blue)
![Build](https://img.shields.io/badge/build-automated-green)

## Features

- ✅ **32-bit Protected Mode** - Full protected mode operation
- ✅ **VGA Text Mode** - 80x25 color display with scrolling
- ✅ **Interrupt Handling** - Full IDT/ISR setup with PIC remapping
- ✅ **Keyboard Driver** - US QWERTY layout with shift/caps support
- ✅ **Timer (PIT)** - 100Hz system timer for delays and uptime
- ✅ **Interactive Shell** - Command-line interface with multiple commands
- ✅ **Memory Management** - Basic physical memory tracking
- ✅ **Cross-Compiler** - Automated i686-elf toolchain build

## Shell Commands

| Command | Description |
|---------|-------------|
| `help` | Show available commands |
| `clear` | Clear the screen |
| `time` | Show system uptime |
| `reboot` | Reboot the system |
| `echo &lt;text&gt;` | Print text to screen |
| `color &lt;0-15&gt;` | Change text color |
| `about` | Show system information |
| `mem` | Show memory statistics |

## Building Locally

### Prerequisites

```bash
sudo apt-get update
sudo apt-get install build-essential nasm qemu-system-x86 xorriso grub2-common
