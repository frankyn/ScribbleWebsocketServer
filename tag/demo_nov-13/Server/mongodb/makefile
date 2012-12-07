include makefile.inc
CC=g++
CFLAGS=-c -Wall 
EFLAGS= -pthread -lmongoclient -lboost_thread-mt -lboost_filesystem -lboost_program_options -lboost_system
LDFLAGS= 
UNITSOURCES= UNIT_DBMongo.cpp 
SOURCES= DBMongo.h DBMongo.cpp
OBJECTS = $(SOURCES:.cpp=.o)
UNITOBJECTS = $(UNITSOURCES:.cpp=.o)
EXECUTABLE= MongoDB_Unit_Test

all: $(SOURCES) $(OBJECTS)

unit: $(SOURCES) $(EXECUTABLE)

clean: 
	rm *.o
	rm *~

$(EXECUTABLE): $(OBJECTS) $(UNITSOURCES)
	$(CC) $(LDFLAGS) $(OBJECTS) $(UNITSOURCES) -o $@ $(EFLAGS)

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

