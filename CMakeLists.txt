 
cmake_minimum_required(VERSION 2.8)

# set the project name
project(VectorizeSSE CXX)

add_compile_options(-std=c++11)

find_package(OpenMP REQUIRED)

# add the executable
add_executable(VectorizeSSE main.cc test_rig.cc)

target_link_libraries(VectorizeSSE PRIVATE "${OpenMP_CXX_FLAGS}")
target_compile_options(VectorizeSSE PRIVATE "${OpenMP_CXX_FLAGS}")