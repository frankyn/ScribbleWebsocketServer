CXX          = g++
CFLAGS       = -Wall -c -g
LDFLAGS      =
UNITSOURCES  = unit_rfc_6455.cc  
SOURCES      = rfc_6455.cc 
DEPENDENCIES = sha1 base64
OBJECTS      = $(SOURCES:.cc=.o) sha1/sha1.o base64/base64.o 
UNITOBJECTS  = $(UNITSOURCES:.cc=.o)
EXECUTABLE   = unit_rfc_6455

all: $(DEPENDENCIES) $(SOURCES) $(OBJECTS) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS) $(UNITOBJECTS)
	$(CXX) $(OBJECTS) $(UNITOBJECTS) $(LDFLAGS) -o $@

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

