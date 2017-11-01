cmake_minimum_required(VERSION 3.5)

include_directories( /usr/local/systemc-2.3.1/include/ )
include_directories( /home/felixh/programs/catch/include/ )
 

add_executable( airbag.x
	airbag.cpp
	airbag.h
	test_bed_main.cpp
	test_bed_main.h
	fisignal.h
	airbag_testbed.cpp
	)

target_link_libraries( airbag.x
	/usr/local/systemc-2.3.1/lib-linux64/libsystemc.a
	pthread )

#########################
# Test cases            #
#########################

include(CTest)

# run the test executable
add_test (test-run airbag.x -a)
