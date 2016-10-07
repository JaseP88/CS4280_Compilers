#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "token.h"
#include "driver.h"
#include "scanner.h"

#define Buffersize 256	//define the buffersize a line

int line_num = 0;
char line_str[Buffersize];

int state = s1; //set state to initial also keep track of currState
keys k = 0;


/* this method stores the read line into an array buffer line_str
	and filters out specific keys, changing it to WS
	filter returns 1 if fine -1 if error */
int filter (char *filename, int line) {

	int i, cursor;
	int count = 0;
	FILE *file;

	/* open the file */
	if ((file=fopen(filename,"r")) == NULL) {
		fprintf(stderr,"error opening file %s, check to see if file exists\n",filename);
		return -1;
	}

	/* reads specific line of a file and store in string buffer 
		fgets adds /0 at the end, reads at least size-1 */
	while (fgets(line_str, sizeof(line_str), file) != NULL) {

		if (count == line)
			break;
		else
			count++;
	}
	
	/* filters out # and newlines and comments*/
	for (i=0; i<sizeof(line_str); i++) {
		if (line_str[i] == '@') {
			line_str[i] = ' ';
			cursor = i+1;
			while (1) {
				if (line_str[cursor] != ' ') {
					line_str[cursor] = ' ';
					//printf("%s\n", line_str); //testprintf
					cursor++;
				}
				else
					break;
			}
			
		}
	
	}
	//printf("%s\n", line_str); //testprintf

	fclose(file);
	return 1;
}


tlk scan(int index) {
	tlk token;

	/* switch case to determine the keys to use for FSAtable*/
	switch (line_str[index]) {
		case 'a': case 'b': case 'c': case 'd': case 'e':
		case 'f': case 'g': case 'h': case 'i': case 'j':
		case 'k': case 'l': case 'm': case 'n': case 'o':
		case 'p': case 'q': case 'r': case 's': case 't':
		case 'u': case 'v': case 'w': case 'x': case 'y':
		case 'z':
		case 'A': case 'B': case 'C': case 'D': case 'E':
		case 'F': case 'G': case 'H': case 'I': case 'J':
		case 'K': case 'L': case 'M': case 'N': case 'O':
		case 'P': case 'Q': case 'R': case 'S': case 'T':
		case 'U': case 'V': case 'W': case 'X': case 'Y':
		case 'Z':
			k = letter;
			break;
		
		case '1': case '2': case '3': case '4':case '5': 
		case '6': case '7':case '8': case '9':
		case '0':
			k = digit;
			break;
		
		case '\0':
			k = end;
			break;
		case ' ':
			k = WS;
			break;
		case '=':
			k = equal;
			break;
		case '!':
			k = excla;
			break;
		case '<':
			k = lftangle;
			break;
		case '>':
			k = rgtangle;
			break;
		case ':':
			k = colon;
			break;
		case '+':
			k = plus;
			break;
		case '-':
			k = minus;
			break;
		case '*':
			k = aster;
			break;
		case '/':
			k = bckslash;
			break;
		case '&':
			k = amper;
			break;
		case '%':
			k = percent;
			break;
		case '.':
			k = period;
			break;
		case '(':
			k = lftparen;
			break;
		case ')':
			k = rgtparen;
			break;
		case ',':
			k = comma;
			break;
		case '{':
			k = lftbrace;
			break;
		case  '}':
			k = rgtbrace;
			break;
		case ';':
			k = semi;
			break;
		case '[':
			k = lftbracket;
			break;
		case ']':
			k = rgtbracket;
			break;
		case '\n':
			line_str[index] = ' ';	//pass WS to Driver method
			k = WS;
			break;
		default:
			fprintf(stderr,"Error key not found\n");
	}

	//call driver here
	token = FADriver(state,k,line_str[index]);
	return token;
}

