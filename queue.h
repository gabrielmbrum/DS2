#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "btree.h"

#define P 10

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

void moveToFirst(Queue *queue, BTreeNode* node);