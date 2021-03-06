#ifndef PARSER_H
#define PARSER_H

#include "token.h"
#include "node.h"

void parser(char *filename);  //auxillary function
node_t *program(char *filename, tlk *tk);
node_t *block(char *filename, tlk *tk, int level);
node_t *vars(char *filename, tlk *tk, int level);
node_t *mvars(char *filename, tlk *tk, int level);
node_t *expr(char *filename, tlk *tk, int level);
node_t *rpxe(char *filename, tlk *tk, int level); //added to grammar
node_t *M(char *filename, tlk *tk, int level);
node_t *N(char *filename, tlk *tk, int level);    //added to grammar
node_t *T(char *filename, tlk *tk, int level);
node_t *X(char *filename, tlk *tk, int level);    //added to grammar
node_t *F(char *filename, tlk *tk, int level);
node_t *R(char *filename, tlk *tk, int level);
node_t *stats(char *filename, tlk *tk, int level);
node_t *mStat(char *filename, tlk *tk, int level);
node_t *stat(char *filename, tlk *tk, int level);
node_t *in(char *filename, tlk *tk, int level);
node_t *out(char *filename, tlk *tk, int level);
node_t *if_(char *filename, tlk *tk, int level);
node_t *loop(char *filename, tlk *tk, int level);
node_t *assign(char *filename, tlk *tk, int level);
node_t *RO(char *filename, tlk *tk, int level);

void errCond(char *nonterm, char *token, char *beforetk, int line);

#endif