cmake_minimum_required(VERSION 3.29)
project(libtorch_httpservice)

# 设置CUDA
set(CMAKE_CUDA_COMPILER "/usr/local/cuda/bin/nvcc")
set(CMAKE_CUDA_ARCHITECTURES 80)

set(CMAKE_CXX_STANDARD 20)

# Libtorch
set(Torch_DIR /data/libtorch/share/cmake/Torch)
find_package(Torch REQUIRED)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${TORCH_CXX_FLAGS}")

# Opencv
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})
link_directories(${OpenCV_LIB_DIRS})

add_executable(libtorch_httpservice main.cpp
        Inference.cpp
        Inference.h
        server/handler.cpp
        server/handler.h
)
target_link_libraries(libtorch_httpservice "${TORCH_LIBRARIES}" "${OpenCV_LIBS}")
target_link_libraries(libtorch_httpservice hv)
