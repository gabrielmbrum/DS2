#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define M 4
#define P 10
#define LICENSE_PLATE_SIZE 8
#define MODEL_SIZE 20
#define BRAND_SIZE 20
#define CATEGORY_SIZE 15
#define STATUS_SIZE 16

typedef struct {
  char keys[M - 1][LICENSE_PLATE_SIZE];    // Vehicle license plates (keys)
  int keys_rrn[M - 1];                         // RRNs of the vehicles in the data file
  int children[M];                         // RRNs of the child nodes
  int numKeys;                             // Number of keys in the page
  bool isLeaf;                             // Indicates if the node is a leaf
  int rrn;                                 // RRN of the node itself
} BTreeNode;

typedef struct queueNode {
  BTreeNode *node;
  bool modified;
  struct queueNode *next;
} QueueNode;

typedef struct queue {
  QueueNode *first;
  QueueNode *last;
  int size;
} Queue;

typedef struct {
  char licensePlate[LICENSE_PLATE_SIZE];    
  char model[MODEL_SIZE];                   
  char brand[BRAND_SIZE];                   
  int year;                                 
  char category[CATEGORY_SIZE];             
  int km;                              
  char status[STATUS_SIZE];                 
} Vehicle;

BTreeNode *createNode(bool is_leaf);

BTreeNode* buildBtreeFile (char *filename);

void split(BTreeNode** parent, int index, BTreeNode* child, Queue** queue);

void insertNonFull(BTreeNode** node, const char *key, int vehicle_rrn, Queue** queue);

void insert(BTreeNode **root, const char *key, int vehicle_rrn, Queue** queue, const char *filename);

BTreeNode* readNodeByRRNFromFile (int page_rrn);

BTreeNode* readRootRRNFromFile(const char *filename, Queue *queue);

void printVehicle (Vehicle v);

void initializeQueue(Queue **queue);

void enqueue(Queue** queue, BTreeNode* node, bool modified);

void dequeue(Queue** queue);

void moveToLast(Queue** queue, BTreeNode* node);

void writeAllModified(Queue* queue);

bool contains(Queue* queue, int rrn);

BTreeNode* getNode (Queue *queue, int rrn);

BTreeNode* searchOnQueue (Queue *queue, int rrn);

void build_name (char **name);

bool fileexist(char *filename);

void writeNodeToFile (BTreeNode *node);

void printNode(BTreeNode* node);

Vehicle readVehicleByRRN(int rrn);

void printVehicleInfo(BTreeNode *node, const char *licensePlate);

BTreeNode* searchNode(BTreeNode *root, const char *licensePlate, Queue *queue);

void searchInBTree(BTreeNode *root, Queue *queue);