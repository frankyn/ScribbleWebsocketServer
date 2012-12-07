CC=g++
CFLAGS=-c -Wall 
EFLAGS=-llua5.1
LDFLAGS= 
SOURCES= unit_scriptloader.cpp ScriptLoader.h ScriptLoader.cpp
OBJECTS = $(SOURCES:.cpp=.o)
EXECUTABLE= ScriptLoader_Unit_Test

all: $(SOURCES) $(EXECUTABLE) 

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@ $(EFLAGS)

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

