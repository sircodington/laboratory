cmake_minimum_required(VERSION 3.17)
project(lab LANGUAGES C CXX)
set(CMAKE_CXX_STANDARD 20)

add_compile_options(-Wno-literal-suffix)
add_compile_options(-Wswitch)

add_executable(${PROJECT_NAME}
    src/main.cpp
)

add_subdirectory(libraries)

target_link_libraries(${PROJECT_NAME}
    core
    sigil
)
