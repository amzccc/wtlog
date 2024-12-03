cmake_minimum_required(VERSION 3.20.0)

add_executable(unittest test.cpp)

target_link_libraries(unittest PRIVATE 
	wtlog_static
)

target_include_directories(unittest PRIVATE
	$<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
	$<INSTALL_INTERFACE:include>
)

enable_testing()

function(do_test Target Arg Result)
	add_test(NAME Test${Arg} COMMAND ${Target} ${Arg})
	set_tests_properties(Test${Arg} PROPERTIES
		PASS_REGULAR_EXPRESSION ${Result}
	)
endfunction()

do_test(unittest "*", "pass")


