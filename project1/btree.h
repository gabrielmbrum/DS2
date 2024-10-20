#include <stdio.h>

#define M 5
#define TAM_PLACA 7

typedef struct page {
	int RRN;
	char KEYS[M-1][TAM_PLACA];
	int RRNS[M-1];
	int CHILDS[M];
  int countkeys;
} PAGE;

typedef struct btreeNode {
	int num_keys; // Number of keys currently in the node
	int keys[M-1]; // Array of keys
	struct btreeNode *children[M]; // Array of child pointers
	bool is_leaf; // True if node is a leaf
} BTreeNode;