/*
 * node.h
 *
 *  Created on: Jan 20, 2024
 *      Author: nvfer
 */
#include <stdlib.h>
#include <stdio.h>
#include "ttoken.h"
#ifndef NODE_H_
//definition of trinary tree node w/ center as mid
#define NODE_H_
typedef struct title{
	char nonterm[11] ;
}title;
typedef struct node{
	Ttoken* tk;
	title tTitle;
	struct node* one;
	struct node* two;
	struct node* three;
	struct node* four;
}node;



#endif /* NODE_H_ */
