include scriptloader/makefile.inc
include mysql/makefile.inc

CC=g++
CFLAGS=-c -Wall -g
EFLAGS=$(SCRIPTLOADER-EFLAGS) $(SQLSCHEMA-EFLAGS) 
LDFLAGS= -pthread
DIRS= scriptloader mysql 
BUILDDIRS= $(DIRS:%=%)
SOURCES= main.cpp ./protocols/WSProtocol.h ./protocols/rfc_6455/RFC_6455.h ./protocols/rfc_6455/RFC_6455.cpp ./protocols/rfc_6455/sha1/sha1.h ./protocols/rfc_6455/sha1/sha1.cpp ./protocols/rfc_6455/base64/base64.c ./protocols/rfc_6455/base64/base64.h ./Connection.h ./Connection.cpp ./ConnectionsWaiting.h ./ConnectionsWaiting.cpp ./ChannelSelector.h ./ChannelSelector.cpp ./Channel.h ./Channel.cc ./common/SocketClass.h ./common/SocketClass.cpp ./common/TCPClient.h ./common/TCPClient.cpp ./common/TCPListener.h ./common/TCPListener.cpp ./ListenerClass.cpp ./ListenerClass.h ./common/ThreadClass.cpp ./common/ThreadClass.h ./common/SemaphoreClass.cpp ./common/SemaphoreClass.h ./common/LockClass.cpp ./common/LockClass.h ./common/Logger.cpp ./common/Logger.h
OBJECTS= $(SOURCES:.cpp=.o)
EXECUTABLE= Scribble
SCRIPTLOADER_PATH= $(SCRIPTLOADER:%=scriptloader/%) 
SQLSCHEMA_PATH = $(SQLSCHEMA:%=mysql/%)

all: $(SOURCES) MAKEALL $(EXECUTABLE)

clean: 
	rm *.o
	$(MAKE) clean -C scriptloader
	$(MAKE) clean -C mysql

$(EXECUTABLE): $(OBJECTS)
	$(clean)
	$(CC) $(LDFLAGS) $(OBJECTS) $(SCRIPTLOADER_PATH) $(SQLSCHEMA_PATH) -o $@ $(EFLAGS)

MAKEALL: $(BUILDDIRS)
	$(MAKE) -C scriptloader
	$(MAKE) -C mysql

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

