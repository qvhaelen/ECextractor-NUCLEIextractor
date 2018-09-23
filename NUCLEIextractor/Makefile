CC=g++
CFLAGS= -c  -O3# -v -da -Q
LDFLAGS= 

objects := $(patsubst %.cc,%.o,$(wildcard *.cc))


all: NUCLEIextractor

NUCLEIextractor: $(objects)
	$(CC)  -o NUCLEIextractor $(objects)

%.o: %.cc
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm *.o 
