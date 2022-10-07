cmake_minimum_required(VERSION 3.8)

project(lab_03)

add_executable(task1
        task1.c)

add_executable(task2
        task2.c)

add_executable(task3
        task3.c)

FILE(GLOB StackSrc2 stack/*.c stack/*.h)
add_executable(stack
        ${StackSrc2}
        )

FILE(GLOB QueueSrc queue/*.c queue/*.h)
add_executable(queue
        ${QueueSrc}
        )

add_executable(task4
        task4.c)

add_executable(task5
        task5.c)