cmake_minimum_required(VERSION 2.8)
project(video_to_pic)

set(CMAKE_CXX_STANDARD 14)
set(OpenCV_DIR "/usr/local/share/OpenCV")

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(video_to_pic main.cpp)
target_link_libraries(video_to_pic ${OpenCV_LIBS} MVSDK pthread)