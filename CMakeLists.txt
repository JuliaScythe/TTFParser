cmake_minimum_required(VERSION 3.15)
project(NEA)
#list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR})
set(CMAKE_CXX_STANDARD 17)

find_package(SDL2 REQUIRED)
#find_package(SDL2_gfx REQUIRED)

include_directories(${SDL2_INCLUDE_DIRS})

add_executable(NEA SDL_gfx/SDL2_gfxPrimitives.c SDL_gfx/SDL2_gfxPrimitives.h src/main.cpp src/Font.cpp include/Font.h src/Header.cpp include/Header.h src/util.cpp include/util.h src/TableHeader.cpp include/TableHeader.h src/HEADTable.cpp include/HEADTable.h src/CMAPTable.cpp include/CMAPTable.h src/Glyph.cpp include/Glyph.h include/PointFlag.cpp include/PointFlag.h src/Point.cpp include/Point.h  SDL_gfx/SDL2_rotozoom.c SDL_gfx/SDL2_rotozoom.h)

target_link_libraries(NEA ${SDL2_LIBRARY})