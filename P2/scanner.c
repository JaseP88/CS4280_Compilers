#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "token.h"
#include "driver.h"
#include "scanner.h"

#define Buffersize 256	//define the buffersize a line

int line_num;
char line_str[Buffersize];

int state = s1; //set state to initial also keep track of currState
keys k = 0;

/* these 2 variables keep track of current line and current index 
	for Scanner() */
int line_position = 0;
int index_position = 0;

/* this method stores the read line into an array buffer line_str
	and filters out specific keys, changing it to WS
	filter returns 1 if fine -1 if error */
int filter (char *filename, int line) {

	int maxline, maxchar;
	int i, cursor, count;
	FILE *file;

	maxline = findmaxLine(filename);
	line_num = line+1;
	count = 0;
	

	/* open the file */
	if ((file=fopen(filename,"r")) == NULL) {
		fprintf(stderr,"error opening file %s, check to see if file exists\n",filename);
		return -1;
	}

	/* reads specific line of a file and store in string buffer 
		fgets adds /0 at the end, reads at least size-1 */
	while (fgets(line_str, sizeof(line_str), file) != NULL) {

		if (count == line)	//line_str stores the specific line
			break;
		else
			count++;
	}
	
	/* filters out comments that starts with @ */
	for (i=0; i<sizeof(line_str); i++) {
		if (line_str[i] == '@') {
			line_str[i] = ' ';
			cursor = i+1;
			while (1) {
				
				if (line_str[cursor] == ' ' || line_str[cursor] == '\n' || line_str[cursor] == '\0') 
					break;
				
				else {
					line_str[cursor] = ' ';
					cursor++;
				}
			}
			
		}
	
	}

	/* simulates an EOF at the end of the file */
	if (line == maxline-1) {
		maxchar = findmaxChar();
		line_str[maxchar] = -1;
	}

	fclose(file);
	return 1;
}

void filter2() {
	int cursor, i;

	for (i=0; i<sizeof(line_str); i++) {
		if (line_str[i] == '@') {
			line_str[i] = ' ';
			cursor = i+1;
			while (1) {
				
				if (line_str[cursor] == ' ' || line_str[cursor] == '\n' || line_str[cursor] == '\0') 
					break;
				
				else {
					line_str[cursor] = ' ';
					cursor++;
				}
			}
			
		}
	
	}

}

/* Scanner function return 1 complete token at a time */
tlk Scanner (char *filename) {
	int maxchar;
	tlk token;

	/* filters the line in file and remove comments and get max char in that line */
	filter (filename, line_position);
	maxchar = findmaxChar();
	
	while (index_position < maxchar) {

		if (index_position == maxchar-1) {	//if the index position is near the end of string buffer
			token = scan(index_position);	
			break;		
		}

		else {
			token = scan(index_position);
			index_position++;
			if (token.wait != 1 && token.error != 1) {
				index_position--;
				return token;
			}
		}
	}
	index_position = 0;	//reset the index to the string buffer here
	line_position++;	//set a new line position
	if (token.wait == 1 || token.error == 1) {
		printf("Error: Extra WS detected at End Of the Line: line #%d \n",line_position);
		exit(1);
	}
	
	return token;
}


/* scan function: scans each the stored characters in line_str buffer and pass
	the define key and character value to the FSAdriver method */
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
		
		case -1:	//eof value
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
			line_str[index] = ' ';	//pass WS to Driver methods if newline is detected
			k = WS;
			break;
		case '\0':
			line_str[index] = ' ';
			k = WS;
			break;
		default:
			fprintf(stderr,"Error key not found. Characters %c not in alphabet: line #%d\n",line_str[index], line_num);
			exit(1);
	}

	//call driver here
	token = FADriver(state,k,line_str[index]);
	return token;
}

/* function to find the max line of the file being read */
int findmaxLine (char *filename) {

	FILE *fp;
	char ch;
	int lines = 0;

	if ((fp=fopen(filename,"r")) == NULL) {
		fprintf(stderr,"error opening file %s, check to see if file exists\n",filename);
		return -1;
	}

	while (!feof(fp)) {
    	ch =  fgetc(fp);
    	if (ch == '\n')
      		lines++;
  	}

  fclose(fp);
  
  return lines;
}

/* function to find the max character in line_str buffer */
int findmaxChar () {

	int i;
	int counter = 0;

	for (i=0; i<Buffersize; i++) {
		
		if (line_str[i] != '\0') {
			counter++;
		}	
		
		else {
			break;
		}
	}
	return counter;
}
