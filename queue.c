#include "queue.h"

void initializeQueue(Queue* queue) {
  queue->first = queue->last = NULL;
  queue->size = 0;
}

void dequeue(Queue* queue) {
  if (queue->first == NULL) return; // if queue is empty

  QueueNode* aux = queue->first;
  while(aux->next->next)
    aux = aux->next;
  queue->last = aux;
  free(aux);
}

void enqueue(Queue* queue, BTreeNode* node, bool modified) {
  if(queue->size == P) {
    dequeue(queue);
  }
}