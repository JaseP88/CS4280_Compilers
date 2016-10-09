#include <stdio.h>
#include <stdlib.h>

#include "scanner.h"
#include "testscanner.h"

#define BufferSize 256


void testScan(char *filename) {

	int i, j;
	int maxline;
	int maxchar;
	tlk token;

	// first find the max line of the file 
	maxline = findmaxLine(filename);
	//printf("maxline is %d\n",maxline);

	for(i=0; i<maxline; i++) {
		filter(filename, i); //filter line i
		maxchar = findmaxChar();
		//printf("maxchar is %d\n",maxchar);

		for (j=0; j<maxchar; j++) {
			token = scan(j);

			
			if (token.wait != 1 && token.error != 1) {
				printf("tokenID:%d\n--tokenInst:%s\n--line:%d\n\n",token.tk_Id,token.tk_inst,token.line);
				
				if (token.tk_Id == 0) {
					printf("EOF reached\n");
					break;
				}
				
				else
					j--;	//back tracing
			}

		}

	}

}

/*
void testScan(char *filename) {

	tlk token;
	int i;
	int maxchar;
	int maxline;

	//put line loop here
	filter(filename, 0);
	maxchar = findmaxChar();
	//printf("max char is %d\n",maxchar);
	//printf("%s\n",line_str);

	for(i=0; i<maxchar; i++) {
		token = scan(i);
		if (token.wait != 1 && token.error != 1) {
			printf("tokenID:%d \n tokenInst:%s \n line:%d\n\n",token.tk_Id,token.tk_inst,token.line);
			i--;
		}
	}
}
*/

/* function to find the max line of the file being read *//*
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
}*/

/* function to find the max character in line_str buffer *//*
int findmaxChar () {

	int i;
	int counter = 0;

	for (i=0; i<BufferSize; i++) {
		
		if (line_str[i] != '\0') {
			counter++;
		}	
		
		else {
			break;
		}
	}
	return counter;
}
*/
