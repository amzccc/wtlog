cmake_minimum_required(VERSION 3.20)

project(wtlog LANGUAGES CXX)

add_library(WtLog INTERFACE)

target_compile_features(WtLog INTERFACE cxx_std_20)

target_include_directories(WtLog INTERFACE
    $<BUILD_INTERFACE: ${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_PREFIX}/include>
)

set_target_properties(WtLog PROPERTIES OUTPUT_NAME "wtlog")

install(TARGETS WtLog
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

option(WTLOG_TEST_FLAG "ctest flag" OFF)
if(WTLOG_TEST_FLAG)
    add_subdirectory(test)
endif()

option(WTLOG_EXAMPLE_FLAG "install example demo" ON)
if(WTLOG_EXAMPLE_FLAG)
    install(DIRECTORY example 
        DESTINATION example
    )
endif()