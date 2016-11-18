CC = gcc
CFLAGS = -g

%.o : %.c
	$(CC) -c $(CFLAGS) $(CPPFLAGS) $< -o $@

all: testSem

scanner: main.o scanner.o driver.o token.o testscanner.o treePrint.o node.o
	$(CC) -o $@ $(CFLAGS) main.o scanner.o driver.o token.o testscanner.o treePrint.o node.o

testSem: main.o scanner.o driver.o token.o parser.o node.o treePrint.o stack.o
	$(CC) -o $@ $(CFLAGS) main.o scanner.o driver.o token.o parser.o node.o treePrint.o stack.o

clean:
	rm -f parser scanner testSem
	rm -f *.o
	
