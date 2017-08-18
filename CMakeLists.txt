project(rl)

cmake_minimum_required(VERSION 2.8)
find_package(Eigen3 REQUIRED)
set(CMAKE_CXX_FLAGS "-std=c++11 -O3")
include_directories(${EIGEN3_INCLUDE_DIR})

file(GLOB RL_SRC "src/test.cpp")
file(GLOB CONNECT_FOUR_RL_SRC "src/debug.cpp")

add_executable(tictactoe 
  ${RL_SRC}
)

add_executable(connectfour
  ${CONNECT_FOUR_RL_SRC}
)
