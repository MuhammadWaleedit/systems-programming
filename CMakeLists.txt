cmake_minimum_required(VERSION 3.16)
project(FinalPoints CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

add_executable(final_points
    src/main.cpp
    src/Person.cpp
    src/Grades.cpp
    src/Input.cpp
    src/FileIO.cpp
    src/Processing.cpp
)
target_include_directories(final_points PRIVATE include)

if(MSVC)
    target_compile_options(final_points PRIVATE /W4)
else()
    target_compile_options(final_points PRIVATE -Wall -Wextra)
endif()

# MinGW: link the C++ runtime statically so a different libstdc++ DLL
# on PATH is not picked up at run time.
if(MINGW)
    target_link_options(final_points PRIVATE -static)
endif()
