#include <stdio.h>
#include <stdlib.h>

#include "parser.h"
#include "scanner.h"
#include "token.h"
#include "node.h"

/* auxillary function  <*/
node_t *parser(char *filename) {
    tlk *token = (tlk *)malloc(sizeof(tlk));    //create token in heap memory
    node_t *root;

    scanner(filename,token); //retrieve token
    root = program(filename,token); //call program

    //printf("THE TOKEN AFTER ALL THATS DONE IS tokenID:%d--%s--%d\n",token->tk_Id,token->tk_inst,token->line);
    if (token->tk_Id != Eof_Tk) 
        errCond("parser","EOF",token->line);    //prints error messages and exits
    
    else {
        printf("parser complete and is A-OK\n");
        return root;
    }
}

/* <program> */
node_t *program(char *filename, tlk *tk) {
    node_t *node = getNode("program");  //create node for <program> and label it 
    node->child1 =  vars(filename,tk);
    node->child2  = block(filename,tk);
    //node->child3 = NULL;
    return node;
}

/* <block> */
node_t *block(char *filename, tlk *tk) {
    node_t *node = getNode("<block>");   //create node for <block> and label it

    if (tk->tk_Id == Bgn_Tk) {  //structural token no need to store
        scanner(filename,tk);   //consume the token and get new token
        
        node->child1 = vars(filename,tk);
        node->child2 = stats(filename,tk);
       
        if (tk->tk_Id == End_Tk) {
            scanner(filename,tk);
            return node; 
        }
        else
            errCond("block","End",tk->line);
    }
    else
        errCond("block","Begin",tk->line);
        //printf("ERROR: <block> expected Begin token instead of %s --line:%d\n",tk->tk_inst,tk->line);
}

/* <vars> */
node_t *vars(char *filename, tlk *tk) {
    node_t *node = getNode("<vars>");

    if (tk->tk_Id == Var_Tk) {
        scanner(filename,tk);
        if (tk->tk_Id == Identifier_Tk) {   //save this semantic token
            node->token = tk;               //saving.....
            scanner(filename,tk);
            node->child1 = mvars(filename,tk);
            return node;
        }
        else
            errCond("vars","ID",tk->line);
    }
    else 
        return NULL; //empty production
}

/* <mvars> */
node_t *mvars(char *filename, tlk *tk) {
    node_t *node = getNode("<mvars>");

    if (tk->tk_Id == Cln_Tk) {
        scanner(filename,tk);
        if (tk->tk_Id == Cln_Tk) {
            scanner(filename,tk);
            if (tk->tk_Id == Identifier_Tk) {
                node->token = tk;
                scanner(filename,tk);
                node->child1 = mvars(filename,tk);
                return node;
            }
            else
                errCond("mvars","ID",tk->line);
        }
        else
            errCond("mvars",":",tk->line);
    }
    else
        return NULL; //empty production
}

/* <expr> */
node_t *expr(char *filename, tlk *tk) {
    node_t *node = getNode("<expr>");
    node->child1 = M(filename,tk);
    node->child2 = rpxe(filename,tk);
    return node;
}

/* <rpxe> backwards HAH */
node_t *rpxe(char *filename, tlk *tk) {
    node_t *node = getNode("<rpxe>");
    if (tk->tk_Id == Plus_Tk) {
        node->token = tk;
        scanner(filename,tk);
        node->child1 = expr(filename,tk);
        return node;
    }
    else
        return NULL;
}

/* <M> */
node_t *M(char *filename, tlk *tk) {
    node_t *node = getNode("<M>");
    node->child1 = T(filename,tk);
    node->child2 = N(filename,tk);
    return node;
}

/* <N> */
node_t *N(char *filename, tlk *tk) {
    node_t *node = getNode("<N>");
    if (tk->tk_Id == Minus_Tk) {
        node->token = tk;
        scanner(filename,tk);
        node->child1 = M(filename,tk);
        return node;
    }
    else
        return NULL;
}

/* <T> */
node_t *T(char *filename, tlk *tk) {
    node_t *node = getNode("<T>");
    node->child1 = F(filename,tk);
    node->child2 = X(filename,tk);
    return node;
}

/* <X> */
node_t *X(char *filename, tlk *tk) {
    node_t *node = getNode("<X>");
    if (tk->tk_Id == Ast_Tk) {
        node->token = tk;
        scanner(filename,tk);
        node->child1 = T(filename,tk);
        return node;
    }
    else if (tk->tk_Id == BkSlash_Tk) {
        node->token = tk;
        scanner(filename,tk);
        node->child1 = T(filename,tk);
        return node;
    }
    else 
        return NULL;
}

/* <F> */
node_t *F(char *filename, tlk *tk) {
    node_t *node = getNode("<F>");
    if (tk->tk_Id == Minus_Tk) {
        node->token = tk;
        scanner(filename,tk);
        node->child1 = F(filename,tk);
        return node;
    }
    else 
        node->child1 = R(filename,tk);
    return node;
}

/* <R> */
node_t *R(char *filename, tlk *tk) {
    node_t *node = getNode("<R>");
    if (tk->tk_Id == LftBracket_Tk) {
        scanner(filename,tk);
        node->child1 = expr(filename,tk);
        if (tk->tk_Id == RgtBracket_Tk) {
            scanner(filename,tk);
            return node;
        }
        else
            errCond("R","]",tk->line);
    }
    else if (tk->tk_Id == Identifier_Tk) {
        node->token = tk;
        scanner(filename,tk);
        return node;
    }
    else if (tk->tk_Id == IntLiteral_Tk) {
        node->token = tk;
        scanner(filename,tk);
        return node;
    }
    else
        errCond("R","[",tk->line);
}

/* <stats> */
node_t *stats(char *filename, tlk *tk) {
    node_t *node = getNode("<stats>");
    node->child1 = stat(filename,tk);
    node->child2 = mStat(filename,tk);
    return node;
}

/* <mStats> */
node_t *mStat(char *filename, tlk *tk) {
    node_t *node = getNode("<mStat>");
    //prediction ??
    if (tk->tk_Id == Scan_Tk || tk->tk_Id == Prnt_Tk || tk->tk_Id == Bgn_Tk || tk->tk_Id == LftBracket_Tk || tk->tk_Id == Loop_Tk || tk->tk_Id == Identifier_Tk) {
        //don't consume the token yet'
        node->child1 = stat(filename,tk);
        node->child2 = mStat(filename,tk);
        return node;
    }
    else
        return NULL;
}

/* <stat> */
node_t *stat(char *filename, tlk *tk) {
    node_t *node = getNode("<stat>");
    if (tk->tk_Id == Scan_Tk) {
        //dont consume token yet
        node->child1 = in(filename,tk);
        return node;
    }
    else if (tk->tk_Id == Prnt_Tk) {
        node->child1 = out(filename,tk);
        return node;
    }
    else if (tk->tk_Id == Bgn_Tk) {
        node->child1 = block(filename,tk);
        return node;
    }
    else if (tk->tk_Id == LftBracket_Tk) {
        node->child1 = if_(filename,tk);
        return node;
    }
    else if (tk->tk_Id == Loop_Tk) {
        node->child1 = loop(filename,tk);
        return node;
    }
    else if (tk->tk_Id == Identifier_Tk) {
        node->child1 = assign(filename,tk);
        return node;
    }
    else {
        printf("ERROR: <stat> expected tokens: Scan OR ID OR Loop OR Begin OR Print OR ( [ ) line:%d\n",tk->line);
        exit(1);
    }
}

/* <in> */
node_t *in(char *filename, tlk *tk) {
    node_t *node = getNode("<in>");
    if (tk->tk_Id == Scan_Tk) {
        scanner(filename,tk);   //finally consume here
        if (tk->tk_Id == Cln_Tk) {
            scanner(filename,tk);
            if (tk->tk_Id == Identifier_Tk) {
                node->token = tk;
                scanner(filename,tk);
                if (tk->tk_Id == Period_Tk) {
                    scanner(filename,tk);
                    return node;
                }
                else
                    errCond("in",".",tk->line);
            }
            else
                errCond("<in>","ID",tk->line);
        }
        else
            errCond("<in>",":",tk->line);
    }
    else
        errCond("<in>","Scan",tk->line);
}

/* <out> */
node_t *out(char *filename, tlk *tk) {
    node_t *node = getNode("<out>");
    if (tk->tk_Id == Prnt_Tk) {
        scanner(filename,tk);
        if (tk->tk_Id == LftBracket_Tk) {
            scanner(filename,tk);
            node->child1 = expr(filename,tk);
            if (tk->tk_Id == RgtBracket_Tk) {
                scanner(filename,tk);
                if (tk->tk_Id == Period_Tk) {
                    scanner(filename,tk);
                    return node;
                }
                else
                    errCond("out",".",tk->line);
            }
            else
                errCond("out","]",tk->line);
        }
        else
            errCond("out","[",tk->line);
    }
    else
        errCond("out","Print",tk->line);
}

/* <if> */
node_t *if_(char *filename, tlk *tk) {
    node_t *node = getNode("<if>");
    if (tk->tk_Id == LftBracket_Tk) {
        scanner(filename,tk);
        node->child1 = expr(filename,tk);
        node->child2 = RO(filename,tk);
        node->child3 = expr(filename,tk);
        if (tk->tk_Id == RgtBracket_Tk) {
            scanner(filename,tk);
            if (tk->tk_Id == Iff_Tk) {
                scanner(filename,tk);
                node->child4 = block(filename,tk);
                return node;
            }
            else
                errCond("if","Iff",tk->line);
        }
        else
            errCond("if","]",tk->line);
    }
    else
        errCond("if","[",tk->line);
}

/* <loop> */
node_t *loop(char *filename, tlk *tk) {
    node_t *node = getNode("<loop>");
    if (tk->tk_Id == Loop_Tk) {
        scanner(filename,tk);
        if (tk->tk_Id == LftBracket_Tk) {
            scanner(filename,tk);
            node->child1 = expr(filename,tk);
            node->child2 = RO(filename,tk);
            node->child3 = expr(filename,tk);
            if (tk->tk_Id == RgtBracket_Tk) {
                scanner(filename,tk);
                node->child4 = block(filename,tk);
                return node;
            }
            else
                errCond("loop","]",tk->line);
        }
        else 
            errCond("loop","[",tk->line);
    }
    else
        errCond("loop","Loop",tk->line);
}

/* <assign> */
node_t *assign(char *filename, tlk *tk) {
    node_t *node = getNode("<assign>");
    if (tk->tk_Id == Identifier_Tk) {
        node->token = tk;
        scanner(filename,tk);
        if (tk->tk_Id == DblEq_Tk) {
            node->token = tk;
            scanner(filename,tk);
            node->child1 = expr(filename,tk);
            if (tk->tk_Id == Period_Tk) {
                scanner(filename,tk);
                return node;
            }
            else   
                errCond("assign",".",tk->line);
        }
        else   
            errCond("assign","==",tk->line);
    }
    else    
        errCond("assign","ID",tk->line);
}

/* <RO> */
node_t *RO(char *filename, tlk *tk) {
    node_t *node = getNode("<RO>");
    if (tk->tk_Id == Gt_Eq_Tk) {
        node->token = tk;
        scanner(filename,tk);
        return node;
    }
    else if (tk->tk_Id == Lt_Eq_Tk) {
        node->token = tk;
        scanner(filename,tk);
        return node;
    }
    else if (tk->tk_Id == Eq_Tk) {
        node->token = tk;
        scanner(filename,tk);
        return node;
    }
    else if (tk->tk_Id == Gt_Tk) {
        node->token = tk;
        scanner(filename,tk);
        return node;
    }
    else if (tk->tk_Id == Lt_Tk) {
        node->token = tk;
        scanner(filename,tk);
        return node;
    }
    else if (tk->tk_Id == DnEq_Tk) {
        node->token = tk;
        scanner(filename,tk);
        return node;
    }
    else
        printf("ERROR: <RO> expected token >=> OR <=< OR = OR > OR < OR =!= line:%d\n",tk->line);
}

//          Error messaging, what nonterminal in BNF, the token expected, and the line (approximation)
void errCond(char *nonterm, char *token, int line) {
    printf("<%s> Error: expected %s token line:%d\n",nonterm,token,line);
    exit(1);
}