#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define M 5
#define P 10
#define LICENSE_PLATE_SIZE 8
#define MODEL_SIZE 20
#define BRAND_SIZE 20
#define CATEGORY_SIZE 15
#define STATUS_SIZE 16

/*                                  old 'btree.h'                                 */

typedef struct {
  char keys[M - 1][LICENSE_PLATE_SIZE];    // Vehicle license plates (keys)
  int keys_rrn[M - 1];                         // RNNs of the vehicles in the data file
  int children[M];                         // RNNs of the child nodes
  int numKeys;                             // Number of keys in the page
  bool isLeaf;                             // Indicates if the node is a leaf
  int rrn;                                 // RRN of the node itself
} BTreeNode;

BTreeNode *createNode(bool is_leaf);

BTreeNode* buildBtreeFile (char *filename);

void split(BTreeNode *parent, int index);

void insertNonFull(BTreeNode *node, const char *key, int rrn);

void insert(BTreeNode **root, const char *key, int rrn);

BTreeNode* readNodeByRNNFromFile (int page_rrn);

/*                                  old 'vehicle.h'                                 */

typedef struct {
  char licensePlate[LICENSE_PLATE_SIZE];    
  char model[MODEL_SIZE];                   
  char brand[BRAND_SIZE];                   
  int year;                                 
  char category[CATEGORY_SIZE];             
  int km;                              
  char status[STATUS_SIZE];                 
} Vehicle;

void printVehicle (Vehicle v);

const size_t tamanho_registro_veiculo = sizeof(Vehicle);

/*                                  old 'queue.h'                                 */

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

void initializeQueue(Queue *queue);

void enqueue(Queue* queue, BTreeNode* node, bool modified);

void dequeue(Queue* queue);

void moveToLast(Queue *queue, BTreeNode* node);

BTreeNode* getNode (Queue *queue, int rrn);

BTreeNode* searchOnQueue (Queue *queue, int rnn);

/*                                  old 'file.h'                                 */

void build_name (char **name);

bool fileexist(char *filename);

void writeNodeToFile (BTreeNode *node);