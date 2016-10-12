include makefile.inc
CC=g++
CFLAGS=-c -Wall
LDFLAGS= 
UNITSOURCES= unit_mongodb.cpp ../common/Logger.h ../common/Logger.cpp
SOURCES= DBMongo.h DBMongo.cpp 
OBJECTS = $(SOURCES:.cpp=.o)
UNITOBJECTS = $(UNITSOURCES:.cpp=.o) 
EXECUTABLE= unit_mongodb

all: $(SOURCES) $(OBJECTS)

unit: $(SOURCES) $(EXECUTABLE)

clean: 
	rm *.o

$(EXECUTABLE): $(OBJECTS) $(UNITSOURCES)
	$(CC) $(LDFLAGS) $(OBJECTS) $(UNITSOURCES) -o $@ $(NOSCHEMA-EFLAGS)

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

