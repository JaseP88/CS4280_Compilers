#ifndef TABLE_H
#define TABLE_H

#include "token.h"
#include "scanner.h"

#define state_	30
#define keys_ 24



typedef enum state {
s1, s2, s3, s4, s5, s6, s7, s8, s9, s10,
s11, s12, s13, s14, s15, s16, s17, s18, s19, s20,
s21, s22, s23, s24, s25, s26, s27, s28, s29, s30
} state_t;

//add error codes here

// FSA Table
extern int table[state_][keys_];


tlk FADriver(int st, int key); //driver prototype


#endif