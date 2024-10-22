#include "queue.h"
#include <time.h>

void initializeQueue(Queue* queue) {
  queue->first = queue->last = NULL;
  queue->size = 0;
}

void enqueue(Queue* queue, BTreeNode* node, bool modified) {
  if(queue->size == P) {
    dequeue(queue);
  }

  QueueNode* newNode = (QueueNode*)malloc(sizeof(QueueNode));
  newNode->node = node;
  newNode->modified = modified;
  newNode->next = NULL;

  if (queue->first != NULL)
    queue->first->next = newNode;

  queue->first = newNode;

  if (queue->last == NULL)
    queue->last = newNode;

  queue->size++;
}

void dequeue(Queue* queue) {
  if (queue->first == NULL) return; // if queue is empty

  QueueNode* temp = queue->first;
  
  if (temp->modified) {
    //writeNodeToFile(temp->node);
  }

  queue->last = temp->next;

  if (queue->first == NULL) {
    queue->last = NULL;
  }

  free(temp);
  queue->size--;
}

void moveToFirst(Queue *queue, BTreeNode* node) {
  if (queue->first == NULL || queue->last == NULL)
    return;

  if (queue->first->node == node)
    return;

  QueueNode* current = queue->first;
  QueueNode* prev = NULL;
  
  while (current != NULL && current->node != node) {
    prev = current;
    current = current->next;
  }

  if (current != NULL) {
    if (prev != NULL) {
      prev->next = current->next;
    }

    if (current == queue->last) {
      queue->last = prev;
    }

    current->next = queue->first;
    queue->first = current;

    if(queue->last == NULL)
      queue->last = current;
  }
}