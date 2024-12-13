cmake_minimum_required(VERSION 3.20.0)


find_package(Catch2 QUIET)

if(NOT Catch2_FOUND)
	include(FetchContent)
	FetchContent_Declare(
		Catch2
		GIT_REPOSITORY https://github.com/catchorg/Catch2.git
		GIT_TAG v3.7.1
	)
	FetchContent_MakeAvailable(Catch2)
endif()

file(GLOB test_src "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

add_executable(wtlog_test)

target_sources(wtlog_test PRIVATE ${test_src})

target_link_libraries(wtlog_test PRIVATE 
	Catch2::Catch2
	wtlog
)

target_include_directories(wtlog_test PRIVATE
	$<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
	$<INSTALL_INTERFACE:include>
)

enable_testing()

function(do_test Target Exe Args)
	add_test(NAME ${Target} COMMAND ${Exe} ${Args})
endfunction(do_test Target Exe Args)

do_test("wtlog_test" "wtlog_test" "--report=verbose")


