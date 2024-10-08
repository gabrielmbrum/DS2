#include <stdio.h>

#define M 5

typedef struct page {
	int RRN;
	char keys[M-1][TAM_PLACA];
	int RRNS[M-1];
	int CHILDS[M];
}
