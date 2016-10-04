#include <stdio.h>
#include <stdlib.h>

#include "driver.h"
#include "token.h"
#include "scanner.h"
#include "testscanner.h"

#define BufferSize 256

/*
void testScan() {

	int i, j;
	int maxline;
	int maxchar;
	tlk token;

	// first find the max line of the file 
	maxline = findmaxLine("file.txt");

	for(i=0; i<maxline; i++) {
		filter("file.txt", i); //filter line i

		for (j=0; j<BufferSize; j++) {
			token = scan(j);
			
			if (token.wait != 1 && token.error != 1) {
				printf("tokenID:%d \n tokenInst:%s \n line:%d\n",token.tk_Id,token.tk_inst,token.line);
				j--;
			}

		}

	}

}
*/

void testScan() {

	tlk token;
	int i;
	int maxchar;

	filter("example.txt", 0);
	maxchar = findmaxChar();
	printf("max char is %d\n",maxchar);
	printf("%s\n",line_str);

	for(i=0; i<maxchar; i++) {
		token = scan(i);
		if (token.wait != 1 && token.error != 1) {
			printf("tokenID:%d \n tokenInst:%s \n line:%d\n",token.tk_Id,token.tk_inst,token.line);
			i--;
		}
	}
}


/* function to find the max line of the file */
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

