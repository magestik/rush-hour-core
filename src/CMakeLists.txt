cmake_minimum_required(VERSION 2.8.12)

add_library(RushHourCore STATIC Block.cpp Block.h Board.cpp Board.h)

target_include_directories(RushHourCore PUBLIC $<BUILD_INTERFACE:${CONTAINERS_INCLUDE_DIR}> $<INSTALL_INTERFACE:include/containers>)

set_property(TARGET RushHourCore PROPERTY CXX_STANDARD 11)
