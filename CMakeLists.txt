cmake_minimum_required( VERSION 3.5.1 )

project( gb-core )



set( SRC_DIR src )
set( TEST_DIR tests )

set( MY_TEST_DIR tests/my-tests )
set( GOOGLE_TEST_DIR  tests/google-tests )

set( GOOGLE_TEST_LIB_DIR lib/googletest-main )

set( SRC_LIST
	${SRC_DIR}/GeometryConstruction.cpp
	${SRC_DIR}/Geometry.cpp
	${SRC_DIR}/Utils.cpp )

set( MY_TESTS_SRC_LIST
	${MY_TEST_DIR}/ConstructionContainerTests.cpp
	${MY_TEST_DIR}/GeometryConstructionTests.cpp
	${MY_TEST_DIR}/TestsLauncher.cpp )

set( GOOGLE_TESTS_SRC_LIST 
	${GOOGLE_TEST_DIR}/Geometry_UnitTest.cpp
	${GOOGLE_TEST_DIR}/TestsLauncher.cpp
	${TEST_DIR}/Utils.cpp )

set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -Wall" )



include_directories( ${GOOGLE_TEST_LIB_DIR}/googletest/include )

add_subdirectory( ${GOOGLE_TEST_LIB_DIR} )



add_library( gb-core SHARED ${SRC_LIST} )

add_executable( my-tests ${MY_TESTS_SRC_LIST} )
add_executable( google-tests ${GOOGLE_TESTS_SRC_LIST} )

target_link_libraries( my-tests PUBLIC gb-core )
target_link_libraries( google-tests PUBLIC gb-core gtest )