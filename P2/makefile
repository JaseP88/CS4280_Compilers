CC = gcc
CFLAGS = -g

%.o : %.c
	$(CC) -c $(CFLAGS) $(CPPFLAGS) $< -o $@

all: parser

testScanner: main.o scanner.o driver.o token.o testscanner.o
	$(CC) -o $@ $(CFLAGS) main.o scanner.o driver.o token.o testscanner.o	

parser: main.o scanner.o driver.o token.o parser.o node.o treePrint.o
	$(CC) -o $@ $(CFLAGS) main.o scanner.o driver.o token.o parser.o node.o treePrint.o

clean:
	rm -f parser testScanner
	rm -f *.o
	
