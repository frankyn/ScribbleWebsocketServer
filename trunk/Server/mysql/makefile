include makefile.inc

CC=g++
CFLAGS=-c -Wall 
LDFLAGS= 
EFLAGS=$(SQLSCHEMA-EFLAGS)
SOURCES= MySQL.cpp MySQL.h 
UNITSOURCES= unit_mysql.cpp ../common/Logger.h ../common/Logger.cpp
OBJECTS = $(SOURCES:.cpp=.o)
UNITOBJECTS = $(UNITSOURCES:.cpp=.o)
EXECUTABLE= unit_mysql

all: $(SOURCES) $(OBJECTS)

unit: $(SOURCES) $(UNITSOURCES) $(EXECUTABLE)

clean:
	rm *.o

$(EXECUTABLE): $(OBJECTS) $(UNITOBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) $(UNITOBJECTS) -o $@  $(EFLAGS)

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

mysql.obj: mysql.c
	$(CC) -c mysql.c -o mysql.obj -DTHREADSAFE=1
