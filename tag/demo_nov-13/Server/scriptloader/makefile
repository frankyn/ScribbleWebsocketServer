include makefile.inc
CC=g++
CFLAGS=-c -Wall 
EFLAGS=-llua5.1
LDFLAGS= 
UNITSOURCES= unit_scriptloader.cpp 
SOURCES= ScriptLoader.h ScriptLoader.cpp
OBJECTS = $(SOURCES:.cpp=.o)
UNITOBJECTS = $(UNITSOURCES:.cpp=.o)
EXECUTABLE= ScriptLoader_Unit_Test

all: $(SOURCES) $(OBJECTS)

unit: $(SOURCES) $(UNITSOURCES) $(EXECUTABLE)

clean: 
	rm *.o
	rm *~

$(EXECUTABLE): $(OBJECTS) $(UNITOBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) $(UNITOBJECTS) -o $@ $(EFLAGS)

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

