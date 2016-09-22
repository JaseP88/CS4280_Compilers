#include <stdio.h>
#include <stdlib.h>
#include "node.h"
#include "traversals.h"


/*
These are functions to print the values and its counter from the 
tree structure into a file.  Can print the values in an
inorder/preorder/postorder format with offsets that correlates
with its level
*/

void inorder (node *tree, char *filename) {

	if (tree == NULL)
		return;

	else {
		
		inorder(tree->left, filename);

		FILE *fp;
		if ((fp=fopen(filename,"a")) == NULL) {
			fprintf(stderr,"error opening file %s\n",filename);
			return;
		}

		fprintf(fp, "%*c%d:%-9d\n",tree->level*2,' ',tree->value,tree->counter);
		fclose(fp);
		
		inorder(tree->right, filename);
	}

}


void preorder (node *tree, char *filename) {

	if (tree == NULL)
		return;

	else {

		FILE *fp;
		if ((fp=fopen(filename,"a")) == NULL) {
			fprintf(stderr,"error opening file %s\n",filename);
			return;
		}

		//fprintf(fp,"%d:%d\n",tree->value, tree->counter);
		fprintf(fp, "%*c%d:%-9d\n",tree->level*2,' ',tree->value,tree->counter);
		fclose(fp);
		
		preorder(tree->left, filename);
		preorder(tree->right, filename);
	}

}


void postorder (node *tree, char *filename) {

	if (tree == NULL)
		return;

	else {
		postorder(tree->left, filename);
		postorder(tree->right, filename);
		//printf("%d ",tree->value);

		FILE *fp;
		if ((fp=fopen(filename,"a")) == NULL) {
			fprintf(stderr,"error opening file %s\n",filename);
			return;
		}

		fprintf(fp, "%*c%d:%-9d\n",tree->level*2,' ',tree->value,tree->counter);
		fclose(fp);
	}

}
