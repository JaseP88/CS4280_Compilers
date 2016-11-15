#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "stack.h"
#define stackmax 100 

void initStack(Stack *s) {
    s->size = -1;
}

void push(char *var, Stack *s) {
    if (s->size > stackmax-1) {
        printf("ERROR: stack is full cannot push\n");
        exit(1);
    }
    else {
        s->size++;
        s->variable[s->size] = var;
        printf("+++pushing: %s\n",s->variable[s->size]);
    }
}

void pop(Stack *s) {
    if (s->size == - 1) {
        printf("ERROR: stack is empty cannot pop\n");
        exit(1);
    }
    else {
        printf("\t---popping: %s\n",top(s));
        s->size--;
    }
}

char *top(Stack *s) {
    if (s->size == -1) {
        printf("ERROR: stack is empty no top stack element\n");
        exit(1);
    }
   return s->variable[s->size];
}

int find(char *var, Stack *s) {
    int distance = 0;
    int top_size = s->size;

    while (top_size > -1) {
        if (strcmp(var, s->variable[top_size]) != 0) {  //if var and the top stack is not same add distance and reduce the top size(pop)
            distance++;
            top_size--;
        }
        else {
            //printf("!!!!!FOUND at distance %d from stack\n",distance);
            return distance;
        } 
    }
    return -1;  //return -1 if didnt find
}

//bad function
void printstack(Stack *s) {
    int i;
    for (i=0; i<s->size; i++)
        printf("%s\n",s->variable[s->size]);
}