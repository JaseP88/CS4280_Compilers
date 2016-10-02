CC = gcc
CFLAGS = -g

%.o : %.c
	$(CC) -c $(CFLAGS) $(CPPFLAGS) $< -o $@

all: scanner

scanner: main.o 
	$(CC) -o $@ $(CFLAGS) main.o 

clean:
	rm -f scanner
	rm -f *.o
	
