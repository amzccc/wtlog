cmake_minimum_required(VERSION 3.20.0)

add_executable(UnitTest test.cpp)

target_link_libraries(UnitTest PRIVATE WtLog)

enable_testing()

function(do_test Target Arg Result)
	add_test(NAME Test${Arg} COMMAND ${Target} ${Arg})
	set_tests_properties(Test${Arg} PROPERTIES
		PASS_REGULAR_EXPRESSION ${Result}
	)
endfunction()



