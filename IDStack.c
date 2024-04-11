#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "IDStack.h"
#include "scanner.h"
#ifndef IDSTACK_C
#define IDSTACK_C
static const int MAXSTACK = 100;//maximum stack size
static sStack stack = {.count = 0, .top = NULL};//initial stack definition on program load.

//////////////////////////////////////////////
//Function to push an element onto the steck//
//////////////////////////////////////////////
void push(Ttoken* tk){
	if(stack.count >= MAXSTACK){
		fprintf(stderr, "\nERROR: IDStack.c: push Stack overflow."
				"Stack has %d elements.\n", stack.count);
		exit(-1);
	}
	eElement *element = (eElement*)malloc(sizeof(eElement));
	element->tk = tokenSetter(tk->ID, tk->tokenInstance, tk->row, tk->column);
	element->next = stack.top;
	stack.top = element;
	stack.count++;	
}
///////////////////////////////////////////////
//Function to pop an element off of the steck//
///////////////////////////////////////////////
void pop(){
	if(stack.count < 1){
		fprintf(stderr, "\nERROR: IDStack.c: pop: Stack empty.\n");
		exit(-1);
	}
	eElement* tempholder = stack.top;
	stack.top = tempholder->next;
	free(tempholder);
	stack.count--;
}
//should not be invoked before stack initialized
///////////////////////////////////////////////////
//Function to search stack for variable instances//
///////////////////////////////////////////////////
int find(Ttoken* tk, int* row, int* col){
	eElement* element = (eElement*)malloc(sizeof(eElement));//stack tracker
	int pos;//position tracker
	element = stack.top;//set to top
	//sets row and column for error tracking,
	//necessary due to pointer errors
	if(element != NULL){
		*row = stack.top->tk.row;
		*col = stack.top->tk.column;
	}
	//checks each element of the stack for a match
	for(pos = 0; pos < stack.count; pos++){
		if(strcmp(tk->tokenInstance, element->tk.tokenInstance)==0){
			return(pos);
		}
		//move to next element
		element = element->next;
		if(element != NULL){
			*row = element->tk.row;
			*col = element->tk.column;
		}
	}
	//return -1 if not found
	return -1;
}
#endif
