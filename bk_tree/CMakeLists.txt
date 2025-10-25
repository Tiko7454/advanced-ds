cmake_minimum_required(VERSION 3.16)
project(BKTree LANGUAGES CXX)

# Use C++20
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Enable debug info by default
set(CMAKE_BUILD_TYPE Debug)

# Add the executable
add_executable(main
    main.cxx
    bk_tree.cxx
)

# Include headers from current directory
target_include_directories(main PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

# Custom clean target (optional, since `make clean` works automatically in CMake builds)
add_custom_target(extra_clean
    COMMAND ${CMAKE_COMMAND} -E rm -f main *.o
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)

