#ifndef SCANNER_H
#define SCANNER_H


/* contains the keys that can be read */
enum {
	character, digit, WS, equal, excla, lftangle, rgtangle, colon, plus, minus, aster,
	bckslash, amper, percent, period, lftparen, rgtparen, comma, lftbrace, rgtbrace,
	semi, lftbracket, rgtbracket, dollar, pound, at, question, singleq, doubleq, underscore,
	fwdslash, tilde, bckquote, line 
};

void filter();
int scan(char *filename, int line);

#endif