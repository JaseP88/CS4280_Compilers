#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "node.h"
#include "buildtree.h"
#include "traversals.h"

int main (int argc, char *argv[]) {


	//check if there are more than 1 argument used
	if (argc > 2) {
		printf ("too many arguments\n");
	}

	//if there is an argument use argument as a filename
	else if (argc == 2) {

		int n;	//n is the value being read from keyboard/file

		int lev = 0;
		node *root = NULL;
		FILE *fp;

		//allows files to have a max of 49 characters
		int nameSize = 50;

		char input_file[nameSize];
		char output_in[nameSize];
		char output_pre[nameSize];
		char output_post[nameSize];

		//get the filename from the argument
		char *filename = argv[1];

		//add file extensions here
		char *extension1 = ".data";
		char *extension2 = ".inorder";
		char *extension3 = ".preorder";
		char *extension4 = ".postorder";
		
		//append the extensions to the filename from argument
		strcpy(input_file, filename);
		strcat(input_file, extension1);

		strcpy(output_in, filename);
		strcat(output_in, extension2);

		strcpy(output_pre, filename);
		strcat(output_pre, extension3);

		strcpy(output_post, filename);
		strcat(output_post, extension4);

		if ((fp=fopen(input_file,"r")) == NULL) {
			fprintf(stderr,"error opening file %s, check to see if file exists\n",input_file);
			return -1;
		}

		while (fscanf(fp, "%d ", &n) != EOF)
			root = insert(root, n, lev);

		inorder(root, output_in);
		preorder(root, output_pre);
		postorder(root, output_post);

		fclose(fp);

	}
	
	//if no argument is used read from keyboard
	else {
		
		int n;

		int lev = 0;
		node *root = NULL;
		
		printf("input values, to quit press cntrl+d to simulate eof\n");
	
		while (scanf("%d ", &n) != EOF)
			root = insert(root, n, lev);

		inorder(root,"out.inorder");
		preorder(root,"out.preorder");
		postorder(root,"out.postorder");

	}

	printf("program done\n");

return 0;	
}
