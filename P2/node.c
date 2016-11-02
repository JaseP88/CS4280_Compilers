#include "node.h"
#include <stdio.h>
#include <stdlib.H>

node_t *getNode(char *label, int level) {
    node_t *node = (node_t *)malloc(sizeof(node_t)); //create the node
    node->label = label;    //label the node with the nontermial
    node->level = level;
    node->tok = NULL;
    node->child1 = NULL;
    node->child2 = NULL;
    node->child3 = NULL;
    node->child4 = NULL;
    return node;
}