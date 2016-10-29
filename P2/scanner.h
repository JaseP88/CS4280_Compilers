#ifndef SCANNER_H
#define SCANNER_H

#include "token.h"
#include "driver.h"

/* contains the keys that can be read */
typedef enum keys{
	letter, digit, end, WS, equal, excla, lftangle, rgtangle, colon,
	plus, minus, aster, bckslash, amper, percent, period, lftparen,
	rgtparen, comma, lftbrace, rgtbrace, semi, lftbracket, rgtbracket,
	dollar, pound, at, question, singleq, doubleq, underscore,
	fwdslash, tilde, bckquote, vertical
} keys;

extern char line_str[];	//string buffer that holds all the text in a line. Size 256
extern int line_num;
extern int state;
extern keys k;
extern int line_position;
extern int index_position;

int filter(char *filename, int line);
void filter2();
tlk scan(int index);
tlk Scanner(char *filename);
int findmaxLine (char *filename);
int findmaxChar();


#endif