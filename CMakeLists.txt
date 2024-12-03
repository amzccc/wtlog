cmake_minimum_required(VERSION 3.20)

project(Wtlog LANGUAGES CXX)

add_library(wtlog_interface INTERFACE)

target_compile_features(wtlog_interface INTERFACE cxx_std_20)

set(gcc_like_cxx "$<COMPILE_LANG_AND_ID: CXX,ARMClang,AppleClang,Clang,GNU,LCC>")
set(msvc_like "$<COMPILE_LANG_AND_ID:CXX,MSVC>")
target_compile_options(wtlog_interface INTERFACE
  "$<${gcc_like_cxx}:$<BUILD_INTERFACE:-Wall;-Wextra;-Wshadow;-Wformat=2;-Wunused>>"
  "$<${msvc_like}:$<BUILD_INTERFACE:-W3>>"
)

add_library(wtlog_static STATIC)

message(STATUS "compiler id: ${CMAKE_CXX_COMPILER_ID}")

target_include_directories(wtlog_static PUBLIC 
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

add_subdirectory(src)

target_link_libraries(wtlog_static PUBLIC wtlog_interface)

set_target_properties(wtlog_static PROPERTIES OUTPUT_NAME "wtlog")

install(TARGETS wtlog_interface wtlog_static
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

install(DIRECTORY ${CMAKE_SOURCE_DIR}/include/
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