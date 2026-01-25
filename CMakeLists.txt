cmake_minimum_required(VERSION 3.26)
project(untitled C)

set(CMAKE_C_STANDARD 90)

add_executable(untitled main.c
        structs.h
        clock_timer.c
        clock_timer.h
        commons.h
        process.c
        process.h
        scheduler_dispatcher.c
        scheduler_dispatcher.h
        utilities.c
        utilities.h
        machine.c
        machine.h
        structs.h
        physical_memory.c
        physical_memory.h
        tlb.c
        tlb.h
        mmu.c
        mmu.h
        page_table.c
        page_table.h
)

