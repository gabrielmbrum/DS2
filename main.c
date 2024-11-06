#include "vbtree.h"
#include <stdio.h>
#include <stdlib.h>

int main() {
  char *filename;
  BTreeNode *root = (BTreeNode*) malloc(sizeof(BTreeNode));
  Queue *queue = (Queue*) malloc(sizeof(Queue));

  build_name(&filename);
  initializeQueue(&queue);
  
  // Carrega o B-tree se o arquivo já existir
  if (fileexist(filename)) {
    root = readRootRRNFromFile(filename, queue);
    printf("Arquivo %s carregado com sucesso.\n", filename);
  } else {
    // Cria o arquivo a partir de "vehicles.dat"
    root = buildBtreeFile(filename);
    printf("Arquivo %s criado com sucesso a partir do datafile.\n", filename);
  }

  int option;
  do {
    printf("\n=== Menu de Opções ===\n");
    printf("1. Buscar veículo pela placa\n");
    printf("2. Inserir veículo (não implementado)\n");
    printf("3. Remover veículo (não implementado)\n");
    printf("4. Sair\n");
    printf("Escolha uma opção: ");
    scanf("%d", &option);

    switch (option) {
      case 1:
        searchInBTree(root, queue);
        break;

      case 2:
        printf("Ops! A funcionalidade de inserção ainda está em desenvolvimento. Aguarde as próximas atualizações!\n");
        break;

      case 3:
        printf("Parece que remover veículos ainda não é possível. Fique ligado para futuras implementações!\n");
        break;

      case 4:
        printf("Saindo do programa. Até logo!\n");
        break;

      default:
        printf("Opção inválida. Tente novamente.\n");
    }
  } while (option != 4);

  free(root);
  free(queue);
  free(filename);
  
  return 0;
}