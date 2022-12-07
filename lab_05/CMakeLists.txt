cmake_minimum_required(VERSION 3.8)

project(lab_05)

add_executable(pc
        pc/main.c
        pc/pc.c
        pc/pc.h)

add_executable(rw
        rw/main.c
        rw/rw.c
        rw/rw.h)