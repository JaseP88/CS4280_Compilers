#include <stdio.h>
#include <stdlib.h>

#include "codegen.h"

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

void targetStop(char *filename) {
	FILE *fp;
    if ((fp=fopen(filename,"a")) == NULL) {
		fprintf(stderr,"error opening file %s\n",filename);
		exit(1);
	}

	fprintf(fp, "STOP\n");
	fclose(fp);
}

void targetNoop(char *filename) {
	FILE *fp;
    if ((fp=fopen(filename,"a")) == NULL) {
		fprintf(stderr,"error opening file %s\n",filename);
		exit(1);
	}

	fprintf(fp, "NOOP\n");
	fclose(fp);
}

void targetLabel(char *filename, char *label) {
	FILE *fp;
    if ((fp=fopen(filename,"a")) == NULL) {
		fprintf(stderr,"error opening file %s\n",filename);
		exit(1);
	}

	fprintf(fp, "%s: ",label);
	fclose(fp);
}

void targetVarInit(char *filename, char *var, int num) {
	FILE *fp;
    if ((fp=fopen(filename,"a")) == NULL) {
		fprintf(stderr,"error opening file %s\n",filename);
		exit(1);
	}

	fprintf(fp, "%s %d\n",var,num);
	fclose(fp);
}

void targetTempInit(char *filename, char *tempvar) {
	FILE *fp;
    if ((fp=fopen(filename,"a")) == NULL) {
		fprintf(stderr,"error opening file %s\n",filename);
		exit(1);
	}

	fprintf(fp, "%s 0\n",tempvar);
	fclose(fp);
}

void targetInstructAlpha(char *filename, char *instruction, char *alpha) {
	FILE *fp;
	if ((fp=fopen(filename,"a")) == NULL) {
		fprintf(stderr,"error opening file %s\n",filename);
		exit(1);
	}

	fprintf(fp, "%s %s\n",instruction,alpha);
	fclose(fp);
}

void targetInstructNum(char *filename, char *instruction, int num) {
	FILE *fp;
	if ((fp=fopen(filename,"a")) == NULL) {
		fprintf(stderr,"error opening file %s\n",filename);
		exit(1);
	}

	fprintf(fp, "%s %d\n",instruction,num);
	fclose(fp);
}









void targetStackR(char *filename, int pos) {
	FILE *fp;
	if ((fp=fopen(filename,"a")) == NULL) {
		fprintf(stderr,"error opening file %s\n",filename);
		exit(1);
	}

	fprintf(fp, "STACKR %d\n",pos);
	fclose(fp);
}

void targetStackW(char *filename, int pos) {
	FILE *fp;
	if ((fp=fopen(filename,"a")) == NULL) {
		fprintf(stderr,"error opening file %s\n",filename);
		exit(1);
	}

	fprintf(fp, "STACKW %d\n",pos);
	fclose(fp);
}

void targetLoadNUM(char *filename, int num) {
	FILE *fp;
	if ((fp=fopen(filename,"a")) == NULL) {
		fprintf(stderr,"error opening file %s\n",filename);
		exit(1);
	}

	fprintf(fp, "LOAD %d\n",num);
	fclose(fp);
}

void targetLoadAL(char *filename, char *alpha) {
	FILE *fp;
	if ((fp=fopen(filename,"a")) == NULL) {
		fprintf(stderr,"error opening file %s\n",filename);
		exit(1);
	}

	fprintf(fp, "LOAD %s\n",alpha);
	fclose(fp);
}

void targetStoreAL(char *filename, char *alpha) {
	FILE *fp;
	if ((fp=fopen(filename,"a")) == NULL) {
		fprintf(stderr,"error opening file %s\n",filename);
		exit(1);
	}

	fprintf(fp, "STORE %s\n",alpha);
	fclose(fp);
}

void targetStoreNUM(char *filename, int num) {
	FILE *fp;
	if ((fp=fopen(filename,"a")) == NULL) {
		fprintf(stderr,"error opening file %s\n",filename);
		exit(1);
	}

	fprintf(fp, "STORE %d\n",num);
	fclose(fp);
}

