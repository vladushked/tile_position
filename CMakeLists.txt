cmake_minimum_required(VERSION 2.8)

set (CMAKE_CXX_STANDARD 11)

project(tile_position)
find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )
set(SRC_LIST src/main.cpp src/TileAngle.cpp include/TileAngle.h src/TilePosition.cpp include/TilePosition.h)
add_executable(${PROJECT_NAME} ${SRC_LIST})
target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS})
