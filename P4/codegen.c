#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "codegen.h"
#include "parser.h"
#include "node.h"
#include "stack.h"
#include "genASM.h"


char *targetFile = "target.asm";

void codeGen(node_t *tree) {

    if (strcmp(tree->label, "<program>") == 0) {
        
    }

    if (strcmp(tree->label, "<vars>") == 0) {
        targetPush(targetFile);
    }

    if (strcmp(tree->label, "<mvars>") == 0) {
        targetPush(targetFile);
    }

    if (strcmp(tree->label, "<expr>") == 0) {
    
    }

    if (strcmp(tree->label, "<rpxe>") == 0) {

    }

    if (strcmp(tree->label, "<M>") == 0) {

    }

    if (strcmp(tree->label, "<N>") == 0) {

    }

    if (strcmp(tree->label, "<T>") == 0) {

    }

    if (strcmp(tree->label, "<X>") == 0) {

    }

    if (strcmp(tree->label, "<F>") == 0) {

    }

    if (strcmp(tree->label, "<R>") == 0) {

    }

    if (strcmp(tree->label, "<stats>") == 0) {

    }

    if (strcmp(tree->label, "<mStat>") == 0) {

    }

    if (strcmp(tree->label, "<stat>") == 0) {

    }

    if (strcmp(tree->label, "<in>") == 0) {

    }
    
    if (strcmp(tree->label, "<out>") == 0) {

    }

    if (strcmp(tree->label, "<if>") == 0) {

    }

    if (strcmp(tree->label, "<loop>") == 0) {

    }

    if (strcmp(tree->label, "<assign>") == 0) {

    }

    if (strcmp(tree->label, "<RO>") == 0) {

    }



    /* the Recursions */
    if (tree->child1 != NULL)
        codeGen(tree->child1);
    
    if (tree->child2 != NULL)
        codeGen(tree->child2);

    if (tree->child3 != NULL)
        codeGen(tree->child3);
    
    if (tree->child4 != NULL) 
        codeGen(tree->child4);
}



void test() {
    
    printf("top is %s\n",top(sharedStack));
}

void testpop() {
    pop(sharedStack);
     printf("top is %s\n",top(sharedStack));
}

int stringsum(char *strLabel) {
    int i;
    int total = 0;
    for (i=0; i<strlen(strLabel); i++) {
        total+=strLabel[i];
    }
    return total;
}