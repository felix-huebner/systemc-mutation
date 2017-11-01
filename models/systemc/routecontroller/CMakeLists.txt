cmake_minimum_required(VERSION 3.5)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories( /usr/local/systemc-2.3.1/include/ )
include_directories( /home/felixh/programs/catch/include/ )
 

add_executable( route_lyngby_r_12a.x
	route_lyngby_r_12a_testbed.cpp
	test_bed_main.cpp
	route_lyngby_r_12a.cpp
	routecontrollergeneric.cpp
	)

target_link_libraries( route_lyngby_r_12a.x
	/usr/local/systemc-2.3.1/lib-linux64/libsystemc.a
	pthread )
	
#########################
# Test cases            #
#########################

include(CTest)

# run the test executable
add_test (test-run route_lyngby_r_12a.x -a)
