#ifndef PARSER_H
#define PARSER_H

#include "token.h"
#include "node.h"
#include "stack.h"

void parser(char *filename);  //auxillary function
node_t *program(char *filename, tlk *tk);
node_t *block(char *filename, tlk *tk, int level, Stack *s, int scope);
node_t *vars(char *filename, tlk *tk, int level, int *valcount, Stack *s, int scope);
node_t *mvars(char *filename, tlk *tk, int level, int *valcount, Stack *s, int scope);


node_t *expr(char *filename, tlk *tk, int level, Stack *s, int *targtemp);
node_t *rpxe(char *filename, tlk *tk, int level, Stack *s, int *targtemp); //added to grammar
node_t *M(char *filename, tlk *tk, int level, Stack *s, int *targtemp);
node_t *N(char *filename, tlk *tk, int level, Stack *s, int *targtemp);    //added to grammar
node_t *T(char *filename, tlk *tk, int level, Stack *s, int *targtemp);
node_t *X(char *filename, tlk *tk, int level, Stack *s, int *targtemp);    //added to grammar
node_t *F(char *filename, tlk *tk, int level, Stack *s, int *targtemp);
node_t *R(char *filename, tlk *tk, int level, Stack *s, int *targtemp);


node_t *stats(char *filename, tlk *tk, int level,Stack *s, int scope);
node_t *mStat(char *filename, tlk *tk, int level, Stack *s, int scope);
node_t *stat(char *filename, tlk *tk, int level, Stack *s, int scope);
node_t *in(char *filename, tlk *tk, int level, Stack *s);
node_t *out(char *filename, tlk *tk, int level, Stack *s);
node_t *if_(char *filename, tlk *tk, int level, Stack *s, int scope);
node_t *loop(char *filename, tlk *tk, int level, Stack *s, int scope);
node_t *assign(char *filename, tlk *tk, int level, Stack *s);


node_t *RO(char *filename, tlk *tk, int level);

void errCond(char *nonterm, char *token, char *beforetk, int line);
void exitAndDel(char *filename);

#endif