#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <limits.h>
#include "ttoken.h"
#include "langscan.h"
#define MAX_LENGTH 8//max length of token definition
#ifndef SCANL_H
#define SCANL_H
//prints error based on error code and where error is
void printErr(int err, int row, int column);
Ttoken tokenSetter(int tkID, char* tkStr, int tkRow, int tkCol);
Ttoken FSADriver(FILE* fName, char* nextChar, int row, int* col);
Ttoken scanner(FILE* fName, char* c, int* row, int* col);
#endif
