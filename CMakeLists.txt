cmake_minimum_required(VERSION 3.15)
project(NEA)

set(CMAKE_CXX_STANDARD 17)

add_executable(NEA src/main.cpp src/Font.cpp include/Font.h src/Header.cpp include/Header.h src/util.cpp include/util.h src/TableHeader.cpp include/TableHeader.h src/HEADTable.cpp include/HEADTable.h src/CMAPTable.cpp include/CMAPTable.h)