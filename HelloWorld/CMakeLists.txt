cmake_minimum_required(VERSION 2.8.4)
project(HelloWorld)

find_library(ALLEGRO_LIB NAMES allegro liballegro)

set(SOURCE_FILES helloworld.c)

add_executable(helloworld ${SOURCE_FILES})

target_link_executable(helloworld ${ALLEGRO_LIB})
