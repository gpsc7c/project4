#ifndef IDSTACK_H
#define IDSTACK_H
#include "ttoken.h"
//Type definition for element of stacl
typedef struct eElement{
	Ttoken tk;
	struct eElement *next;
}eElement;
//Type definition for stack
typedef struct sStack{
	int count;
	eElement* top;
}sStack;
void stackSet(sStack);
void push(Ttoken*);
void pop();
int find(Ttoken*, int* row, int* col);
#endif
