include makefile.inc

CC=g++
CFLAGS=-c -Wall
LDFLAGS= 
EFLAGS=$(SQLSCHEMA-EFLAGS)
SOURCES= SQLite.cpp SQLite.h 
UNITSOURCES= unit_sqlite3.cpp ../common/Logger.h ../common/Logger.cpp
OBJECTS = $(SOURCES:.cpp=.o)
UNITOBJECTS = $(UNITSOURCES:.cpp=.o)
EXECUTABLE= unit_sqlite3

all: $(SOURCES) $(OBJECTS)

unit: $(SOURCES) $(UNITSOURCES) $(EXECUTABLE)

clean:
	rm *.o

$(EXECUTABLE): $(OBJECTS) $(UNITOBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) $(UNITOBJECTS) -o $@  $(EFLAGS)

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

sqlite3.obj: sqlite3.c
	$(CC) -c sqlite3.c -o sqlite.obj -DTHREADSAFE=1
