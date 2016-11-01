#include "node.h"
#include <stdio.h>
#include <stdlib.H>

node_t *getNode(char *label) {
    node_t *node = (node_t *)malloc(sizeof(node_t)); //create the node
    node->label = label;    //label the node with the nontermial
    return node;
}