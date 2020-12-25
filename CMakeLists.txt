cmake_minimum_required(VERSION 3.15)
project(battleships)

set(CMAKE_CXX_STANDARD 17)

add_executable(battleships main.cpp Game.h Game.cpp ConsoleGamer.cpp RandomGamer.cpp OptimalGamer.cpp optionparser.h)
#add_executable(battleships main.cpp Game.h Game.cpp optionparser.h)