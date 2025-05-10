cmake_minimum_required(VERSION 3.30)
project(Image_processing C)

set(CMAKE_C_STANDARD 11)

include_directories(.)

add_executable(Image_processing
    bmp8.c
    bmp8.h
    t_bmp24.h
    t_bmp24.c
    main.c
)
