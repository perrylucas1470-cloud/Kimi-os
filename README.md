# Kimi-OS

A bare-bones operating system built from scratch, compiled automatically via GitHub Actions.

## Features

- 32-bit protected mode
- VGA text mode output
- GDT (Global Descriptor Table) setup
- IDT (Interrupt Descriptor Table) framework
- Multiboot compliant (works with GRUB)

## Building Locally

### Prerequisites

```bash
sudo apt-get install build-essential nasm qemu-system-x86 xorriso grub2-common
