#include <stdio.h>
#include <stdlib.h>

#include "node.h"
#include "treePrint.h"

static int n = 0;

//      Print the tree in preorder
void printTree(node_t *tree) {

    if (tree == NULL) {
       return;
    }

    else {
        if (tree->tok.tk_Id != -1) 
            printf("%*c%s  lev:%d--ID:%d --line:%d--inst:%s\n",tree->level*2,' ',tree->label,tree->level,tree->tok.tk_Id,tree->tok.line,tree->tok.tk_inst);
            //printf("%*c%s %-9d / %d / %s\n",tree->level*2,' ',tree->label,tree->token->tk_Id,tree->token->line,tree->token->tk_inst);
            //printf("ok %d\n",n);

        if (tree->child1 != NULL)
            printTree(tree->child1);
        
        if (tree->child2 != NULL)
            printTree(tree->child2);
        
        if (tree->child3 != NULL)
            printTree(tree->child3);

        if (tree->child4 != NULL)
            printTree(tree->child4);
    }
}
