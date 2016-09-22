#include <stdlib.h>
#include <stdio.h>
#include "buildtree.h"
#include "node.h"

/*
insert a new node into a tree structure with
a value, level, left child, right child property
*/
node *insert (node *tree_node, int val, int lv) {

	if (tree_node == NULL) {
		node *temp = (node *) malloc(sizeof(node));
		temp->value = val;
		temp->counter = 1;
		temp->left = NULL;
		temp->right = NULL;
		temp->level = lv;
		return temp;
	}

	else if (val == tree_node->value)
		tree_node->counter += 1;

	else if (val < tree_node->value)
		tree_node->left = insert(tree_node->left, val, lv+1);

	else if (val > tree_node->value)
		tree_node->right = insert(tree_node->right, val, lv+1);

	return tree_node;
}
