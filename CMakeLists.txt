cmake_minimum_required(VERSION 3.10)

project(recorder)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenCV REQUIRED)

add_executable(ScreenRecorder main.cpp)

target_link_libraries(ScreenRecorder PRIVATE 
    ${OpenCV_LIBS} 
    user32.lib 
    gdi32.lib
)

set_target_properties(ScreenRecorder PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/Release")
