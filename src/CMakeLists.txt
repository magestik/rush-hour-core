cmake_minimum_required(VERSION 2.8.11)

FILE(GLOB_RECURSE src *.cpp *.h)

add_library(RushHourCore STATIC ${src})
