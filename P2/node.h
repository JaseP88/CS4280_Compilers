#ifndef NODE_H
#define NODE_H

#include "token.h"

typedef struct node_t {
    char *label;                    //label
    tlk  *token;                    //token
    struct node_t *child1;          //child1 
    struct node_t *child2;          //child2
    struct node_t *child3;
    struct node_t *child4;
} node_t;

node_t *getNode(char *label);

#endif