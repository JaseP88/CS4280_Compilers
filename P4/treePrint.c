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
        printf("%*c%s ",tree->level*2,' ',tree->label,tree->level);

        if (tree->tok.tk_Id != -1 && tree->tok2.tk_Id != -1) { //if 2 semantic token in node, only 1 BNF <assign> has 2 semantic token
            printf(" tokenID1:%d tok1line:%d token1:%s / tokenID2:%d toke2line:%d token2:%s\n",tree->tok.tk_Id,tree->tok.line,tree->instance,tree->tok2.tk_Id,tree->tok2.line,tree->tok2.tk_inst);
        }
        else if (tree->tok.tk_Id != -1 && tree->instance != NULL)   //if only 1 semantic token and is Identifier or integer literal
            printf(" tokenID:%d  line:%d  semantic token:%s\n",tree->tok.tk_Id,tree->tok.line,tree->instance);
        
        else if (tree->tok.tk_Id != -1) //if only 1 semantic token and is an operator
            printf(" tokenID:%d line:%d semantic token:%s\n",tree->tok.tk_Id,tree->tok.line,tree->tok.tk_inst);  

        printf("\n");

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
