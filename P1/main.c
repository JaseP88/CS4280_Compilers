#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "testscanner.h"

#define filenameSize 50


int main (int argc, char *argv[]) {

	/* if more than two argument error */
	if (argc > 2) {
		printf ("too many arguments\n");
	}


	/* if file is provided in argument */
	else if (argc == 2) {

		char file[filenameSize];
		char *filename = argv[1];
		char *extension = ".txt";

		strcpy(file, filename);
		strcat(file,extension);

		testScan(file);
	}	

	/* read from keyboard */
	else {



	}

	return 0;
}