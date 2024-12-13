cmake_minimum_required(VERSION 3.20.0)

find_package(wtlog REQUIRED)

add_executable(wtlog_example example.cpp)

target_link_libraries(wtlog_example PRIVATE wtlog::wtlog)

target_include_directories(wtlog_example PRIVATE
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)