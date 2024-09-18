cmake_minimum_required(VERSION 3.20.0)

add_executable(wtlog_example example.cpp)

target_link_libraries(wtlog_example PRIVATE WtLog)