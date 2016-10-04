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

	// first find the max line of the file 
	maxline = findmaxLine("file.txt");

	for(i=0; i<maxline; i++) {
		filter("file.txt", i); //filter line i

		for (j=0; j<BufferSize; j++) {
			scan(j);

		}


	}

}
*/

void testScan() {

	tlk token;

	filter("example.txt", 0);
	token = scan(0);
	if (token.wait != 1 && token.error != 1) {
		printf("tokenID:%d \n tokenInst:%s \n line:%d\n",token.tk_Id,token.tk_inst,token.line);
	}
	else
		printf("token error or wait\n");
	
	token = scan(1);
	if (token.wait != 1 && token.error != 1) {
		printf("tokenID:%d \n tokenInst:%s \n line:%d\n",token.tk_Id,token.tk_inst,token.line);
	}
	else
		printf("token error or wait\n");

	token = scan(2);
	if (token.wait != 1 && token.error != 1) {
		printf("tokenID:%d \n tokenInst:%s \n line:%d\n",token.tk_Id,token.tk_inst,token.line);
	}
	else
		printf("token error or wait\n");
}

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