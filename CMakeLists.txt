cmake_minimum_required(VERSION 3.29)

set(CMAKE_CXX_COMPILER clang++)
project(app LANGUAGES CXX)

add_subdirectory("vendor/SDL3")
add_subdirectory("vendor/glad")
add_subdirectory("vendor/glm")

set(CMAKE_CXX_STANDARD 23)
file(GLOB_RECURSE MY_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
file(GLOB_RECURSE MY_HEADERS CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/include/*.h")

add_executable(${CMAKE_PROJECT_NAME} "${MY_SOURCES}" "${MY_HEADERS}")
target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include/")

target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE SDL3::SDL3 glad glm)
target_compile_definitions(${CMAKE_PROJECT_NAME} PRIVATE OPEGL)

target_compile_options(${CMAKE_PROJECT_NAME} PRIVATE -Wall -Wextra -g)
