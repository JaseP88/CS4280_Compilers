CC = gcc
CFLAGS = -g

%.o : %.c
	$(CC) -c $(CFLAGS) $(CPPFLAGS) $< -o $@

all: parser

scanner: main.o scanner.o driver.o token.o testscanner.o treePrint.o node.o
	$(CC) -o $@ $(CFLAGS) main.o scanner.o driver.o token.o testscanner.o	treePrint.o node.o

parser: main.o scanner.o driver.o token.o parser.o node.o treePrint.o
	$(CC) -o $@ $(CFLAGS) main.o scanner.o driver.o token.o parser.o node.o treePrint.o

clean:
	rm -f parser scanner
	rm -f *.o
	
