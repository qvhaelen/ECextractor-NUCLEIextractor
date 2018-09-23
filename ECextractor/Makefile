CC=g++
CFLAGS= -c  -O3#  -v -da -Q 
LDFLAGS= 

objects := $(patsubst %.cc,%.o,$(wildcard *.cc))


all: ECextractor

ECextractor: $(objects)
	$(CC)  -o ECextractor $(objects)

%.o: %.cc
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm *.o 
