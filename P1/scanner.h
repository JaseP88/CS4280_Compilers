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

extern char line_str[];
extern int line_num;
extern int state;
extern int savedState;
extern keys k;

int filter(char *filename, int line);
tlk scan(int index);

#endif