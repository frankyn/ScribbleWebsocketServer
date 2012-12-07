include makefile.inc

CC=g++
CFLAGS=-c -Wall
LDFLAGS= 
EXFLAGS=-lsqlite3
SOURCES= SQLite.cpp SQLite.h
UNITSOURCES= unit_sqlite3.cpp
OBJECTS = $(SOURCES:.cpp=.o)
UNITOBJECTS = $(UNITSOURCES:.cpp=.o)
EXECUTABLE= unit_sqlite3

all: $(SOURCES) $(OBJECTS)

unit: $(SOURCES) $(UNITSOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) $(UNITOBJECTS) -o $@  $(EXFLAGS)

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

sqlite3.obj: sqlite3.c
	$(CC) -c sqlite3.c -o sqlite.obj -DTHREADSAFE=1
