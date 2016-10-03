#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "token.h"
#include "driver.h"
#include "scanner.h"

static int line_num;
static char line_str[1024];

void filter () {
	int i;

	for (i=0; i<sizeof(line_str); i++)
		if (line_str[i] == '\n')
			line_str[i] =' ';
		else if (line_str[i] == '#')
			line_str[i] = ' ';
}


int scan (char *filename, int line) {

	int count = 0;
	FILE *file;

	/* open the file */
	if ((file=fopen(filename,"r")) == NULL) {
		fprintf(stderr,"error opening file %s, check to see if file exists\n",filename);
		return -1;
	}

	/* reads specific line of a file */
	while (fgets(line_str, sizeof(line_str), file) != NULL) {

		if (count == line)
			break;
		else
			count++;
	}

	filter();

	printf("%s", line_str);

	fclose(file);
	return 0;
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