cmake_minimum_required(VERSION 3.20)

file(GLOB WTLOG_SRC "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

target_sources(wtlog_static PRIVATE ${WTLOG_SRC})