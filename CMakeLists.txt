cmake_minimum_required(VERSION 3.14)
project(minesweeper)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES src/main.cpp src/Tile.h src/WelcomeScreen.cpp src/WelcomeScreen.h src/Tile.cpp src/TextureLoader.h src/TextureLoader.cpp src/Board.h src/Board.cpp src/LeaderboardScreen.h src/LeaderboardScreen.cpp)
add_executable(minesweeper ${SOURCE_FILES})
include_directories(/usr/local/include)

find_package(SFML 2.5 COMPONENTS system window graphics network audio REQUIRED)
include_directories(${SFML_INCLUDE_DIRS})
target_link_libraries(minesweeper sfml-system sfml-window sfml-graphics sfml-network sfml-audio)