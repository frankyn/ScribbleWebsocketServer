CC=g++
CFLAGS=-c -Wall
LDFLAGS= -pthread 
SOURCES= unit_protocol.cpp RFC_6455.h RFC_6455.cpp base64/base64.h base64/base64.c sha1/sha1.h sha1/sha1.cpp ../WSProtocol.h
OBJECTS = $(SOURCES:.cpp=.o)
EXECUTABLE= RFC_6455_Unit_Test

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

