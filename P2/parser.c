#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "parser.h"
#include "scanner.h"
#include "token.h"
#include "node.h"
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
        printTree(root);
    }
}

/* <program> */
node_t *program(char *filename, tlk *tk) {
    int level = 0;
    node_t *node = getNode("<program>",level);  //create node for <program> and label it and also get level of the node
    
    node->child1 =  vars(filename,tk,level);
    node->child2  = block(filename,tk,level);
    return node;
}

/* <block> */
node_t *block(char *filename, tlk *tk, int level) {
    level++;
    node_t *node = getNode("<block>",level);   //create node for <block> and label it

    if (tk->tk_Id == Bgn_Tk) {  //structural token no need to store
        scanner(filename,tk);   //consume the token and get new token
        
        node->child1 = vars(filename,tk,level);
        node->child2 = stats(filename,tk,level);
       
        if (tk->tk_Id == End_Tk) {
            scanner(filename,tk);
            return node; 
        }
        else
            errCond("block","End",tk->tk_inst,tk->line);
    }
    else
        errCond("block","Begin",tk->tk_inst,tk->line);
        //printf("ERROR: <block> expected Begin token instead of %s --line:%d\n",tk->tk_inst,tk->line);
}

/* <vars> */
node_t *vars(char *filename, tlk *tk, int level) {
    level++;
    tlk temp;
    char *tempname;
    
    node_t *node = getNode("<vars>",level);
    node->instance = (char *)malloc(24);

    if (tk->tk_Id == Var_Tk) {
        scanner(filename,tk);
        if (tk->tk_Id == Identifier_Tk) {   //save this semantic token
            temp = *tk;
            node->tok = temp;
            
            tempname = tk->tk_inst;
            strcpy(node->instance,tempname);
           
            scanner(filename,tk);
            node->child1 = mvars(filename,tk,level);
            return node;
        }
        else
            errCond("vars","ID",tk->tk_inst,tk->line);
    }

    return NULL; //empty production
}

/* <mvars> */
node_t *mvars(char *filename, tlk *tk, int level) {
    level++;
    tlk temp;
    char *tempname;
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
                strcpy(node->instance,tempname);
                scanner(filename,tk);
                node->child1 = mvars(filename,tk,level);
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
node_t *expr(char *filename, tlk *tk, int level) {
    level++;
    node_t *node = getNode("<expr>",level);

    node->child1 = M(filename,tk,level);
    node->child2 = rpxe(filename,tk,level);
    return node;
}

/* <rpxe> backwards HAH */
node_t *rpxe(char *filename, tlk *tk, int level) {
    tlk temp;
    level++;
    node_t *node = getNode("<rpxe>",level);
  
    if (tk->tk_Id == Plus_Tk) {
        temp = *tk;
        node->tok = temp;
        scanner(filename,tk);
        node->child1 = expr(filename,tk,level);
        return node;
    }
    
    return NULL;
}

/* <M> */
node_t *M(char *filename, tlk *tk, int level) {
    level++;
    node_t *node = getNode("<M>",level);

    node->child1 = T(filename,tk,level);
    node->child2 = N(filename,tk,level);
    return node;
}

/* <N> */
node_t *N(char *filename, tlk *tk, int level) {
    level++;
    tlk temp;
    node_t *node = getNode("<N>",level);

    if (tk->tk_Id == Minus_Tk) {
        temp = *tk;
        node->tok = temp;
        scanner(filename,tk);
        node->child1 = M(filename,tk,level);
        return node;
    }
    
    return NULL;
}

/* <T> */
node_t *T(char *filename, tlk *tk, int level) {
    level++;
    node_t *node = getNode("<T>",level);
    node->child1 = F(filename,tk,level);
    node->child2 = X(filename,tk,level);
    return node;
}

/* <X> */ 
node_t *X(char *filename, tlk *tk, int level) {
    level++;
    tlk temp;
    node_t *node = getNode("<X>",level);

    if (tk->tk_Id == Ast_Tk) {
        temp = *tk;
        node->tok = temp;
        scanner(filename,tk);
        node->child1 = T(filename,tk,level);
        return node;
    }
    else if (tk->tk_Id == BkSlash_Tk) {
        temp = *tk;
        node->tok = temp;
        scanner(filename,tk);
        node->child1 = T(filename,tk,level);
        return node;
    }
     
    return NULL;
}

/* <F> */
node_t *F(char *filename, tlk *tk, int level) {
    level++;
    tlk temp;
    node_t *node = getNode("<F>",level);

    if (tk->tk_Id == Minus_Tk) {
        temp = *tk;
        node->tok = temp;
        scanner(filename,tk);
        node->child1 = F(filename,tk,level);
        return node;
    }
    else 
        node->child1 = R(filename,tk,level);

    return node;
}

/* <R> */
node_t *R(char *filename, tlk *tk, int level) {
    level++;
    tlk temp;
    char *tempname;
    node_t *node = getNode("<R>",level);
    node->instance = (char *)malloc(24);

    if (tk->tk_Id == LftBracket_Tk) {
        scanner(filename,tk);
        node->child1 = expr(filename,tk,level);
     
        if (tk->tk_Id == RgtBracket_Tk) {
            scanner(filename,tk);
            return node;
        }
        else
            errCond("R","]",tk->tk_inst,tk->line);
    }
    else if (tk->tk_Id == Identifier_Tk) {
        temp = *tk;
        node->tok = temp;
        tempname = tk->tk_inst;
        strcpy(node->instance,tempname);
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
node_t *stats(char *filename, tlk *tk, int level) {
    level++;
    node_t *node = getNode("<stats>",level);

    node->child1 = stat(filename,tk,level);
    node->child2 = mStat(filename,tk,level);
    return node;
}

/* <mStats> */
node_t *mStat(char *filename, tlk *tk, int level) {
    level++;
    node_t *node = getNode("<mStat>",level);

    //prediction ??
    if (tk->tk_Id == Scan_Tk || tk->tk_Id == Prnt_Tk || tk->tk_Id == Bgn_Tk || tk->tk_Id == LftBracket_Tk || tk->tk_Id == Loop_Tk || tk->tk_Id == Identifier_Tk) {
        //don't consume the token yet'
        node->child1 = stat(filename,tk,level);
        node->child2 = mStat(filename,tk,level);
        return node;
    }
    else
        return NULL;
}

/* <stat> */
node_t *stat(char *filename, tlk *tk, int level) {
    level++;
    node_t *node = getNode("<stat>",level);

    if (tk->tk_Id == Scan_Tk) {
        //dont consume token yet
        node->child1 = in(filename,tk,level);
        return node;
    }
    else if (tk->tk_Id == Prnt_Tk) {
        node->child1 = out(filename,tk,level);
        return node;
    }
    else if (tk->tk_Id == Bgn_Tk) {
        node->child1 = block(filename,tk,level);
        return node;
    }
    else if (tk->tk_Id == LftBracket_Tk) {
        node->child1 = if_(filename,tk,level);
        return node;
    }
    else if (tk->tk_Id == Loop_Tk) {
        node->child1 = loop(filename,tk,level);
        return node;
    }
    else if (tk->tk_Id == Identifier_Tk) {
        node->child1 = assign(filename,tk,level);
        return node;
    }
    else {
        printf("ERROR: <stat> expected tokens: Scan OR ID OR Loop OR Begin OR Print OR ( [ ) line:%d\n",tk->line);
        exit(1);
    }
}

/* <in> */
node_t *in(char *filename, tlk *tk, int level) {
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
                temp = *tk;
                node->tok = temp;
                tempname = tk->tk_inst;
                strcpy(node->instance,tempname);
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
node_t *out(char *filename, tlk *tk, int level) {
    node_t *node = getNode("<out>",level);

    if (tk->tk_Id == Prnt_Tk) {
        scanner(filename,tk);
        if (tk->tk_Id == LftBracket_Tk) {
            scanner(filename,tk);
            node->child1 = expr(filename,tk,level);
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
node_t *if_(char *filename, tlk *tk, int level) {
    level++;
    node_t *node = getNode("<if>",level);

    if (tk->tk_Id == LftBracket_Tk) {
        scanner(filename,tk);
        node->child1 = expr(filename,tk,level);
        node->child2 = RO(filename,tk,level);
        node->child3 = expr(filename,tk,level);
        if (tk->tk_Id == RgtBracket_Tk) {
            scanner(filename,tk);
            if (tk->tk_Id == Iff_Tk) {
                scanner(filename,tk);
                node->child4 = block(filename,tk,level);
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
node_t *loop(char *filename, tlk *tk, int level) {
    level++;
    node_t *node = getNode("<loop>",level);

    if (tk->tk_Id == Loop_Tk) {
        scanner(filename,tk);
        if (tk->tk_Id == LftBracket_Tk) {
            scanner(filename,tk);
            node->child1 = expr(filename,tk,level);
            node->child2 = RO(filename,tk,level);
            node->child3 = expr(filename,tk,level);
            if (tk->tk_Id == RgtBracket_Tk) {
                scanner(filename,tk);
                node->child4 = block(filename,tk,level);
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
node_t *assign(char *filename, tlk *tk, int level) {
    level++;
    tlk temp;
    tlk temp2;
    char *tempname;
    node_t *node = getNode("<assign>",level);
    node->instance = (char *)malloc(24);

    if (tk->tk_Id == Identifier_Tk) {
        temp = *tk;
        node->tok = temp;
        tempname = tk->tk_inst;
        strcpy(node->instance,tempname);
        scanner(filename,tk);
        if (tk->tk_Id == DblEq_Tk) {
            temp2 = *tk;
            node->tok2 = temp2;  //the only nonterminal with 2 semantic tokens
            scanner(filename,tk);
            node->child1 = expr(filename,tk,level);
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