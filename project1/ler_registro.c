#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAMANHO_PLACA 8
#define TAMANHO_MODELO 20
#define TAMANHO_MARCA 20
#define TAMANHO_CATEGORIA 15
#define TAMANHO_STATUS 16

typedef struct {
  char placa[TAMANHO_PLACA];
  char modelo[TAMANHO_MODELO];
  char marca[TAMANHO_MARCA];
  int ano;
  char categoria[TAMANHO_CATEGORIA];
  int quilometragem;
  char status[TAMANHO_STATUS];
} Veiculo;

int main() {
	FILE *datFile = fopen("veiculos.dat", "rb");
	if (datFile == NULL) {
		perror("Erro ao abrir o arquivo veiculos.dat");
		return 1;
	}

	size_t tamanho_registro = sizeof(Veiculo);
	printf("Tamanho de um registro em bytes: %zu\n",tamanho_registro);
	Veiculo veiculo;
	// Ler o registro
	size_t registros_lidos = fread(&veiculo, tamanho_registro, 1, datFile);
	if (registros_lidos != 1) {
		printf("Erro ao ler o registro");
		fclose(datFile);
		return 1;
	}
	// Imprimir os dados do veículo
	printf("Registro de RNN 0\n");
	printf("Placa: %s\n", veiculo.placa);
	printf("Modelo: %s\n", veiculo.modelo);
	printf("Marca: %s\n", veiculo.marca);
	printf("Ano: %d\n", veiculo.ano);
	printf("Categoria: %s\n", veiculo.categoria);
	printf("Quilometragem: %d\n", veiculo.quilometragem);
	printf("Status: %s\n", veiculo.status);
	printf("---------------------------\n");
	fclose(datFile);
	return 0;
}
