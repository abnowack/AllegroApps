project(HelloWorld)

set(SOURCE_FILES helloworld.c)

add_executable(hello_world ${SOURCE_FILES})

target_link_libraries(hello_world ${ALLEGRO_LIB})
