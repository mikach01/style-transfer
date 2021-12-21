cmake_minimum_required(VERSION 3.10)

project(sliced_wasserstein)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(wasserstein wasserstein.cpp)

