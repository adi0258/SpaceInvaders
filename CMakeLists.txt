cmake_minimum_required(VERSION 3.31)
project(bagel26)

set(CMAKE_CXX_STANDARD 20)

add_executable(bagel26
        main.cpp
        bagel.h
        tests.cpp
        SpaceInvaders.cpp
        SpaceInvaders.h)

set(SDL_STATIC ON)
set(SDL_SHARED OFF)
add_subdirectory(lib/SDL)
target_link_libraries(bagel26 PUBLIC SDL3-static)

set(BUILD_SHARED_LIBS OFF)
add_subdirectory(lib/SDL_image)
target_link_libraries(bagel26 PUBLIC SDL3_image-static)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different
        "${PROJECT_SOURCE_DIR}/res"
        "$<TARGET_FILE_DIR:${PROJECT_NAME}>/res")