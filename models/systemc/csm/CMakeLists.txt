cmake_minimum_required(VERSION 3.5)

include_directories( /usr/local/systemc-2.3.1/include/ )
include_directories( /home/felixh/programs/catch/include/ )
 

add_executable( csm.x
	ceiling_speed_monitor_testbed.cpp
	test_bed_main.cpp
	ceiling_speed_monitor.cpp
	)

target_link_libraries( csm.x
	/usr/local/systemc-2.3.1/lib-linux64/libsystemc.a
	pthread )
	
#########################
# Test cases            #
#########################

include(CTest)

# run the test executable
add_test (test-run csm.x -a)
