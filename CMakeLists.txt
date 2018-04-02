cmake_minimum_required (VERSION 2.6)
project (PCClient)
find_package( OpenCV REQUIRED )
add_definitions("-std=c++11")

find_package (Threads)

include_directories(include)

add_executable(smart src/oneSmart.cpp src/base64.cpp)
target_link_libraries (smart ${CMAKE_THREAD_LIBS_INIT} ${OpenCV_LIBS})
