#ifndef STACK_H
#define STACK_H

#define stackmax 100

typedef struct stackstruct {
    char *variable[stackmax];     //the variable instance in array structure
    int size;
} Stack;

void initStack(Stack *s);
void push(char *var, Stack *s);
void pop(Stack *s);
char *top(Stack *s);
int find(char *var, Stack *s);
void printstack(Stack *s);


#endif