cmake_minimum_required(VERSION 3.20)

project(wtlog LANGUAGES CXX)

add_library(Log_Object OBJECT)

target_compile_features(Log_Object PUBLIC cxx_std_20)

target_include_directories(Log_Object PUBLIC
    $<BUILD_INTERFACE: ${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_PREFIX}/include>
)

add_subdirectory(src)

set_target_properties(Log_Object PROPERTIES 
    POSITION_INDEPENDENT_CODE 1
)

add_library(wtlog_static STATIC $<TARGET_OBJECTS:Log_Object>)

add_library(wtlog SHARED $<TARGET_OBJECTS:Log_Object>)

install(TARGETS wtlog_static wtlog 
    EXPORT wtlogConfig
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)

install(EXPORT wtlogConfig
    FILE 
    NAMESPACE wtlog::
    DESTINATION lib/cmake/wtlog
)

install(DIRECTORY ${CMAKE_SOURCE_DIR}/include
    DESTINATION include
)