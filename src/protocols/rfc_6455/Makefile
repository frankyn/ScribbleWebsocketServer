CC=g++
CFLAGS=-c -Wall
LDFLAGS= -pthread
UNITSOURCES =  unit_rfc_6455.cc  
SOURCES= rfc_6455.h rfc_6455.cc base64/base64.h base64/base64.c sha1/sha1.h sha1/sha1.cpp ../WSProtocol.h
OBJECTS = $(SOURCES:.cpp=.o)
UNITOBJECTS = $(UNITSOURCES:.cpp=.o)
EXECUTABLE= unit_rfc_6455

all: $(SOURCES) $(OBJECTS)

unit: $(SOURCES) $(UNITSOURCES) $(EXECUTABLE)

clean:
	  rm *.o

$(EXECUTABLE): $(OBJECTS) $(UNITOBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) $(UNITOBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

