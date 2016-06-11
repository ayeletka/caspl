cmake_minimum_required(VERSION 3.5)
project(lab8)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
    task0/4.txt
    task0/abc.txt
    task0/main
    task0/main.c
    task0/main.o
    task0/makefile
    task0/ntsc
    task1/4.txt
    task1/a.out
    task1/abc.txt
    task1/main
    task1/main.c
    task1/main.o
    task1/makefile
    task1/ntsc
    task2/4.txt
    task2/a.out
    task2/abc.txt
    task2/main
    task2/main.c
    task2/main.o
    task2/makefile
    task2/ntsc)

add_executable(lab8 ${SOURCE_FILES})