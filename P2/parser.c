#include <stdio.h>
#include <stdlib.h>

#include "parser.h"
#include "scanner.h"
#include "token.h"

/* auxillary function  <*/
void parser(char *filename) {
    tlk *token = (tlk *)malloc(sizeof(tlk));    //create token in heap memory

    scanner(filename,token); //retrieve token
    program(filename,token); //call program
    scanner(filename,token);

    if (token->tk_Id != Eof_Tk) {
        printf("ERROR: <parser> expected EOF token, EOF token not found\n");
    }
    else
        printf("parser complete and is A-OK\n");
}

/* <program> */
void program(char *filename, tlk *tk) {
   vars(filename,tk);
   //block(filename,tk);
   return;
}

/* <vars> */
void vars(char *filename, tlk *tk) {
    if (tk->tk_Id == Var_Tk) {
        scanner(filename,tk);
        if (tk->tk_Id == Identifier_Tk) {
            scanner(filename,tk);
            mvars(filename,tk);
            return;
        }
        else
            printf("ERROR: <vars> expected Id token after Var token --line:%d inst:%s\n",tk->line, tk->tk_inst);
    }
    else 
        return; //empty production
}

/* <mvars> */
void mvars(char *filename, tlk *tk) {
    if (tk->tk_Id == Cln_Tk) {
        scanner(filename,tk);
        if (tk->tk_Id == Cln_Tk) {
            scanner(filename,tk);
            if (tk->tk_Id == Identifier_Tk) {
                scanner(filename,tk);
                mvars(filename,tk);
                return;
            }
            else
                printf("ERROR: <mvars> expected Id token --line:%d inst:%s\n",tk->line,tk->tk_inst);
        }
        else
            printf("ERROR: <mvars> expected second colon(:) token --line:%d inst:%s\n",tk->line,tk->tk_inst);
    }
    else
        return; //empty production
}