CXX          = g++
CFLAGS       = -Wall -c -g
LDFLAGS      =
SOURCES      = rfc_6455.cc unit_rfc_6455.cc
DEPENDENCIES = sha1 base64
OBJECTS      = $(SOURCES:.cc=.o) sha1/sha1.o base64/base64.o 
EXECUTABLE   = unit_rfc_6455

all: $(DEPENDENCIES) $(SOURCES) $(OBJECTS) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS) 
	$(CXX) $(OBJECTS) $(LDFLAGS) -o $@

.cc.o:
	$(CXX) $(CFLAGS) $< $(LDFLAGS) -o $@

.PHONY: $(DEPENDENCIES)
$(DEPENDENCIES):
	$(MAKE) -C $@

.PHONY: clean
clean:
	$(RM) *.o $(EXECUTABLE)
	$(MAKE) -C sha1 clean
	$(MAKE) -C base64 clean 

