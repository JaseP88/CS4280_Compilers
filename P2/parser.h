#ifndef PARSER_H
#define PARSER_H

#include "token.h"

void parser(char *filename);  //auxillary function
void program(char *filename, tlk *tk);
void block(char *filename, tlk *tk);
void vars(char *filename, tlk *tk);
void mvars(char *filename, tlk *tk);
void expr(char *filename, tlk *tk);
void rpxe(char *filename, tlk *tk); //added to grammar
void M(char *filename, tlk *tk);
void N(char *filename, tlk *tk);    //added to grammar
void T(char *filename, tlk *tk);
void X(char *filename, tlk *tk);    //added to grammar
void F(char *filename, tlk *tk);
void R(char *filename, tlk *tk);
void stats(char *filename, tlk *tk);
void mStat(char *filename, tlk *tk);
void stat(char *filename, tlk *tk);
void in(char *filename, tlk *tk);
void out(char *filename, tlk *tk);
void if_(char *filename, tlk *tk);
void loop(char *filename, tlk *tk);
void assign(char *filename, tlk *tk);
void RO(char *filename, tlk *tk);

#endif