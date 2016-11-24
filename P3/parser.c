#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "parser.h"
#include "scanner.h"
#include "token.h"
#include "node.h"
#include "stack.h"
#include "treePrint.h"


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

    /* CREATE THE STACK FOR THE PROGRAM */
    Stack *stack = (Stack *)malloc(sizeof(Stack));
    initStack(stack);
    
    printf("%*cGlobal Vars:\n",1,' ');
    node->child1 =  vars(filename,tk,level,varCount,stack,scope); 
    printf("%*cLocal Vars\n",3,' ');  
    node->child2  = block(filename,tk,level,stack,scope);
    
/* Maybe no need to pop global scopes???   
    printf("Global Scope End\n");
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
    *varCount = 0;  //init set the varCount to 0

    
    if (tk->tk_Id == Bgn_Tk) {  //structural token no need to store
        scanner(filename,tk);   //consume the token and get new token
        
        node->child1 = vars(filename,tk,level,varCount,s,scope);
        node->child2 = stats(filename,tk,level,s,scope);
   
        if (tk->tk_Id == End_Tk) {
            //pop the instance of each block when done
            for (i=0; i<*varCount; i++) 
                pop(s);
            
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
            tempname+=5;
            strcpy(node->instance,tempname);
            
            //originally varcount is 0 in all vars()
            push(node->instance,s);
            *varcount+=1;   //increment the varcount for this identifier
            printf("%*c%s, ", scope*3, ' ', top(s));
            
            scanner(filename,tk);
            node->child1 = mvars(filename,tk,level,varcount,s,scope);   //pass the varcount to mvars
            printf("\n\n");
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
                        *varcount+=1;
                        printf("%s, ", top(s));
                    }
                    else if (number < *varcount) {
                        printf("\nERROR: multiple identifiers. %s already in scope, line %d\n",node->instance,temp.line);
                        exit(1);
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
node_t *expr(char *filename, tlk *tk, int level, Stack *s) {
    level++;
    node_t *node = getNode("<expr>",level);

    node->child1 = M(filename,tk,level,s);
    node->child2 = rpxe(filename,tk,level,s);
    return node;
}

/* <rpxe> backwards HAH */
node_t *rpxe(char *filename, tlk *tk, int level, Stack *s) {
    tlk temp;
    level++;
    node_t *node = getNode("<rpxe>",level);
  
    if (tk->tk_Id == Plus_Tk) {
        temp = *tk;
        node->tok = temp;
        scanner(filename,tk);
        node->child1 = expr(filename,tk,level,s);
        return node;
    }
    
    return NULL;
}

/* <M> */
node_t *M(char *filename, tlk *tk, int level, Stack *s) {
    level++;
    node_t *node = getNode("<M>",level);

    node->child1 = T(filename,tk,level,s);
    node->child2 = N(filename,tk,level,s);
    return node;
}

/* <N> */
node_t *N(char *filename, tlk *tk, int level, Stack *s) {
    level++;
    tlk temp;
    node_t *node = getNode("<N>",level);

    if (tk->tk_Id == Minus_Tk) {
        temp = *tk;
        node->tok = temp;
        scanner(filename,tk);
        node->child1 = M(filename,tk,level,s);
        return node;
    }
    
    return NULL;
}

/* <T> */
node_t *T(char *filename, tlk *tk, int level, Stack *s) {
    level++;
    node_t *node = getNode("<T>",level);
    node->child1 = F(filename,tk,level,s);
    node->child2 = X(filename,tk,level,s);
    return node;
}

/* <X> */ 
node_t *X(char *filename, tlk *tk, int level, Stack *s) {
    level++;
    tlk temp;
    node_t *node = getNode("<X>",level);

    if (tk->tk_Id == Ast_Tk) {
        temp = *tk;
        node->tok = temp;
        scanner(filename,tk);
        node->child1 = T(filename,tk,level,s);
        return node;
    }
    else if (tk->tk_Id == BkSlash_Tk) {
        temp = *tk;
        node->tok = temp;
        scanner(filename,tk);
        node->child1 = T(filename,tk,level,s);
        return node;
    }
     
    return NULL;
}

/* <F> */
node_t *F(char *filename, tlk *tk, int level, Stack *s) {
    level++;
    tlk temp;
    node_t *node = getNode("<F>",level);

    if (tk->tk_Id == Minus_Tk) {
        temp = *tk;
        node->tok = temp;
        scanner(filename,tk);
        node->child1 = F(filename,tk,level,s);
        return node;
    }
    else 
        node->child1 = R(filename,tk,level,s);

    return node;
}

/* <R> */
node_t *R(char *filename, tlk *tk, int level, Stack *s) {
    level++;
    tlk temp;
    char *tempname;
    node_t *node = getNode("<R>",level);
    node->instance = (char *)malloc(24);

    if (tk->tk_Id == LftBracket_Tk) {
        scanner(filename,tk);
        node->child1 = expr(filename,tk,level,s);
     
        if (tk->tk_Id == RgtBracket_Tk) {
            scanner(filename,tk);
            return node;
        }
        else
            errCond("R","]",tk->tk_inst,tk->line);
    }
    else if (tk->tk_Id == Identifier_Tk) {
        int num;
        temp = *tk;
        node->tok = temp;
        tempname = tk->tk_inst;
        tempname+=5;
        strcpy(node->instance,tempname);

        num = find(node->instance,s);
        if (num == -1) {
            printf("ERROR: var %s not found in scope, Line%d\n",node->instance,node->tok.line);
            exit (1);
        }

        scanner(filename,tk);
        return node;
    }
    else if (tk->tk_Id == IntLiteral_Tk) {
        temp = *tk;
        node->tok = temp;
        tempname = tk->tk_inst;
        strcpy(node->instance,tempname);
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
        printf("%*cLocal Vars\n",(scope+1)*3,' ');  
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
        exit(1);
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
                    exit(1);
                }

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

    if (tk->tk_Id == Prnt_Tk) {
        scanner(filename,tk);
        if (tk->tk_Id == LftBracket_Tk) {
            scanner(filename,tk);
            node->child1 = expr(filename,tk,level,s);
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

    if (tk->tk_Id == LftBracket_Tk) {
        scanner(filename,tk);
        node->child1 = expr(filename,tk,level,s);
        node->child2 = RO(filename,tk,level);
        node->child3 = expr(filename,tk,level,s);
        if (tk->tk_Id == RgtBracket_Tk) {
            scanner(filename,tk);
            if (tk->tk_Id == Iff_Tk) {
                int *varCount = (int *)malloc(sizeof(int));
                *varCount = 0;
                scanner(filename,tk);
                printf("%*cIff Scope:\n",(scope+1)*3,' ');
                node->child4 = block(filename,tk,level,s,scope);
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

    if (tk->tk_Id == Loop_Tk) {
        scanner(filename,tk);
        if (tk->tk_Id == LftBracket_Tk) {
            scanner(filename,tk);
            node->child1 = expr(filename,tk,level,s);
            node->child2 = RO(filename,tk,level);
            node->child3 = expr(filename,tk,level,s);
            if (tk->tk_Id == RgtBracket_Tk) {
                scanner(filename,tk);
                 printf("%*cLoop Scope:\n",(scope+1)*3,' ');
                node->child4 = block(filename,tk,level,s,scope);
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

        if (find(node->instance,s) == -1) {
            printf("ERROR: var %s not found in scope, Line%d\n",node->instance,node->tok.line);
            exit (1);
        }

        scanner(filename,tk);
        if (tk->tk_Id == DblEq_Tk) {
            temp2 = *tk;
            node->tok2 = temp2;  //the only nonterminal with 2 semantic tokens
            scanner(filename,tk);
            node->child1 = expr(filename,tk,level,s);
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
node_t *RO(char *filename, tlk *tk, int level) {
    level++;
    tlk temp;
    node_t *node = getNode("<RO>",level);
   
    if (tk->tk_Id == Gt_Eq_Tk) {
        temp = *tk;
        node->tok = temp;
        scanner(filename,tk);
        return node;
    }
    else if (tk->tk_Id == Lt_Eq_Tk) {
        temp = *tk;
        node->tok = temp;
        scanner(filename,tk);
        return node;
    }
    else if (tk->tk_Id == Eq_Tk) {
        temp = *tk;
        node->tok = temp;
        scanner(filename,tk);
        return node;
    }
    else if (tk->tk_Id == Gt_Tk) {
        temp = *tk;
        node->tok = temp;
        scanner(filename,tk);
        return node;
    }
    else if (tk->tk_Id == Lt_Tk) {
        temp = *tk;
        node->tok = temp;
        scanner(filename,tk);
        return node;
    }
    else if (tk->tk_Id == DnEq_Tk) {
        temp = *tk;
        node->tok = temp;
        scanner(filename,tk);
        return node;
    }
    else
        printf("ERROR: <RO> expected token >=> OR <=< OR = OR > OR < OR =!= line:%d\n",tk->line);
}

//          Error messaging, what nonterminal in BNF, the token expected before next token, and the line (approximation)
void errCond(char *nonterm, char *token,char *beforeTk, int line) {
    printf("<%s> Error: expected %s token before %s line:%d\n",nonterm,token,beforeTk,line);
    exit(1);
}