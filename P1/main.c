#include <stdio.h>
#include <stdlib.h>
#include "scanner.h"
#include "driver.h"
#include "token.h"


int main (int argc, char *argv[]) {

	scan("file.txt", 0);
	printf("\n");
	printf ("in main %d\n",table[1][2]);
	printf ("in main %s\n",tokenString[1]);

	return 0;
}