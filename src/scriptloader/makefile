include makefile.inc
CC=g++
CFLAGS=-c -Wall 
EFLAGS= $(SCRIPTLOADER-EFLAGS)
LDFLAGS= 
UNITSOURCES= unit_scriptloader.cpp ../common/Logger.h ../common/Logger.cpp
SOURCES= ScriptLoader.h ScriptLoader.cpp
OBJECTS = $(SOURCES:.cpp=.o)
UNITOBJECTS = $(UNITSOURCES:.cpp=.o)
EXECUTABLE= unit_scriptloader

all: $(SOURCES) $(OBJECTS)

unit: $(SOURCES) $(UNITSOURCES) $(EXECUTABLE)

clean: 
	rm *.o

$(EXECUTABLE): $(OBJECTS) $(UNITOBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) $(UNITOBJECTS) -o $@ $(EFLAGS)

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

