cmake_minimum_required(VERSION 3.10)
project(GameClient)

# 设置C++标准
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# 添加可执行文件
add_executable(GameClient main.cpp)
