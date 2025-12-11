cmake_minimum_required(VERSION 3.10)

# Project name
project(EffectiveCppDemo VERSION 1.0)

# Set C++ standard (C++17 pentru smart pointers si threading)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add compile options for better warnings
if(MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# Include directories
include_directories(${PROJECT_SOURCE_DIR}/include)

# Source files
set(SOURCES
    src/main.cpp
    src/Person.cpp
    src/Employee.cpp
    src/Widget.cpp
)

# Create executable
add_executable(${PROJECT_NAME} ${SOURCES})

# Link threading library (required for std::thread, std::mutex)
find_package(Threads REQUIRED)
target_link_libraries(${PROJECT_NAME} Threads::Threads)

# Print some information
message(STATUS "Project: ${PROJECT_NAME}")
message(STATUS "C++ Standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")


