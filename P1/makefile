CC = gcc
CFLAGS = -g

%.o : %.c
	$(CC) -c $(CFLAGS) $(CPPFLAGS) $< -o $@

all: proj1

proj1: main.o scanner.o driver.o token.o testscanner.o
	$(CC) -o $@ $(CFLAGS) main.o scanner.o driver.o token.o testscanner.o

clean:
	rm -f scanner
	rm -f *.o
	
