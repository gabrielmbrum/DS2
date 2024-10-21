#include <stdio.h>

#define LICENSE_PLATE_SIZE 8
#define MODEL_SIZE 20
#define BRAND_SIZE 20
#define CATEGORY_SIZE 15
#define STATUS_SIZE 16

typedef struct {
  char licensePlate[LICENSE_PLATE_SIZE];    // License plate
  char model[MODEL_SIZE];                   // Model of the vehicle
  char brand[BRAND_SIZE];                   // Brand of the vehicle
  int year;                                 // Year of manufacture
  char category[CATEGORY_SIZE];             // Category (e.g., economy, luxury, SUV)
  int mileage;                              // Mileage
  char status[STATUS_SIZE];                 // Availability status (e.g., available, rented, under maintenance)
} Vehicle;

void printVehicle (Vehicle v);