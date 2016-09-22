#ifndef NODE_H
#define NODE_H

typedef struct tree_structure {
	int value;
	int counter;
	int level;

	struct tree_structure *left;
	struct tree_structure *right;
} node;

#endif
