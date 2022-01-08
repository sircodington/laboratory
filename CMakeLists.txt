cmake_minimum_required(VERSION 3.17)
project(lab LANGUAGES C CXX)
set(CMAKE_CXX_STANDARD 20)

add_executable(${PROJECT_NAME}
    src/main.cpp
)

add_subdirectory(libraries)

target_link_libraries(${PROJECT_NAME}
    core
    sigil
)
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:core> $<TARGET_FILE_DIR:${PROJECT_NAME}>
)
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:sigil> $<TARGET_FILE_DIR:${PROJECT_NAME}>
)
