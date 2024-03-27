cmake_minimum_required(VERSION 3.27)
project(projet C)

set(CMAKE_C_STANDARD 11)

add_executable(projet
        main.c
        include/structures.h
        include/library.h
        include/partition.h
        src/library.c
        src/partition.c
)
