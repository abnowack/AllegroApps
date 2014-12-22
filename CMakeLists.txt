cmake_minimum_required(VERSION 2.8.4)
project(AllegroApps)

find_library(ALLEGRO_LIB NAMES allegro liballegro)

set(SOURCE_FILES HelloWorld/helloworld.c)

add_executable(hello_world ${SOURCE_FILES})

target_link_libraries(hello_world ${ALLEGRO_LIB})
