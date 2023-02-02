CC=g++
FLAGS=-std=c++11 -Wall

SRC_DIR=../src
TESTS_DIR=../tests

CORE_OBJECTS=GeometryConstruction.o Utils.o Geometry.o
GCTEST_OBJECTS=GeometryConstructionTests.o TestsLauncher.o
#CCTEST_OBJECTS=
EXECUTABLES=gc_test



all : core core_tests

core : $(CORE_OBJECTS)

core_tests : $(GCTEST_OBJECTS)



# Core Targets :

GeometryConstruction.o : $(addprefix $(SRC_DIR)/, GeometryConstruction.cpp GeometryConstruction.h  ConstructionContainer.h)
	$(CC) -c $(SRC_DIR)/GeometryConstruction.cpp $(FLAGS)

Utils.o : $(addprefix $(SRC_DIR)/, Utils.cpp Utils.h )
	$(CC) -c $(SRC_DIR)/Utils.cpp $(FLAGS)

Geometry.o : $(addprefix $(SRC_DIR)/, Geometry.cpp Geometry.h )
	$(CC) -c $(SRC_DIR)/Geometry.cpp $(FLAGS)



# CoreTests Targets :

gc_test : $(CORE_OBJECTS) $(GCTEST_OBJECTS)
	$(CC) -o $@ $(GCTEST_OBJECTS) $(CORE_OBJECTS) $(FLAGS)

TestsLauncher.o : $(addprefix $(TESTS_DIR)/, TestsLauncher.cpp ConstructionContainerTests.h GeometryConstructionTests.h)
	$(CC) -c $(TESTS_DIR)/TestsLauncher.cpp $(FLAGS)

GeometryConstructionTests.o : $(addprefix $(TESTS_DIR)/, GeometryConstructionTests.cpp GeometryConstructionTests.h) $(SRC_DIR)/GeometryConstruction.h
	$(CC) -c $(TESTS_DIR)/GeometryConstructionTests.cpp $(FLAGS)



clean-all :
	rm -v *.o $(EXECUTABLES)
