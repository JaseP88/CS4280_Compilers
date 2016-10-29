#include <stdio.h>
#include <stdlib.h>

#include "scanner.h"
#include "token.h"
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
				
				/* if the token is EOF break out of the back tracing route */
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

void testScan2() {
	int maxchar, j;
	tlk token;
	line_num = 0;
	while (fgets(line_str, 256, stdin) != NULL) {
		line_num++;
		maxchar = findmaxChar();
		filter2();
		for (j=0; j<maxchar; j++) {
			token = scan(j);

			
			if (token.wait != 1 && token.error != 1) {
				printf("tokenID:%d\n--tokenInst:%s\n--line:%d\n\n",token.tk_Id,token.tk_inst,token.line);
				
				/* if the token is EOF break out of the back tracing route */
				if (token.tk_Id == 0) {
					printf("EOF reached\n");
					break;
				}

				else
					j--;	//back tracing
			}

		}

	}

	maxchar = findmaxChar();	//finds max char of last line
	line_str[maxchar] = -1;		//add the EOF signal to end the stream
	token = scan(maxchar);
	printf("tokenID:%d\n--tokenInst:%s\n--line:%d\n\n",token.tk_Id, token.tk_inst,token.line);
}
