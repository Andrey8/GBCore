cmake_minimum_required( VERSION 3.5.1 )

project( gb-core )



set( SRC_DIR src )
set( TEST_DIR tests )
set( MY_TEST_DIR tests/my-tests )

set( SRC_LIST
	${SRC_DIR}/GeometryConstruction.cpp
	${SRC_DIR}/Geometry.cpp
	${SRC_DIR}/Utils.cpp )

set( MY_TESTS_SRC_LIST
	${MY_TEST_DIR}/ConstructionContainerTests.cpp
	${MY_TEST_DIR}/GeometryConstructionTests.cpp
	${MY_TEST_DIR}/TestsLauncher.cpp )

set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -Wall" )



add_library( gb-core SHARED ${SRC_LIST} )

add_executable( my-tests ${MY_TESTS_SRC_LIST} )

target_link_libraries( my-tests PUBLIC gb-core )