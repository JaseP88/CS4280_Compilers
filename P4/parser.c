#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "parser.h"
#include "scanner.h"
#include "token.h"
#include "node.h"
#include "stack.h"
#include "treePrint.h"
#include "codegen.h"

char *asmfile = "target.asm";   //the target file produced

/* contain counters for tempvars 
and labels for branching */
int maximumTemp = 0;
int outcounter = 0;
int condcounter = 0;
int loopcounter = 0;

/* auxillary function  <*/
void parser(char *filename) {
    tlk *token = (tlk *)malloc(sizeof(tlk));    //create token in heap memory
    node_t *root;

    scanner(filename,token); //retrieve token
    root = program(filename,token); //call program

    if (token->tk_Id != Eof_Tk) 
        errCond("parser","EOF",token->tk_inst,token->line);    //prints error messages and exits
    
    else {
        printf("parser complete and is A-OK\n");
        //printTree(root);
    }
}

/* <program> */
node_t *program(char *filename, tlk *tk) {
    int i;
    int level = 0;
    int scope = 0;
    node_t *node = getNode("<program>",level);  //create node for <program> and label it and also get level of the node
    
    /* CREATE GLOBAL VAR COUNT FOR FS16 LANG */
    int *varCount = (int *)malloc(sizeof(int));
    *varCount = 0;

    char tempno[10];

    /* CREATE THE STACK FOR THE PROGRAM */
    Stack *stack = (Stack *)malloc(sizeof(Stack));
    initStack(stack);

    node->child1 =  vars(filename,tk,level,varCount,stack,scope); 
    node->child2  = block(filename,tk,level,stack,scope);
    
    //code gen stop
    targetStop(asmfile);
    for (i=0; i<=maximumTemp; i++) { //code gen storage allocation for temp vars using storage directive *after STOP*
        sprintf(tempno,"t%d",i);
        targetTempInit(asmfile,tempno);
    }

/* Maybe no need to pop global scopes???   
    //pop the global variables of fs16 language
    for (i=0; i<*varCount; i++) 
        pop(stack);
*/
    return node;
}

/* <block> */
node_t *block(char *filename, tlk *tk, int level, Stack *s, int scope) {
    level++;
    scope++;
    int i;

    node_t *node = getNode("<block>",level);   //create node for <block> and label it

    int *varCount = (int *)malloc(sizeof(int)); //create the varCount for each block scope
    *varCount = 0;  //init set the varCount to 0 for this block
    
    if (tk->tk_Id == Bgn_Tk) {  //structural token no need to store
        scanner(filename,tk);   //consume the token and get new token
        
        node->child1 = vars(filename,tk,level,varCount,s,scope);
        node->child2 = stats(filename,tk,level,s,scope);
   
        if (tk->tk_Id == End_Tk) {
            //pop the instance of each block when done
            for (i=0; i<*varCount; i++)  {
                pop(s);
                targetPop(asmfile); //codegen pop
            }
            scanner(filename,tk);
            return node; 
        }
        else
            errCond("block","End",tk->tk_inst,tk->line);
    }
    else
        errCond("block","Begin",tk->tk_inst,tk->line);
}

/* <vars> */
node_t *vars(char *filename, tlk *tk, int level, int *varcount, Stack *s, int scope) {
    level++;
    tlk temp;
    char *tempname;
    int number;
    
    node_t *node = getNode("<vars>",level);
    node->instance = (char *)malloc(24);

    if (tk->tk_Id == Var_Tk) {
        scanner(filename,tk);
        if (tk->tk_Id == Identifier_Tk) {   //save this semantic token

            temp = *tk;
            node->tok = temp;
            
            tempname = tk->tk_inst;
            tempname+=5;    //only take the name (removed string "idtk:")
            strcpy(node->instance,tempname);
            
            //originally varcount is 0 in all vars()
            push(node->instance,s);
            targetPush(asmfile);
            
            // checks if there are duplicate var out of this block scope
            // returns 1 if there is only 1 instance of the variable in stack
            int numofvar = varcounter(node->instance,s);
            if (numofvar == 1)  
                targetVarInit(asmfile,node->instance,0);    //disables multiple var init in codegen

            *varcount+=1;   //increment the varcount for this identifier
           
            scanner(filename,tk);
            node->child1 = mvars(filename,tk,level,varcount,s,scope);   //pass the varcount to mvars
           
            return node;
        }
        else
            errCond("vars","ID",tk->tk_inst,tk->line);
    }

    return NULL; //empty production
}

/* <mvars> */
node_t *mvars(char *filename, tlk *tk, int level, int *varcount, Stack *s, int scope) {
    level++;
    tlk temp;
    char *tempname;
    int number;

    node_t *node = getNode("<mvars>",level);
    node->instance = (char *)malloc(24);

    if (tk->tk_Id == Cln_Tk) {
        scanner(filename,tk);
        if (tk->tk_Id == Cln_Tk) {
            scanner(filename,tk);
            if (tk->tk_Id == Identifier_Tk) {

                temp = *tk;
                node->tok = temp;
                tempname = tk->tk_inst;
                tempname+=5;
                strcpy(node->instance,tempname);

                //push identifier into stack 
                if (*varcount > 0) {
                    number = find(node->instance,s);  //find the identifier
                    if (number == -1 || number >= *varcount) {
                        push(node->instance,s);
                        targetPush(asmfile);
                        
                        int numofvar = varcounter(node->instance,s);
                        if (numofvar == 1)  //no duplicate var outside scope
                            targetVarInit(asmfile,node->instance,0);
                        
                        *varcount+=1;
                        
                    }
                    else if (number < *varcount) {
                        printf("\nERROR: multiple identifiers. %s already in scope, line %d\n",node->instance,temp.line);
                        exitAndDel(asmfile);
                    }
                }
                
                scanner(filename,tk);
                node->child1 = mvars(filename,tk,level,varcount,s,scope);
                return node;
            }
            else
                errCond("mvars","ID",tk->tk_inst,tk->line);
        }
        else
            errCond("mvars",":",tk->tk_inst,tk->line);
    }
 
    return NULL; //empty production
}

/* <expr> */
node_t *expr(char *filename, tlk *tk, int level, Stack *s, int *targtemp) {
    level++;
    node_t *node = getNode("<expr>",level);

    node->child1 = M(filename,tk,level,s,targtemp);
    node->child2 = rpxe(filename,tk,level,s,targtemp);
    return node;
}

/* <rpxe> backwards HAH */
node_t *rpxe(char *filename, tlk *tk, int level, Stack *s, int *targtemp) {
    tlk temp;
    level++;
    node_t *node = getNode("<rpxe>",level);
  
    if (tk->tk_Id == Plus_Tk) {
        temp = *tk;
        node->tok = temp;

        scanner(filename,tk);
        node->child1 = expr(filename,tk,level,s,targtemp);

        char tempno[10];
        int tempnum = *targtemp-1;

        sprintf(tempno,"t%d",tempnum-1);
        targetInstructAlpha(asmfile,"LOAD",tempno);
        sprintf(tempno,"t%d",tempnum);
        targetInstructAlpha(asmfile,"ADD",tempno);
        sprintf(tempno,"t%d",tempnum-1);
        targetInstructAlpha(asmfile,"STORE",tempno);
        *targtemp-=1;

        return node;
    }
    
    return NULL;
}

/* <M> */
node_t *M(char *filename, tlk *tk, int level, Stack *s, int *targtemp) {
    level++;
    node_t *node = getNode("<M>",level);

    node->child1 = T(filename,tk,level,s,targtemp);
    node->child2 = N(filename,tk,level,s,targtemp);
    return node;
}

/* <N> */
node_t *N(char *filename, tlk *tk, int level, Stack *s, int *targtemp) {
    level++;
    tlk temp;
    node_t *node = getNode("<N>",level);

    if (tk->tk_Id == Minus_Tk) {
        temp = *tk;
        node->tok = temp;
        scanner(filename,tk);
        node->child1 = M(filename,tk,level,s, targtemp);

        char tempno[10];
        int tempnum = *targtemp-1;

        sprintf(tempno,"t%d",tempnum-1);
        targetInstructAlpha(asmfile,"LOAD",tempno);
        sprintf(tempno,"t%d",tempnum);
        targetInstructAlpha(asmfile,"SUB",tempno);
        sprintf(tempno,"t%d",tempnum-1);
        targetInstructAlpha(asmfile,"STORE",tempno);
        *targtemp-=1;

        return node;
    }
    
    return NULL;
}

/* <T> */
node_t *T(char *filename, tlk *tk, int level, Stack *s, int *targtemp) {
    level++;
    node_t *node = getNode("<T>",level);
    node->child1 = F(filename,tk,level,s,targtemp);
    node->child2 = X(filename,tk,level,s,targtemp);
    return node;
}

/* <X> */ 
node_t *X(char *filename, tlk *tk, int level, Stack *s, int *targtemp) {
    level++;
    tlk temp;
    node_t *node = getNode("<X>",level);

    if (tk->tk_Id == Ast_Tk) {
        temp = *tk;
        node->tok = temp;

        scanner(filename,tk);
        node->child1 = T(filename,tk,level,s,targtemp);

        char tempno[10];
        int tempnum = *targtemp-1;
        
        sprintf(tempno,"t%d",tempnum-1);
        targetInstructAlpha(asmfile,"LOAD",tempno);
        sprintf(tempno,"t%d",tempnum);
        targetInstructAlpha(asmfile,"MULT",tempno);
        sprintf(tempno,"t%d",tempnum-1);
        targetInstructAlpha(asmfile,"STORE",tempno);
        *targtemp-=1;

        return node;
    }
    else if (tk->tk_Id == BkSlash_Tk) {
        temp = *tk;
        node->tok = temp;
        scanner(filename,tk);
        node->child1 = T(filename,tk,level,s,targtemp);

        char tempno[10];
        int tempnum = *targtemp-1;
        
        sprintf(tempno,"t%d",tempnum-1);
        targetInstructAlpha(asmfile,"LOAD",tempno);
        sprintf(tempno,"t%d",tempnum);
        targetInstructAlpha(asmfile,"DIV",tempno);
        sprintf(tempno,"t%d",tempnum-1);
        targetInstructAlpha(asmfile,"STORE",tempno);
        *targtemp-=1;

        return node;
    }
     
    return NULL;
}

/* <F> */
node_t *F(char *filename, tlk *tk, int level, Stack *s, int *targtemp) {
    level++;
    tlk temp;
    node_t *node = getNode("<F>",level);

    if (tk->tk_Id == Minus_Tk) {
        temp = *tk;
        node->tok = temp;
        scanner(filename,tk);
        node->child1 = F(filename,tk,level,s,targtemp);

        char tempno[10];
        int tempnum = *targtemp-1;

        sprintf(tempno,"t%d",tempnum);
        targetInstructAlpha(asmfile,"LOAD",tempno);
        targetInstructAlpha(asmfile,"MULT","2");
        sprintf(tempno,"t%d",tempnum+1);
        targetInstructAlpha(asmfile,"STORE",tempno);
        sprintf(tempno,"t%d",tempnum);
        targetInstructAlpha(asmfile,"LOAD",tempno);
        sprintf(tempno,"t%d",tempnum+1);
        targetInstructAlpha(asmfile,"SUB",tempno);
        sprintf(tempno,"t%d",tempnum);
        targetInstructAlpha(asmfile,"STORE",tempno);

        return node;
    }
    else 
        node->child1 = R(filename,tk,level,s,targtemp);

    return node;
}

/* <R> */
node_t *R(char *filename, tlk *tk, int level, Stack *s, int *targtemp) {
    level++;
    tlk temp;
    char *tempname;
    node_t *node = getNode("<R>",level);
    node->instance = (char *)malloc(24);

    if (tk->tk_Id == LftBracket_Tk) {   // if expr is [<expr>]
        scanner(filename,tk);
        node->child1 = expr(filename,tk,level,s,targtemp);
        if (tk->tk_Id == RgtBracket_Tk) {
            scanner(filename,tk);
            return node;
        }
        else
            errCond("R","]",tk->tk_inst,tk->line);
    }

    else if (tk->tk_Id == Identifier_Tk) {  // if expr is an identifier
        int num;
        char tempno[10]; 
        int tempnum = *targtemp;
        sprintf(tempno,"t%d",tempnum);

        temp = *tk;
        node->tok = temp;
        tempname = tk->tk_inst;
        tempname+=5;
        strcpy(node->instance,tempname);

        num = find(node->instance,s);
        if (num == -1) {
            printf("ERROR: var %s not found in scope, Line%d\n",node->instance,node->tok.line);
            exitAndDel(asmfile);
        }

        targetInstructNum(asmfile,"STACKR",num);
        targetInstructAlpha(asmfile,"STORE",tempno);
        *targtemp+=1;

        if (*targtemp > maximumTemp)    //updates maximumTemp global to allow for temp var storage after STOP
            maximumTemp = *targtemp;

        scanner(filename,tk);
        return node;
    }
    else if (tk->tk_Id == IntLiteral_Tk) {
        char tempno[10]; 
        int tempnum = *targtemp;
        sprintf(tempno,"t%d",tempnum);

        temp = *tk;
        node->tok = temp;
        tempname = tk->tk_inst;
        tempname+=4;
        strcpy(node->instance,tempname);

        targetInstructAlpha(asmfile, "LOAD", node->instance);
        targetInstructAlpha(asmfile, "STORE", tempno);
        *targtemp+=1;

        if (*targtemp > maximumTemp)
            maximumTemp = *targtemp;

        scanner(filename,tk);
        return node;
    }
    else
        errCond("R","[",tk->tk_inst,tk->line);
}

/* <stats> */
node_t *stats(char *filename, tlk *tk, int level, Stack *s, int scope) {
    level++;
    node_t *node = getNode("<stats>",level);

    node->child1 = stat(filename,tk,level,s,scope);
    node->child2 = mStat(filename,tk,level,s,scope);
    return node;
}

/* <mStats> */
node_t *mStat(char *filename, tlk *tk, int level, Stack *s, int scope) {
    level++;
    node_t *node = getNode("<mStat>",level);

    //prediction ??
    if (tk->tk_Id == Scan_Tk || tk->tk_Id == Prnt_Tk || tk->tk_Id == Bgn_Tk || tk->tk_Id == LftBracket_Tk || tk->tk_Id == Loop_Tk || tk->tk_Id == Identifier_Tk) {
        //don't consume the token yet'
        node->child1 = stat(filename,tk,level,s,scope);
        node->child2 = mStat(filename,tk,level,s,scope);
        return node;
    }
    else
        return NULL;
}

/* <stat> */
node_t *stat(char *filename, tlk *tk, int level, Stack *s, int scope) {
    level++;
    node_t *node = getNode("<stat>",level);

    if (tk->tk_Id == Scan_Tk) {
        //dont consume token yet
        node->child1 = in(filename,tk,level,s);
        return node;
    }
    else if (tk->tk_Id == Prnt_Tk) {
        node->child1 = out(filename,tk,level,s);
        return node;
    }
    else if (tk->tk_Id == Bgn_Tk) {
        int *varCount = (int *)malloc(sizeof(int));
        *varCount = 0;
       
        node->child1 = block(filename,tk,level,s,scope); 
        return node;
    }
    else if (tk->tk_Id == LftBracket_Tk) {
        node->child1 = if_(filename,tk,level,s,scope);
        return node;
    }
    else if (tk->tk_Id == Loop_Tk) {
        node->child1 = loop(filename,tk,level,s,scope);
        return node;
    }
    else if (tk->tk_Id == Identifier_Tk) {
        node->child1 = assign(filename,tk,level,s);
        return node;
    }
    else {
        printf("ERROR: <stat> expected tokens: Scan OR ID OR Loop OR Begin OR Print OR ( [ ) line:%d\n",tk->line);
        exitAndDel(asmfile);
    }
}

/* <in> */
node_t *in(char *filename, tlk *tk, int level, Stack *s) {
    level++;
    tlk temp;
    char *tempname;
    node_t *node = getNode("<in>",level);
    node->instance = (char *)malloc(24);

    if (tk->tk_Id == Scan_Tk) {
        scanner(filename,tk);   //finally consume here
        if (tk->tk_Id == Cln_Tk) {
            scanner(filename,tk);
            if (tk->tk_Id == Identifier_Tk) {
                int num;
                
                temp = *tk;
                node->tok = temp;
                tempname = tk->tk_inst;
                tempname+=5;
                strcpy(node->instance,tempname);

                num = find(node->instance,s);
                if (num == -1) {
                    printf("ERROR: var %s not found in scope, Line:%d\n",node->instance,node->tok.line);
                    exitAndDel(asmfile);
                }

                targetInstructAlpha(asmfile,"READ",node->instance);
                targetInstructAlpha(asmfile,"LOAD",node->instance);
                targetInstructNum(asmfile,"STACKW",num);

                scanner(filename,tk);

                if (tk->tk_Id == Period_Tk) {
                    scanner(filename,tk);
                    return node;
                }
                else
                    errCond("in",".",tk->tk_inst,tk->line);
            }
            else
                errCond("<in>","ID",tk->tk_inst,tk->line);
        }
        else
            errCond("<in>",":",tk->tk_inst,tk->line);
    }
    else
        errCond("<in>","Scan",tk->tk_inst,tk->line);
}

/* <out> */
node_t *out(char *filename, tlk *tk, int level, Stack *s) {
    node_t *node = getNode("<out>",level);
    int *targtemp = (int *)malloc(sizeof(int));
    *targtemp = 0;

    if (tk->tk_Id == Prnt_Tk) {
        scanner(filename,tk);
        if (tk->tk_Id == LftBracket_Tk) {
            scanner(filename,tk);
            
            node->child1 = expr(filename,tk,level,s, targtemp);
            
            targetInstructAlpha(asmfile,"WRITE","t0");  //might cause trouble?

            if (tk->tk_Id == RgtBracket_Tk) {
                scanner(filename,tk);
                if (tk->tk_Id == Period_Tk) {
                    scanner(filename,tk);
                    return node;
                }
                else
                    errCond("out",".",tk->tk_inst,tk->line);
            }
            else
                errCond("out","]",tk->tk_inst,tk->line);
        }
        else
            errCond("out","[",tk->tk_inst,tk->line);
    }
    else
        errCond("out","Print",tk->tk_inst,tk->line);
}

/* <if> */
node_t *if_(char *filename, tlk *tk, int level, Stack *s, int scope) {
    level++;

    node_t *node = getNode("<if>",level);

    int *targtemp = (int *)malloc(sizeof(int));
    *targtemp = 0;

    int *relop = (int *)malloc(sizeof(int));    //used to determine which relational op used

    if (tk->tk_Id == LftBracket_Tk) {
        scanner(filename,tk);
        node->child1 = expr(filename,tk,level,s,targtemp);
        node->child2 = RO(filename,tk,level,relop);
        node->child3 = expr(filename,tk,level,s,targtemp);

        char tempno[10];
        int tempnum = *targtemp-1;
        char outlabel[10];
        char condlabel[10];

        if (*relop == 0) {  //greater or equal to
            sprintf(tempno,"t%d",tempnum-1);
            targetInstructAlpha(asmfile,"LOAD",tempno);
            sprintf(tempno,"t%d",tempnum);
            targetInstructAlpha(asmfile,"SUB",tempno);
            sprintf(outlabel,"out%d",outcounter);
            targetInstructAlpha(asmfile,"BRNEG",outlabel);
            outcounter++;
        }

        if (*relop == 2) {  //greater than
            sprintf(tempno,"t%d",tempnum-1);
            targetInstructAlpha(asmfile,"LOAD",tempno);
            sprintf(tempno,"t%d",tempnum);
            targetInstructAlpha(asmfile,"SUB",tempno);
            sprintf(outlabel,"out%d",outcounter);
            targetInstructAlpha(asmfile,"BRZNEG",outlabel);
            outcounter++;
        }

        if (*relop == 1) {  //lesser or equal to
            sprintf(tempno,"t%d",tempnum-1);
            targetInstructAlpha(asmfile,"LOAD",tempno);
            sprintf(tempno,"t%d",tempnum);
            targetInstructAlpha(asmfile,"SUB",tempno);
            sprintf(outlabel,"out%d",outcounter);
            targetInstructAlpha(asmfile,"BRPOS",outlabel);
            outcounter++;
        }

        if (*relop == 3) {  //lesser than
            sprintf(tempno,"t%d",tempnum-1);
            targetInstructAlpha(asmfile,"LOAD",tempno);
            sprintf(tempno,"t%d",tempnum);
            targetInstructAlpha(asmfile,"SUB",tempno);
            sprintf(outlabel,"out%d",outcounter);
            targetInstructAlpha(asmfile,"BRZPOS",outlabel);
            outcounter++;
        }   

        if (*relop == 4) {  //equal to 
            sprintf(tempno,"t%d",tempnum-1);
            targetInstructAlpha(asmfile,"LOAD",tempno);
            sprintf(tempno,"t%d",tempnum);
            targetInstructAlpha(asmfile,"SUB",tempno);
            sprintf(condlabel,"cond%d",condcounter);
            targetInstructAlpha(asmfile,"BRZERO",condlabel);
            sprintf(outlabel,"out%d",outcounter);
            targetInstructAlpha(asmfile,"BR",outlabel);
            targetLabel(asmfile,condlabel);
            targetNoop(asmfile);
            outcounter++;
            condcounter++;
        }

        if (*relop == 5) {  //does not equal to
            sprintf(tempno,"t%d",tempnum-1);
            targetInstructAlpha(asmfile,"LOAD",tempno);
            sprintf(tempno,"t%d",tempnum);
            targetInstructAlpha(asmfile,"SUB",tempno);
            sprintf(outlabel,"out%d",outcounter);
            targetInstructAlpha(asmfile,"BRZERO",outlabel);
            outcounter++;
        }

        if (tk->tk_Id == RgtBracket_Tk) {
            scanner(filename,tk);
            if (tk->tk_Id == Iff_Tk) {
                int *varCount = (int *)malloc(sizeof(int));
                *varCount = 0;
                scanner(filename,tk);
                
                node->child4 = block(filename,tk,level,s,scope);

                targetLabel(asmfile,outlabel);
                targetNoop(asmfile);

                return node;
            }
            else
                errCond("if","Iff",tk->tk_inst,tk->line);
        }
        else
            errCond("if","]",tk->tk_inst,tk->line);
    }
    else
        errCond("if","[",tk->tk_inst,tk->line);
}

/* <loop> */
node_t *loop(char *filename, tlk *tk, int level, Stack *s, int scope) {
    level++;

    node_t *node = getNode("<loop>",level);

    int *targtemp = (int *)malloc(sizeof(int));
    *targtemp =0;

    int *relop = (int *)malloc(sizeof(int));
    *relop = 0;

    if (tk->tk_Id == Loop_Tk) {
        scanner(filename,tk);
        if (tk->tk_Id == LftBracket_Tk) {
            scanner(filename,tk);

            char looplabel[10];
            sprintf(looplabel,"loop%d",loopcounter);
            targetLabel(asmfile,looplabel);
            targetNoop(asmfile);

            node->child1 = expr(filename,tk,level,s,targtemp);
            node->child2 = RO(filename,tk,level,relop);
            node->child3 = expr(filename,tk,level,s, targtemp);

            char tempno[10];
            int tempnum = *targtemp-1;
            char outlabel[10];
            char condlabel[10];

            if (*relop == 0) {  //greater or equal to
                sprintf(tempno,"t%d",tempnum-1);
                targetInstructAlpha(asmfile,"LOAD",tempno);
                sprintf(tempno,"t%d",tempnum);
                targetInstructAlpha(asmfile,"SUB",tempno);
                sprintf(outlabel,"out%d",outcounter);
                targetInstructAlpha(asmfile,"BRNEG",outlabel); 
                outcounter++;
                loopcounter++;
            }

            if (*relop == 2) {  //greater than

                sprintf(tempno,"t%d",tempnum-1);
                targetInstructAlpha(asmfile,"LOAD",tempno);
                sprintf(tempno,"t%d",tempnum);
                targetInstructAlpha(asmfile,"SUB",tempno);
                sprintf(outlabel,"out%d",outcounter);
                targetInstructAlpha(asmfile,"BRZNEG",outlabel); 
                outcounter++;
                loopcounter++;
            }

            if (*relop == 1) {  //lesser or equal to
                sprintf(tempno,"t%d",tempnum-1);
                targetInstructAlpha(asmfile,"LOAD",tempno);
                sprintf(tempno,"t%d",tempnum);
                targetInstructAlpha(asmfile,"SUB",tempno);
                sprintf(outlabel,"out%d",outcounter);
                targetInstructAlpha(asmfile,"BRPOS",outlabel);    
                outcounter++;
                loopcounter++;
            }

            if (*relop == 3) {   //less than
                sprintf(tempno,"t%d",tempnum-1);
                targetInstructAlpha(asmfile,"LOAD",tempno);
                sprintf(tempno,"t%d",tempnum);
                targetInstructAlpha(asmfile,"SUB",tempno);
                sprintf(outlabel,"out%d",outcounter);
                targetInstructAlpha(asmfile,"BRZPOS",outlabel);
                outcounter++;
                loopcounter++;
            }

            if (*relop == 4) {  //equal to
                sprintf(tempno,"t%d",tempnum-1);
                targetInstructAlpha(asmfile,"LOAD",tempno);
                sprintf(tempno,"t%d",tempnum);
                targetInstructAlpha(asmfile,"SUB",tempno);
                sprintf(condlabel,"cond%d",condcounter);
                targetInstructAlpha(asmfile,"BRZERO",condlabel);
                sprintf(outlabel,"out%d",outcounter);
                targetInstructAlpha(asmfile,"BR",outlabel);
                targetLabel(asmfile,condlabel);
                targetNoop(asmfile);
                outcounter++;
                loopcounter++;
                condcounter++;
            }

            if (*relop == 5) {  //does not equal to
                sprintf(tempno,"t%d",tempnum-1);
                targetInstructAlpha(asmfile,"LOAD",tempno);
                sprintf(tempno,"t%d",tempnum);
                targetInstructAlpha(asmfile,"SUB",tempno);
                sprintf(outlabel,"out%d",outcounter);
                targetInstructAlpha(asmfile,"BRZERO",outlabel);
                outcounter++;
                loopcounter++;
            }

            if (tk->tk_Id == RgtBracket_Tk) {
                scanner(filename,tk);
                 
                node->child4 = block(filename,tk,level,s,scope);

                targetInstructAlpha(asmfile,"BR",looplabel);
                targetLabel(asmfile,outlabel);
                targetNoop(asmfile);

                return node;
            }
            else
                errCond("loop","]",tk->tk_inst,tk->line);
        }
        else 
            errCond("loop","[",tk->tk_inst,tk->line);
    }
    else
        errCond("loop","Loop",tk->tk_inst,tk->line);
}

/* <assign> */
node_t *assign(char *filename, tlk *tk, int level, Stack *s) {
    level++;
    tlk temp;
    tlk temp2;
    char *tempname;
    node_t *node = getNode("<assign>",level);
    node->instance = (char *)malloc(24);

    if (tk->tk_Id == Identifier_Tk) {
        int num;
        temp = *tk;
        node->tok = temp;
        tempname = tk->tk_inst;
        tempname+=5;
        strcpy(node->instance,tempname);

        num = find(node->instance,s);
        if (num == -1) {
            printf("ERROR: var %s not found in scope, Line%d\n",node->instance,node->tok.line);
            exitAndDel(asmfile);
        }
   
        scanner(filename,tk);
        if (tk->tk_Id == DblEq_Tk) {
            temp2 = *tk;
            node->tok2 = temp2;  //the only nonterminal with 2 semantic tokens
            
            scanner(filename,tk);
            
            int *targtemp = (int *)malloc(sizeof(int));
            *targtemp = 0;
            
            node->child1 = expr(filename,tk,level,s,targtemp);

            targetInstructAlpha(asmfile,"LOAD","t0");
            targetInstructNum(asmfile,"STACKW",num);
            
            if (tk->tk_Id == Period_Tk) {
                scanner(filename,tk);
                return node;
            }
            else   
                errCond("assign",".",tk->tk_inst,tk->line);
        }
        else   
            errCond("assign","==",tk->tk_inst,tk->line);
    }
    else    
        errCond("assign","ID",tk->tk_inst,tk->line);
}

/* <RO> */
node_t *RO(char *filename, tlk *tk, int level, int *relop) {
    level++;
    tlk temp;
    node_t *node = getNode("<RO>",level);
   
    if (tk->tk_Id == Gt_Eq_Tk) {
        temp = *tk;
        node->tok = temp;
        scanner(filename,tk);
        *relop = 0;
        return node;
    }
    else if (tk->tk_Id == Lt_Eq_Tk) {
        temp = *tk;
        node->tok = temp;
        scanner(filename,tk);
        *relop = 1;
        return node;
    }
    else if (tk->tk_Id == Eq_Tk) {
        temp = *tk;
        node->tok = temp;
        scanner(filename,tk);
        *relop = 4;
        return node;
    }
    else if (tk->tk_Id == Gt_Tk) {
        temp = *tk;
        node->tok = temp;
        scanner(filename,tk);
        *relop = 2;
        return node;
    }
    else if (tk->tk_Id == Lt_Tk) {
        temp = *tk;
        node->tok = temp;
        scanner(filename,tk);
        *relop = 3;
        return node;
    }
    else if (tk->tk_Id == DnEq_Tk) {
        temp = *tk;
        node->tok = temp;
        scanner(filename,tk);
        *relop = 5;
        return node;
    }
    else
        printf("ERROR: <RO> expected token >=> OR <=< OR = OR > OR < OR =!= line:%d\n",tk->line);
}

//          Error messaging, what nonterminal in BNF, the token expected before next token, and the line (approximation)
void errCond(char *nonterm, char *token,char *beforeTk, int line) {
    printf("<%s> Error: expected %s token before %s line:%d\n",nonterm,token,beforeTk,line);
    //exit(1);
    exitAndDel(asmfile);
}

void exitAndDel(char *filename) {
    remove(filename);
    exit(1);
}