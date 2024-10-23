#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// build the filename it will be determinated by M at "btree.h"
void build_name (char **name);

// @return true if exist a file with the name passed
bool fileexist(char *filename);