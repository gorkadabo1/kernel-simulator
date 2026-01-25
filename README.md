# Kernel Simulator

A multi-threaded operating system kernel simulator written in ANSI C that implements core OS components including process scheduling, virtual memory management with paging, and address translation through MMU and TLB.

Developed as part of the Operating Systems course at EHU (Euskal Herriko Unibertsitatea) during the Computer Engineering degree.

## Overview

This project simulates a complete kernel environment capable of managing multiple processes across a configurable hardware architecture with multiple CPUs, cores, and hardware threads. The simulator loads and executes ELF (Executable and Linkable Format) programs while handling memory allocation, address translation, and process scheduling.

## Architecture

The simulator models a hierarchical hardware structure:

```
Machine
└── CPUs (configurable count)
    └── Cores (configurable per CPU)
        └── Hardware Threads (configurable per core)
            ├── PC (Program Counter)
            ├── IR (Instruction Register)
            ├── PTBR (Page Table Base Register)
            └── MMU
                └── TLB
```

## Features

### Process Management
- **Round-Robin Scheduler**: Fair distribution of processes across available hardware threads using a rotation strategy
- **Process Control Blocks (PCB)**: Complete process state management including memory mappings and execution context
- **ELF Loader**: Parses and loads executable files, extracting text and data segments into physical memory
- **Process Queue**: FIFO queue implementation for ready processes

### Memory Management
- **Virtual Memory**: Full virtual-to-physical address translation
- **Paging System**: Configurable page size with page tables stored in kernel space
- **Physical Memory Division**: 2/3 allocated to user space, 1/3 reserved for kernel (page tables)
- **Page Directory**: Centralized management of per-process page tables

### Address Translation
- **MMU (Memory Management Unit)**: Handles virtual-to-physical address translation using page tables
- **TLB (Translation Lookaside Buffer)**: Cache for recent address translations with circular replacement policy
- **Bit Masking**: Efficient extraction of page index and offset from virtual addresses

### Timing and Synchronization
- **System Clock**: Generates regular pulses simulating time progression
- **Timer**: Counts clock pulses and signals the scheduler at configurable intervals
- **Thread Synchronization**: Condition variables and signals for coordinating clock cycles with process execution

## Project Structure

```
kernel-simulator/
├── main.c                    # Entry point, system initialization
├── commons.h                 # Global definitions and constants
├── structs.h                 # Data structure definitions
├── clock_timer.c/.h          # Clock and timer implementation
├── machine.c/.h              # Hardware architecture simulation
├── physical_memory.c/.h      # Physical memory management
├── process.c/.h              # Process loading and PCB management
├── page_table.c/.h           # Page table operations
├── scheduler_dispatcher.c/.h # Round-robin scheduler
├── mmu.c/.h                  # Memory Management Unit
├── tlb.c/.h                  # Translation Lookaside Buffer
├── utilities.c/.h            # Helper functions
├── CMakeLists.txt            # Build configuration
└── elf/                      # Directory for ELF executables
    └── prog*.elf             # Sample programs
```

## Technical Implementation Details

### Virtual-to-Physical Address Translation

The translation process uses bit manipulation to extract page indices and offsets:

1. **Page Index**: Obtained by right-shifting the virtual address by the offset bit count
2. **Page Offset**: Extracted using a bitmask based on page size
3. **Physical Address**: Page table lookup + offset addition

```
Virtual Address: [Page Index | Page Offset]
                      ↓
              Page Table Lookup
                      ↓
Physical Address: [Frame Number | Page Offset]
```

### Scheduler Operation

The round-robin scheduler operates on timer signals:

1. Timer counts clock pulses until reaching the configured interval
2. On interval completion, scheduler searches for an available hardware thread
3. Process is dequeued and assigned to the thread
4. Execution launches in a separate pthread
5. Rotation ensures fair thread utilization

### Memory Layout

```
Physical Memory
┌─────────────────────────┐
│                         │
│     User Space          │  ← 2/3 of total memory
│   (Code + Data)         │    Programs loaded here
│                         │
├─────────────────────────┤
│                         │
│    Kernel Space         │  ← 1/3 of total memory
│   (Page Tables)         │    Page directory and tables
│                         │
└─────────────────────────┘
```

## Building and Running

### Prerequisites
- GCC or compatible C compiler
- CMake 3.x or higher
- POSIX-compliant system (Linux recommended)
- pthread library

### Compilation

```bash
mkdir build && cd build
cmake ..
make
```

### Execution

```bash
./kernel <interval> <quantum> <num_cpus> <cores_per_cpu> <threads_per_core>
```

Place ELF files in the `elf/` directory before running. The simulator will automatically detect and load all `.elf` files.

### Example

```bash
./kernel 100 50 2 2 2
```
This configures: 100 clock cycles per scheduler interval, quantum of 50, 2 CPUs with 2 cores each, and 2 threads per core (8 total hardware threads).

## Key Learning Outcomes

This project provided deep understanding of:

- **Pointer Arithmetic in C**: Managing memory addresses and understanding how pointer operations account for data type sizes
- **64-bit vs 32-bit Addressing**: Debugging issues arising from incorrect pointer sizes on 64-bit systems
- **Virtual Memory Concepts**: Practical implementation of page tables, TLB, and address translation
- **Concurrent Programming**: Thread synchronization using mutexes, condition variables, and signals
- **ELF File Format**: Parsing executable files and loading segments into memory
- **Debugging with GDB**: Extensive use of debugger to trace memory issues and segmentation faults

## Technologies

- **Language**: ANSI C
- **Build System**: CMake
- **Threading**: POSIX Threads (pthreads)
- **Development Environment**: CLion, GCC, GDB
- **Target Platform**: Linux/UNIX

## Course Context

This simulator was developed for the Operating Systems course, covering:
- Part 1: System Architecture (thread management, clock simulation)
- Part 2: Process Scheduler (round-robin implementation)
- Part 3: Memory Manager (virtual memory, paging, MMU/TLB)

## License

This project was developed for educational purposes as part of university coursework.

## Author

Gorka Dabo - Computer Engineering, EHU
