#include <stdio.h>

#define LICENSE_PLATE_SIZE 8
#define MODEL_SIZE 20
#define BRAND_SIZE 20
#define CATEGORY_SIZE 15
#define STATUS_SIZE 16

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