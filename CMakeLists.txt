cmake_minimum_required(VERSION 3.17)
project(Dames)

set(CMAKE_CXX_STANDARD 20)

include_directories("C:/SFML/include" "${PROJECT_SOURCE_DIR}/include")
link_directories("C:/SFML/lib")

#add_executable(Dames src/main.cpp)
add_executable(Dames src/main.cpp include/Board.hpp src/Board.cpp src/Piece.cpp include/Piece.hpp src/RingShape.cpp include/RingShape.hpp include/Params.hpp src/Params.cpp)

target_link_libraries(Dames sfml-graphics-d sfml-window-d sfml-system-d)