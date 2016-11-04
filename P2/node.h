#ifndef NODE_H
#define NODE_H

#include "token.h"

typedef struct nodestruct {
    char *label;                    //label
    tlk  tok;                    //token
    int level;                      //level of the node
    struct nodestruct *child1;          //child1 
    struct nodestruct *child2;          //child2
    struct nodestruct *child3;
    struct nodestruct *child4;
} node_t;

node_t *getNode(char *label, int level);

#endif