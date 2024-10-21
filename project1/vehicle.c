#include "vehicle.h"

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