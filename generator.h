#include <stdio.h>
#include "node.h"

#ifndef GENERATOR_H
#define GENERATOR_H

void recGen(node* dataNode, int* IDCount, FILE* outFile);
void generator(node* root, FILE* outFile); 

#endif
