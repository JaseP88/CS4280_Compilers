#include <stdio.h>
#include <stdlib.h>

#include "parser.h"
#include "scanner.h"
#include "token.h"
#include "node.h"

/* auxillary function  <*/
void parser(char *filename) {
    tlk *token = (tlk *)malloc(sizeof(tlk));    //create token in heap memory

    scanner(filename,token); //retrieve token
    program(filename,token); //call program
    //scanner(filename,token);

    printf("THE TOKEN AFTER ALL THATS DONE IS tokenID:%d--%s--%d\n",token->tk_Id,token->tk_inst,token->line);

    if (token->tk_Id != Eof_Tk) {
        printf("ERROR: <parser> expected EOF token, EOF token not found\n");
    }
    else
        printf("parser complete and is A-OK\n");
}

/* <program> */
void program(char *filename, tlk *tk) {
   vars(filename,tk);
   block(filename,tk);
   return;
}

/* <block> */
void block(char *filename, tlk *tk) {
    if (tk->tk_Id == Bgn_Tk) {
        scanner(filename,tk);
        vars(filename,tk);
        stats(filename,tk);
        if (tk->tk_Id == End_Tk) {
            scanner(filename,tk);
            return; //
        }
        else
            printf("ERROR: <block> expected End token instead of %s --line:%d\n",tk->tk_inst,tk->line);
    }
    else
        printf("ERROR: <block> expected Begin token instead of %s --line:%d\n",tk->tk_inst,tk->line);
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
            printf("ERROR: <vars> expected Id token after Var instead of %s --line:%d\n",tk->tk_inst,tk->line);
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
                printf("ERROR: <mvars> expected Id token instead of %s --line:%d\n",tk->tk_inst,tk->line);
        }
        else
            printf("ERROR: <mvars> expected second colon( : ) token instead of %s --line:%d\n",tk->tk_inst,tk->line);
    }
    else
        return; //empty production
}

/* <expr> */
void expr(char *filename, tlk *tk) {
    M(filename,tk);
    rpxe(filename,tk);
    return;
}

/* <rpxe> backwards HAH */
void rpxe(char *filename, tlk *tk) {
    if (tk->tk_Id == Plus_Tk) {
        scanner(filename,tk);
        expr(filename,tk);
        return;
    }
    else
        return;
}

/* <M> */
void M(char *filename, tlk *tk) {
    T(filename,tk);
    N(filename,tk);
    return;
}

/* <N> */
void N(char *filename, tlk *tk) {
    if (tk->tk_Id == Minus_Tk) {
        scanner(filename,tk);
        M(filename,tk);
        return;
    }
    else
        return;
}

/* <T> */
void T(char *filename, tlk *tk) {
    F(filename,tk);
    X(filename,tk);
    return;
}

/* <X> */
void X(char *filename, tlk *tk) {
    if (tk->tk_Id == Ast_Tk) {
        scanner(filename,tk);
        T(filename,tk);
        return;
    }
    else if (tk->tk_Id == BkSlash_Tk) {
        scanner(filename,tk);
        T(filename,tk);
        return;
    }
    else 
        return;
}

/* <F> */
void F(char *filename, tlk *tk) {
    if (tk->tk_Id == Minus_Tk) {
        scanner(filename,tk);
        F(filename,tk);
        return;
    }
    else 
        R(filename,tk);
    return;
}

/* <R> */
void R(char *filename, tlk *tk) {
    if (tk->tk_Id == LftBracket_Tk) {
        scanner(filename,tk);
        expr(filename,tk);
        if (tk->tk_Id == RgtBracket_Tk) {
            scanner(filename,tk);
            return;
        }
        else
            printf("ERROR: <R> expected Right Bracket Token ( ] ) instead of %s --line:%d\n",tk->tk_inst,tk->line);
    }
    else if (tk->tk_Id == Identifier_Tk) {
        scanner(filename,tk);
        return;
    }
    else if (tk->tk_Id == IntLiteral_Tk) {
        scanner(filename,tk);
        return;
    }
    else
        printf("ERROR: <R> expected ( [ ) OR Id OR # Token instead of %s --line:%d\n",tk->tk_inst,tk->line);
}

/* <stats> */
void stats(char *filename, tlk *tk) {
    stat(filename,tk);
    mStat(filename,tk);
    return;
}

/* <mStats> */
void mStat(char *filename, tlk *tk) {
    //prediction ??
    if (tk->tk_Id == Scan_Tk || tk->tk_Id == Prnt_Tk || tk->tk_Id == Bgn_Tk || tk->tk_Id == LftBracket_Tk || tk->tk_Id == Loop_Tk || tk->tk_Id == Identifier_Tk) {
        //don't consume the token yet'
        stat(filename,tk);
        mStat(filename,tk);
        return;
    }
    else
        return;
}

/* <stat> */
void stat(char *filename, tlk *tk) {
    if (tk->tk_Id == Scan_Tk) {
        //dont consume token yet
        in(filename,tk);
        return;
    }
    else if (tk->tk_Id == Prnt_Tk) {
        out(filename,tk);
        return;
    }
    else if (tk->tk_Id == Bgn_Tk) {
        block(filename,tk);
        return;
    }
    else if (tk->tk_Id == LftBracket_Tk) {
        if_(filename,tk);
        return;
    }
    else if (tk->tk_Id == Loop_Tk) {
        loop(filename,tk);
        return;
    }
    else if (tk->tk_Id == Identifier_Tk) {
        assign(filename,tk);
        return;
    }
    else
        printf("ERROR: <stat> expected tokens: Scan OR ID OR Loop OR Begin OR Print OR ( [ ) instead of %s --line:%d\n",tk->tk_inst,tk->line);
}

/* <in> */
void in(char *filename, tlk *tk) {
    if (tk->tk_Id == Scan_Tk) {
        scanner(filename,tk);   //finally consume here
        if (tk->tk_Id == Cln_Tk) {
            scanner(filename,tk);
            if (tk->tk_Id == Identifier_Tk) {
                scanner(filename,tk);
                if (tk->tk_Id == Period_Tk) {
                    scanner(filename,tk);
                    return;
                }
                else
                    printf("ERROR: <in> expected token ( . ) instead of %s --line:%d\n",tk->tk_inst,tk->line);
            }
            else
                printf("ERROR: <in> expected token ID instead of %s --line:%d\n",tk->tk_inst,tk->line);
        }
        else
            printf("ERROR: <in> expected token ( : ) instead of %s --line:%d\n",tk->tk_inst,tk->line);
    }
    else
        printf("ERROR: <in> expected token Scan instead of %s --line:%d\n",tk->tk_inst,tk->line);
}

/* <out> */
void out(char *filename, tlk *tk) {
    if (tk->tk_Id == Prnt_Tk) {
        scanner(filename,tk);
        if (tk->tk_Id == LftBracket_Tk) {
            scanner(filename,tk);
            expr(filename,tk);
            if (tk->tk_Id == RgtBracket_Tk) {
                scanner(filename,tk);
                if (tk->tk_Id == Period_Tk) {
                    scanner(filename,tk);
                    return;
                }
                else
                    printf("ERROR: <out> expected token ( . ) instead of %s --line:%d\n",tk->tk_inst,tk->line);
            }
            else
                printf("ERROR: <out> expected token ( ] ) instead of %s --line:%d\n",tk->tk_inst,tk->line);
        }
        printf("ERROR: <out> expected token ( [ ) instead of %s --line:%d\n",tk->tk_inst,tk->line);
    }
    printf("ERROR: <out> expected token Print instead of %s --line:%d\n",tk->tk_inst,tk->line);
}

/* <if> */
void if_(char *filename, tlk *tk) {
    if (tk->tk_Id == LftBracket_Tk) {
        scanner(filename,tk);
        expr(filename,tk);
        RO(filename,tk);
        expr(filename,tk);
        if (tk->tk_Id == RgtBracket_Tk) {
            scanner(filename,tk);
            if (tk->tk_Id == Iff_Tk) {
                scanner(filename,tk);
                block(filename,tk);
                return;
            }
            else
                printf("ERROR: <if> expected token Iff instead of %s --line:%d\n",tk->tk_inst,tk->line);
        }
        else
            printf("ERROR: <if> expected token ( ] ) instead of %s --line:%d\n",tk->tk_inst,tk->line);
    }
    else
        printf("ERROR: <if> expected token ( [ ) instead of %s --line:%d\n",tk->tk_inst,tk->line);
}

/* <loop> */
void loop(char *filename, tlk *tk) {
    if (tk->tk_Id == Loop_Tk) {
        scanner(filename,tk);
        if (tk->tk_Id == LftBracket_Tk) {
            scanner(filename,tk);
            expr(filename,tk);
            RO(filename,tk);
            expr(filename,tk);
            if (tk->tk_Id == RgtBracket_Tk) {
                scanner(filename,tk);
                block(filename,tk);
                return;
            }
            else
                printf("ERROR: <loop> expected token ( ] ) instead of %s --line:%d\n",tk->tk_inst,tk->line);
        }
        else 
            printf("ERROR: <loop> expected token  ( [ ) instead of %s --line:%d\n",tk->tk_inst,tk->line);
    }
    else    
        printf("ERROR: <loop> expected token Loop instead of %s --line:%d\n",tk->tk_inst,tk->line);
}

/* <assign> */
void assign(char *filename, tlk *tk) {
    if (tk->tk_Id == Identifier_Tk) {
        scanner(filename,tk);
        if (tk->tk_Id == DblEq_Tk) {
            scanner(filename,tk);
            expr(filename,tk);
            if (tk->tk_Id == Period_Tk) {
                scanner(filename,tk);
                return;
            }
            else   
                printf("ERROR: <assign> expected token ( . ) instead of %s --line:%d\n",tk->tk_inst,tk->line);
        }
        else   
            printf("ERROR: <assign> expected token ( == ) instead of %s --line:%d\n",tk->tk_inst,tk->line);
    }
    else    
        printf("ERROR: <assign> expected token ID instead of %s --line:%d\n",tk->tk_inst,tk->line);
}

/* <RO> */
void RO(char *filename, tlk *tk) {
    if (tk->tk_Id == Gt_Eq_Tk) {
        scanner(filename,tk);
        return;
    }
    else if (tk->tk_Id == Lt_Eq_Tk) {
        scanner(filename,tk);
        return;
    }
    else if (tk->tk_Id == Eq_Tk) {
        scanner(filename,tk);
        return;
    }
    else if (tk->tk_Id == Gt_Tk) {
        scanner(filename,tk);
        return;
    }
    else if (tk->tk_Id == Lt_Tk) {
        scanner(filename,tk);
        return;
    }
    else if (tk->tk_Id == DnEq_Tk) {
        scanner(filename,tk);
        return;
    }
    else
        printf("ERROR: <RO> expected token >=> OR <=< OR = OR > OR < OR =!= instead of %s --line:%d\n",tk->tk_inst,tk->line);
}