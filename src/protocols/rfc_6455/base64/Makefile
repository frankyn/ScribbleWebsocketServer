CXX     = g++
CFLAGS  = -Wall -c -g
LDFLAGS =
SOURCE  = base64.cc
OBJECTS = $(SOURCE:.cc=.o)

all: $(OBJECTS)
 
.PHONY: clean
clean: 
	rm *.o 

.cc.o:
	$(CXX) $(CFLAGS) $< $(LDFLAGS) -o $@
