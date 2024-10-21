#include <stdio.h>

#define M 5
#define LICENSE_PLATE_SIZE 7

typedef struct page {
	int RRN;
	char KEYS[M-1][LICENSE_PLATE_SIZE];
	int RRNS[M-1];
	int CHILDS[M];
  int countkeys;
} PAGE;

typedef struct {
  char keys[M - 1][LICENSE_PLATE_SIZE];    // Vehicle license plates (keys)
  int rrns[M - 1];                         // RNNs of the vehicles in the data file
  int children[M];                         // RNNs of the child nodes
  int numKeys;                             // Number of keys in the page
  int isLeaf;                              // Indicates if the node is a leaf
  int rrn;                                 // RRN of the node itself
} BTreeNode;