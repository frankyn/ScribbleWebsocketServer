CC=g++
CFLAGS=-c -Wall
LDFLAGS= -pthread
UNITSOURCES =  unit_protocol.cpp  
SOURCES= RFC_6455.h RFC_6455.cpp base64/base64.h base64/base64.c sha1/sha1.h sha1/sha1.cpp ../WSProtocol.h
OBJECTS = $(SOURCES:.cpp=.o)
UNITOBJECTS = $(UNITSOURCES:.cpp=.o)
EXECUTABLE= RFC_6455_Unit_Test

all: $(SOURCES) $(OBJECTS)

unit: $(SOURCES) $(UNITSOURCES) $(EXECUTABLE)

clean:
	  rm *.o

$(EXECUTABLE): $(OBJECTS) $(UNITOBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) $(UNITOBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

