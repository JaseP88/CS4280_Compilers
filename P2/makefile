CC = gcc
CFLAGS = -g

%.o : %.c
	$(CC) -c $(CFLAGS) $(CPPFLAGS) $< -o $@

all: testScanner

testScanner: main.o scanner.o driver.o token.o testscanner.o
	$(CC) -o $@ $(CFLAGS) main.o scanner.o driver.o token.o testscanner.o

clean:
	rm -f testScanner
	rm -f *.o
	
