cmake_minimum_required(VERSION 3.6)
project(ArchiveAlgorithm_HW)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_CXX_COMPILER /usr/local/Cellar/gcc/6.2.0/bin/g++-6)

set(SOURCE_FILES main.cpp Huffman.cpp Huffman.h ShannonFano.cpp ShannonFano.h BitIO.cpp BitIO.h Utils.cpp Utils.h)
add_executable(ArchiveAlgorithm_HW ${SOURCE_FILES})