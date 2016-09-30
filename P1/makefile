CC = gcc
CFLAGS = -g

%.o : %.c
	$(CC) -c $(CFLAGS) $(CPPFLAGS) $< -o $@

all: funproject 

funproject: main.o buildtree.o traversals.o
	$(CC) -o $@ $(CFLAGS) main.o buildtree.o traversals.o

clean:
	rm -f funproject
	rm -f *.o
	rm -f out.*
	rm -f *.inorder *.preorder *.postorder
