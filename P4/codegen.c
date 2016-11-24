#include <stdio.h>
#include <stdlib.h>

#include "genASM.h"

void targetPush(char *filename) {
    FILE *fp;
	if ((fp=fopen(filename,"a")) == NULL) {
		fprintf(stderr,"error opening file %s\n",filename);
		exit(1);
	}

	fprintf(fp, "PUSH\n");
	fclose(fp);
}

void targetPop(char *filename) {
    FILE *fp;
    if ((fp=fopen(filename,"a")) == NULL) {
		fprintf(stderr,"error opening file %s\n",filename);
		exit(1);
	}

	fprintf(fp, "POP\n");
	fclose(fp);
}