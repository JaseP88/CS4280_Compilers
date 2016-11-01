#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "testscanner.h"
#include "scanner.h"

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
		char *extension = ".txt";	//change to fsf
		char *extension2 = ".fs16";
		strcpy(file, filename);
		strcat(file,extension);
		testScanner(file);
		//parser(file);
		//testScan(file);
	}	

	/* read from keyboard */
	else 
		//testScan2();
	

	return 0;
}