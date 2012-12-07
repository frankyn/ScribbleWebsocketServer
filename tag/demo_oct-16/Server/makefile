CC=g++
CFLAGS=-c -Wall -g
EFLAGS=-llua5.1
LDFLAGS= -pthread
SOURCES= main.cpp ./scriptloader/ScriptLoader.h ./scriptloader/ScriptLoader.cpp ./protocols/WSProtocol.h ./protocols/rfc_6455/RFC_6455.h ./protocols/rfc_6455/RFC_6455.cpp ./protocols/rfc_6455/sha1/sha1.h ./protocols/rfc_6455/sha1/sha1.cpp ./protocols/rfc_6455/base64/base64.c ./protocols/rfc_6455/base64/base64.h ./Connection.h ./Connection.cpp ./ConnectionsWaiting.h ./ConnectionsWaiting.cpp ./ChannelSelector.h ./ChannelSelector.cpp ./Channel.h ./Channel.cpp ./common/SocketClass.h ./common/SocketClass.cpp ./ListenerClass.cpp ./ListenerClass.h ./common/ThreadClass.cpp ./common/ThreadClass.h ./common/SemaphoreClass.cpp ./common/SemaphoreClass.h ./common/LockClass.cpp ./common/LockClass.h ./common/json/jsonxx.cc ./common/json/jsonxx.h ./common/Logger.cpp ./common/Logger.h
OBJECTS = $(SOURCES:.cpp=.o)
EXECUTABLE= Scribble

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@ $(EFLAGS)

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

