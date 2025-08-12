cmake_minimum_required(VERSION 3.10)
project(Monitor )

# Use C++17 standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(Curses REQUIRED)
# Add source files and create an executable
add_executable(monitor
    main.cpp
    process.cpp
    colors.cpp
)

target_link_libraries(monitor PRIVATE ${CURSES_LIBRARIES})

