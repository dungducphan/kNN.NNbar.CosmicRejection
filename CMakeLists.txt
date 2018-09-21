cmake_minimum_required(VERSION 3.12)
project(nova_nnbar_kNN)

set(CMAKE_CXX_STANDARD 17)

find_package(ROOT REQUIRED)

include(${ROOT_USE_FILE})

add_executable(nova_nnbar_kNN main.cpp VpTree.h AccessTree.cpp AccessTree.h)
target_link_libraries(nova_nnbar_kNN ${ROOT_LIBRARIES} Core Tree Hist)