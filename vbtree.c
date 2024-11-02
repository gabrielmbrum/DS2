#include "vbtree.h"

/*                       old 'btree.c'                         */

int global_page_rrn = 0;
int global_vehicle_rrn = 0;

BTreeNode *createNode(bool is_leaf) {
  int i;
  BTreeNode *newNode = (BTreeNode *)malloc(sizeof(BTreeNode));
  
  if (newNode == NULL) {
    printf("\n\tERROR: memory allocation failed :(\n");
    exit(EXIT_FAILURE);
  }
  newNode->numKeys = 0;
  newNode->isLeaf = is_leaf;
  
  for (i = 0; i < M - 1; i++) {
    strcpy(newNode->keys[i], "");
    newNode->keys_rrn[i] = -1; 
    newNode->children[i] = -1;    
  }
  newNode->children[i] = -1;
  
  newNode->rrn = global_page_rrn++;

  return newNode;
}

void split(BTreeNode *parent, int index) {
    BTreeNode *child = /* Obter o nó filho de RRN */ parent->children[index];
    BTreeNode *newNode = createNode(child->isLeaf);
    
    newNode->numKeys = (M / 2) - 1;
    
    // Move as chaves e os RNNs para o novo nó
    for (int i = 0; i < M / 2 - 1; i++) {
        strcpy(newNode->keys[i], child->keys[i + M / 2]);
        newNode->keys_rrn[i] = child->keys_rrn[i + M / 2];
    }
    
    // Move os filhos, se o nó não for uma folha
    if (!child->isLeaf) {
        for (int i = 0; i < M / 2; i++) {
            newNode->children[i] = child->children[i + M / 2];
        }
    }
    
    child->numKeys = M / 2 - 1;
    
    // Ajusta os filhos do pai para acomodar o novo nó
    for (int i = parent->numKeys; i > index; i--) {
        parent->children[i + 1] = parent->children[i];
    }
    parent->children[index + 1] = /* RRN do novo nó */;
    
    // Ajusta as chaves e os RNNs do pai para inserir a chave do meio do filho
    for (int i = parent->numKeys - 1; i >= index; i--) {
        strcpy(parent->keys[i + 1], parent->keys[i]);
        parent->keys_rrn[i + 1] = parent->keys_rrn[i];
    }
    
    strcpy(parent->keys[index], child->keys[M / 2 - 1]);
    parent->keys_rrn[index] = child->keys_rrn[M / 2 - 1];
    parent->numKeys++;
}

void insertNonFull(BTreeNode *node, const char *key, int rrn) {
    int i = node->numKeys - 1;
    
    if (node->isLeaf) {
        // Insere a chave em ordem crescente
        while (i >= 0 && strcmp(node->keys[i], key) > 0) {
            strcpy(node->keys[i + 1], node->keys[i]);
            node->keys_rrn[i + 1] = node->keys_rrn[i];
            i--;
        }
        strcpy(node->keys[i + 1], key);
        node->keys_rrn[i + 1] = rrn;
        node->numKeys++;
    } else {
        // Encontra o filho onde a chave deve ser inserida
        while (i >= 0 && strcmp(node->keys[i], key) > 0) {
            i--;
        }
        i++;
        
        if (/* Obter o nó filho de RRN */ node->children[i]->numKeys == M - 1) {
            // Divide o filho se estiver cheio
            split(node, i);
            
            // Determina em qual dos dois filhos a chave deve ir
            if (strcmp(node->keys[i], key) < 0) {
                i++;
            }
        }
        insertNonFull(/* Obter o nó filho de RRN */ node->children[i], key, rrn);
    }
}

void insert(BTreeNode **root, const char *key, int rrn) {
  BTreeNode *node = *root;

  if (node == NULL) {
    // Cria um novo nó raiz
    *root = createNode(true);
    strcpy((*root)->keys[0], key);
    (*root)->keys_rrn[0] = rrn; //vehicle rrn
    (*root)->numKeys = 1;
  } else {
    if (node->numKeys == M - 1) {
      // Divide a raiz se estiver cheia
      BTreeNode *new_root = createNode(false);
      new_root->children[0] = (*root)->rrn;
      split(new_root, 0);
      *root = new_root;
    }
    insertNonFull(*root, key, rrn);
  }
}

BTreeNode* readNodeByRNNFromFile (int page_rrn) {
  FILE *fp;
  char *filename;
  BTreeNode *node;

  build_name(&filename);
  
  fp = fopen(filename, "rb");
  if (fp == NULL) {
    printf("\n\tERROR: failed to open %s file", filename);
    exit(1);
  }

  node = (BTreeNode*) malloc(sizeof(BTreeNode));

  fseek(fp, sizeof(BTreeNode) * page_rrn, SEEK_SET); 

  fread(node, sizeof(BTreeNode), 1, fp);

  fclose(fp);

  return node;
}

BTreeNode* buildBtreeFile (char *filename) {
  /*
  while (tem registros no .dat)
  // ler do .dat
  // inserir na fila
    // if chega no limite da fila
      // if modified
        // write on .idx
  */
}

/*                       old 'vehicle.c'                         */

void printVehicle (Vehicle v) {
  printf("---------------\n");
  printf("License Plate: %s\n", v.licensePlate);
  printf("Model: %s\n", v.model);
  printf("Brand: %s\n", v.brand);
  printf("Year: %d\n", v.year);
  printf("Category: %s\n", v.category);
  printf("Mileage: %d km\n", v.mileage);
  printf("Status: %s\n", v.status);
}

/*                       old 'queue.c'                         */

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

  if (queue->last != NULL) 
    queue->last->next = newNode;

  queue->last = newNode;

  if (queue->first == NULL)
    queue->first = newNode;

  queue->size++;
}

void dequeue(Queue* queue) {
  if (queue->first == NULL) return; // if queue is empty

  QueueNode* temp = queue->first;
  
  // if (temp->modified) {
  //   writeNodeToFile(temp->node);
  // }

  if (temp->next == NULL) {
    free(temp);
  } else {
    while (temp->next->next != NULL) 
      temp = temp->next;

    queue->last = temp;

    free(temp->next);
  }

  queue->size--;
}

void moveToLast(Queue *queue, BTreeNode* node) {
  if (queue->first == NULL || queue->last == NULL)
    return;

  if (queue->last->node == node || queue->size < 1)
    return;

  QueueNode* current = queue->first;
  QueueNode* prev = NULL;
  
  while (current != NULL && current->node != node) {
    prev = current;
    current = current->next;
  }

  // node isnt on the queue
  if (current == NULL)
    return;

  if (current == queue->first) {
    // in this case, prev is NULL (there isnt a node before the current)
    queue->first = current->next;
  } else {
    prev->next = current->next;
  }
  current->next = NULL;
  queue->last->next = current;
  queue->last = current;
}


BTreeNode* getNode (Queue *queue, int rnn) {
  BTreeNode *node;

  node = searchOnQueue(queue, rnn);

  if(node == NULL) { //node it's not on queue
    node = readNodeByRNNFromFile(rnn);
  }

  return node;
}

BTreeNode* searchOnQueue (Queue *queue, int rnn) {
  QueueNode *aux = queue->first;

  while (aux != NULL & aux->node->rrn != rnn)
    aux = aux->next;

  return aux != NULL ? aux->node : NULL;
}

/*                       old 'file.c'                         */

void build_name (char **name) {
  char *aux;
  *name = (char*)malloc(sizeof(char) * 14); //the biggest name would be btree_100.idx (13 chars + \0) = 14 
  aux = malloc(sizeof(char) * 4); //it will receive the numeric order (max it's 100)

  sprintf(aux, "%d", M);
  strcpy(*name, "btree_");
  strcat(*name, aux);
  strcat(*name, ".idx");

  free(aux);
}

bool fileexist(char *filename) {
  return !(fopen(filename, "r") == NULL);
}

void writeNodeToFile (BTreeNode *node) {
  FILE *fp;
  char *filename;
  size_t btree_size, aux;
  build_name(&filename);

  if ((fp = fopen(filename, "wb")) == NULL) {
    printf("failed to open %s :<\n", filename);
    return;
  }

  btree_size = sizeof(BTreeNode);

  fseek(fp, node->rrn, SEEK_SET);
  aux = fwrite(true, btree_size, 1, fp);

  free(fp);
  free(filename);
}