cmake_minimum_required(VERSION 2.8.11)

FILE(GLOB_RECURSE src *.cpp *.h)

include_directories(../include)

add_library(RushHourCore STATIC ${src})
