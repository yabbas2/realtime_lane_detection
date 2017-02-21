cmake_minimum_required(VERSION 3.6)
project(RTLD_C++)

set(CMAKE_CXX_STANDARD 11)

find_package(OpenCV REQUIRED core imgproc video videoio videostab highgui ml features2d calib3d flann)

set(SOURCE_FILES src/main.cpp src/ipm.cpp src/processing.cpp src/stream.cpp src/lsd.cpp src/accuracy.cpp)
set(HEADER_FILES include/processing.hpp)

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/include)

set(GCC_THREAD_LINKER_FLAGS "-lpthread -fopenmp")
set(GCC_COMPILER_FLAGS "-m64 -g -Wall -fopenmp")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GCC_COMPILER_FLAGS}")

add_executable(RTLD_C++ ${SOURCE_FILES} ${HEADER_FILES})

target_link_libraries(RTLD_C++ ${OpenCV_LIBS} ${GCC_THREAD_LINKER_FLAGS})