#include "vbtree.h"
#include <stdio.h>

int counter = 0;
int global_page_rrn = 1;
int global_vehicle_rrn = 1;
const size_t sizeof_rrn = sizeof(int);
const size_t tamanho_registro_veiculo = sizeof(Vehicle);
const size_t btree_size = sizeof(BTreeNode);

/* Creates a new B-tree node as either a leaf or an internal node */
BTreeNode *createNode(bool is_leaf) {
  int i;
  BTreeNode *newNode = (BTreeNode *)malloc(sizeof(BTreeNode));

  if (newNode == NULL) {
    printf("\n\tERROR: memory allocation failed to create a new BTreeNode :(\n");
    exit(EXIT_FAILURE);
  }

  newNode->numKeys = 0;
  newNode->isLeaf = is_leaf ? 1 : 0;

  for (i = 0; i < M - 1; i++) {
    strcpy(newNode->keys[i], "");
    newNode->keys_rrn[i] = -1; 
    newNode->children[i] = -1;    
  }
  newNode->children[M - 1] = -1;
  newNode->rrn = global_page_rrn++;

  return newNode;
}

/* Writes the root RRN to the file header (first sizeof_rrn bytes) */
void writeRootRRNToFile(int root_rrn, const char *filename) {
  FILE *fp = fopen(filename, "wb");
  if (fp == NULL) {
    printf("ERROR: Could not open %s to write root RRN\n", filename);
    exit(1);
  }

  fseek(fp, 0, SEEK_SET);
  fwrite(&root_rrn, sizeof_rrn, 1, fp);
  fclose(fp);
}

/* Reads the root RRN from the file header and returns the root node */
BTreeNode* readRootRRNFromFile(const char *filename, Queue *queue) {
  int root_rrn;
  FILE *fp = fopen(filename, "rb");
  if (fp == NULL) {
    printf("ERROR: Could not open %s to read root RRN\n", filename);
    exit(1);
  }

  // Read the root RRN
  fseek(fp, 0, SEEK_SET);
  fread(&root_rrn, sizeof_rrn, 1, fp);
  fclose(fp);

  // Load the root node from the file and enqueue it
  BTreeNode *root_node = readNodeByRRNFromFile(root_rrn);
  enqueue(&queue, root_node, false);
  return root_node;
}

/* Splits a full child node at index, promoting the median key to the parent */
void split(BTreeNode** parent, int index, BTreeNode* child, Queue** queue) {
  if (*parent == NULL || child == NULL) {
    return;
  }

  BTreeNode *newNode = createNode(child->isLeaf);
  newNode->numKeys = (M / 2) - 1;

  for (int i = 0; i <= newNode->numKeys; i++) {
    strcpy(newNode->keys[i], child->keys[i + (M / 2)]);
    newNode->keys_rrn[i] = child->keys_rrn[i + (M / 2)];
  }

  if (!child->isLeaf) {
    for (int i = 0; i < M / 2; i++) {
      newNode->children[i] = child->children[i + (M / 2)];
    }
  }

  child->numKeys = (M / 2) - 1;

  for (int i = (*parent)->numKeys; i > index; i--) {
    (*parent)->children[i + 1] = (*parent)->children[i];
  }
  (*parent)->children[index + 1] = newNode->rrn;

  enqueue(queue, newNode, true);
  enqueue(queue, child, true);

  for (int i = (*parent)->numKeys - 1; i >= index; i--) {
    strcpy((*parent)->keys[i + 1], (*parent)->keys[i]);
    (*parent)->keys_rrn[i + 1] = (*parent)->keys_rrn[i];
  }

  strcpy((*parent)->keys[index], child->keys[(M / 2) - 1]);
  (*parent)->keys_rrn[index] = child->keys_rrn[(M / 2) - 1];
  (*parent)->numKeys++;
}

/* Inserts a key into a non-full node, recursing as needed */
void insertNonFull(BTreeNode** node, const char *key, int vehicle_rrn, Queue** queue) {
  int i = (*node)->numKeys - 1;

  if ((*node)->isLeaf) {
    while (i >= 0 && strcmp((*node)->keys[i], key) > 0) {
      strcpy((*node)->keys[i + 1], (*node)->keys[i]);
      (*node)->keys_rrn[i + 1] = (*node)->keys_rrn[i];
      i--;
    }
    strcpy((*node)->keys[i + 1], key);
    (*node)->keys_rrn[i + 1] = vehicle_rrn;
    (*node)->numKeys++;
  } else {
    while (i >= 0 && strcmp((*node)->keys[i], key) > 0) {
      i--;
    }
    i++;

    BTreeNode* aux = getNode(*queue, (*node)->children[i]);

    if (aux->numKeys == M - 1) {
      split(node, i, aux, queue);
      
      if (strcmp((*node)->keys[i], key) < 0) {
        i++;
      }
      aux = getNode(*queue, (*node)->children[i]);
    }
    insertNonFull(&aux, key, vehicle_rrn, queue);
  }
}

/* Handles insertion at the root level, splitting root if needed */
void insert(BTreeNode **root, const char *key, int vehicle_rrn, Queue** queue, const char *filename) {
  BTreeNode *node = *root;

  if (node->rrn == 0) { // Create a root if it doesn't exist
    *root = createNode(true);
    strcpy((*root)->keys[0], key);
    (*root)->keys_rrn[0] = vehicle_rrn;
    (*root)->numKeys++;
    enqueue(queue, *root, true);
    writeRootRRNToFile((*root)->rrn, filename); // Update the root RRN in file
  } else {
    // Split the root if full
    if (node->numKeys == M - 1) {
      BTreeNode *new_root = createNode(false);
      new_root->children[0] = node->rrn;

      split(&new_root, 0, node, queue);
      *root = new_root;

      // Update the root RRN in the file after split
      writeRootRRNToFile(new_root->rrn, filename);
    }
    insertNonFull(root, key, vehicle_rrn, queue);
  }
}

/* Reads a B-tree node from file by its RRN (relative record number) */
BTreeNode* readNodeByRRNFromFile(int page_rrn) {
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
  fseek(fp, sizeof_rrn + sizeof(BTreeNode) * page_rrn, SEEK_SET); // Offset to skip root RRN
  fread(node, sizeof(BTreeNode), 1, fp);
  fclose(fp);

  return node;
}

/* Builds a B-tree index file from an input dataset */
BTreeNode* buildBtreeFile(char *filename) {
  FILE *dat = fopen("data/vehicles.dat", "rb");
  if (dat == NULL) {
    exit(1);
  }

  Vehicle reader_vehicle;
  fread(&reader_vehicle, tamanho_registro_veiculo, 1, dat);

  Queue *queue = (Queue*)malloc(sizeof(Queue));
  initializeQueue(&queue);
  
  BTreeNode *root;

  while (!feof(dat)) {
    fread(&reader_vehicle, tamanho_registro_veiculo, 1, dat);
    insert(&root, reader_vehicle.licensePlate, global_vehicle_rrn++, &queue, filename);
  }

  writeAllModified(queue);
  free(queue);
  fclose(dat);

  return root;
}

/* Initializes an empty queue for node storage */
void initializeQueue(Queue** queue) {
  (*queue)->first = (*queue)->last = NULL;
  (*queue)->size = 0;
}

/* Adds a node to the queue, moving it to the end if it's modified */
void enqueue(Queue** queue, BTreeNode* node, bool modified) {
  if (contains(*queue, node->rrn) && modified) {
    moveToLast(queue, node);
    return;
  }
  
  if ((*queue)->size == P) {
    dequeue(queue);
  }

  // Create and insert new queue node
  QueueNode* newNode = (QueueNode*)malloc(sizeof(QueueNode));
  newNode->node = node;
  newNode->modified = modified;
  newNode->next = NULL;

  if ((*queue)->last != NULL) {
    (*queue)->last->next = newNode;
  }

  (*queue)->last = newNode;
  if ((*queue)->first == NULL) {
    (*queue)->first = newNode;
  }
  (*queue)->size++;
}

/* Checks if the queue contains a node with a given RRN */
bool contains(Queue *queue, int rrn) {
  if (queue->size == 0) return false;

  QueueNode* node = queue->first;
  while (node != NULL) {
    if (node->node->rrn == rrn) {
      return true;
    }
    node = node->next;
  }
  return false;
}

/* Removes the oldest node from the queue, writing it to file if modified */
void dequeue(Queue** queue) {
  if ((*queue)->first == NULL) return;

  QueueNode* temp = (*queue)->first;

  if (temp->modified) {
    writeNodeToFile(temp->node);
  }

  if (temp->next == NULL) {
    free(temp);
  } else {
    while (temp->next->next != NULL) 
      temp = temp->next;

    (*queue)->last = temp;
    free(temp->next);
  }
  (*queue)->size--;
}

/* Moves a node to the end of the queue */
void moveToLast(Queue** queue, BTreeNode* node) {
  if ((*queue)->first == NULL || (*queue)->last == NULL)
    return;

  if ((*queue)->last->node == node || (*queue)->size < 1)
    return;

  QueueNode* current = (*queue)->first;
  QueueNode* prev = NULL;
  
  while (current != NULL && current->node != node) {
    prev = current;
    current = current->next;
  }

  if (current == NULL)
    return;

  if (current == (*queue)->first) {
    (*queue)->first = current->next;
  } else {
    prev->next = current->next;
  }
  current->next = NULL;
  (*queue)->last->next = current;
  (*queue)->last = current;
}

/* Writes all modified nodes in the queue to the index file */
void writeAllModified(Queue* queue) {
  QueueNode* node = queue->first;

  if (node->modified) writeNodeToFile(node->node);
  while (node->next != NULL) {
    node = node->next;
    writeNodeToFile(node->node);
  }
}

/* Builds a filename based on tree order (M) */
void build_name(char **name) {
  char *aux;
  *name = (char*)malloc(sizeof(char) * 14);
  aux = malloc(sizeof(char) * 4);

  sprintf(aux, "%d", M);
  strcpy(*name, "data/btree_");
  strcat(*name, aux);
  strcat(*name, ".idx");

  free(aux);
}

/* Writes a B-tree node to the index file by its RRN */
void writeNodeToFile(BTreeNode *node) {
  FILE *fp;
  char *filename;
  size_t aux;
  build_name(&filename);

  if ((fp = fopen(filename, "rb+")) == NULL) {
    if ((fp = fopen(filename, "wb+")) == NULL) {
      exit(EXIT_FAILURE);
    }
  }

  long offset = node->rrn * sizeof(BTreeNode) + sizeof_rrn;
  fseek(fp, 0, SEEK_END);
  long fileSize = ftell(fp);

  // Pad with zeros if needed
  if (offset > fileSize) {
    fseek(fp, fileSize, SEEK_SET);
    for (long i = fileSize; i < offset; i++) {
      fputc(0, fp);
    }
  }

  fseek(fp, offset, SEEK_SET);
  aux = fwrite(node, sizeof(BTreeNode), 1, fp);

  fclose(fp);
  free(filename);
}

// Função de busca no B-Tree usando a chave da placa
void searchInBTree(BTreeNode *root, Queue *queue) {
  char licensePlate[8];
  printf("Digite a placa do veículo que deseja buscar (ex: ABC1234): ");
  scanf("%s", licensePlate);

  // Usa a função de busca para encontrar o nó
  BTreeNode *node = searchNode(root, licensePlate, queue);

  if (node) {
    printf("Veículo com placa %s encontrado!\n", licensePlate);
    printVehicleInfo(node, licensePlate);  // Função que exibe detalhes do veículo
  } else {
    printf("Veículo com placa %s não encontrado.\n", licensePlate);
  }
}

// Função auxiliar de busca: retorna o nó que contém a chave ou NULL se não encontrado
BTreeNode* searchNode(BTreeNode *root, const char *licensePlate, Queue *queue) {
  BTreeNode *current = root;

  while (current != NULL) {
    // Procura pela chave no nó atual
    int i = 0;
    while (i < current->numKeys && strcmp(licensePlate, current->keys[i]) > 0) {
      i++;
    }

    // Se a chave foi encontrada, retorna o nó atual
    if (i < current->numKeys && strcmp(licensePlate, current->keys[i]) == 0) {
      return current;
    }

    // Se o nó atual é uma folha, a chave não existe na árvore
    if (current->isLeaf) {
      return NULL;
    }

    // Continua a busca no próximo nó filho
    current = getNode(queue, current->children[i]);
  }
  return NULL; // A chave não foi encontrada
}

// Função de impressão de informações de um veículo específico em um nó
void printVehicleInfo(BTreeNode *node, const char *licensePlate) {
  for (int i = 0; i < node->numKeys; i++) {
    if (strcmp(node->keys[i], licensePlate) == 0) {
      Vehicle vehicle = readVehicleByRRN(node->keys_rrn[i]);
      printVehicle(vehicle);
      return;
    }
  }
  printf("Informações do veículo não encontradas.\n");
}

// Função para ler um veículo no arquivo "vehicles.dat" com base no RRN
Vehicle readVehicleByRRN(int rrn) {
  FILE *file = fopen("data/vehicles.dat", "rb");
  if (file == NULL) {
    fprintf(stderr, "Erro ao abrir o arquivo vehicles.dat.\n");
    exit(EXIT_FAILURE);
  }

  // Calcula o deslocamento com base no tamanho do registro e o RRN
  long offset = rrn * tamanho_registro_veiculo;

  // Posiciona no local do registro desejado
  fseek(file, offset, SEEK_SET);

  // Lê o registro do veículo
  Vehicle vehicle;
  size_t result = fread(&vehicle, sizeof(Vehicle), 1, file);
  if (result != 1) {
    fprintf(stderr, "Erro ao ler o veículo com RRN %d.\n", rrn);
    fclose(file);
    exit(EXIT_FAILURE);
  }

  fclose(file);
  return vehicle;
}

void printVehicle (Vehicle v) {
  printf("---------------\n");
  printf("License Plate: %s\n", v.licensePlate);
  printf("Model: %s\n", v.model);
  printf("Brand: %s\n", v.brand);
  printf("Year: %d\n", v.year);
  printf("Category: %s\n", v.category);
  printf("Mileage: %d km\n", v.km);
  printf("Status: %s\n", v.status);
}

BTreeNode* getNode (Queue *queue, int rnn) {
  BTreeNode *node;

  node = searchOnQueue(queue, rnn);

  if(node == NULL) { //node it's not on queue
    node = readNodeByRRNFromFile(rnn);
  }

  return node;
}

BTreeNode* searchOnQueue (Queue *queue, int rnn) {
  QueueNode *aux = queue->first;

  while (aux != NULL & aux->node->rrn != rnn)
    aux = aux->next;

  return aux != NULL ? aux->node : NULL;
}

bool fileexist(char *filename) {
  return !(fopen(filename, "r") == NULL);
}