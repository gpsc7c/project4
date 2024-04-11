#ifndef TREE_C_
#define TREE_C_
#include "node.h"
#include <stdio.h>
#include <string.h>
#include "IDStack.h"
#include "tree.h"
char testnonterms[28][11] = {"TERMINAL", "<program>", "<func>", "<block>", "<vars>", "<facvars>", "<expr>", "<N>", "<N1>", "<A>", "<M>", "<R>", "<stats>", "<mStat>", "<stat>", "<in>", "<out>", "<if>", "<pick>", "<pickbody>", "<loop1>", "<loop2>", "<assign>", "<RBracket>", "<RTriplet>", "<R0>", "<label>", "<goto>"};
char* tokenNames[43] = {"IDTK", "INTTK", "ASSIGNTK", "GREATTK", "LESSTK", "ISEQUALTK", "NOTEQUALTK", "COLONTK", "COLONEQLTK", "PLUS", "MINUSTK", "MULTIPLYTK", "DIVIDETK", "EXPONTK", "DOTTK", "OPENPARENTK", "CLOSEPARENTK", "COMMATK", "OPENCURLTK", "CLOSECURLTK", "SEMICOLONTK", "OPENSQUARETK", "CLOSESQUARETK", "ORTK", "ANDTK", "STARTTK", "STOPTK", "WHILETK", "REPEATTK", "UNTILTK", "LABELTK", "RETURNTK", "CINTK", "COUTTK", "TAPETK", "JUMPTK", "IFTK", "THENTK", "PICKTK", "CREATETK", "SETTK", "FUNCTK", "EOFTK"};

struct node* newTermNode(Ttoken* tk){ // Allocate memory for new node
    	 node* nNode
        	= (struct node*)malloc(sizeof(node));
	int i;
	int j;
	nNode->tk = (Ttoken*)malloc(sizeof(Ttoken));
    	//assign title
    	nNode->tk->ID = tk->ID;
	for(j = 0; j < 9; j++){
    		nNode->tk->tokenInstance[j] = tk->tokenInstance[j];
	}
    	nNode->tk->row = tk->row;
    	nNode->tk->column = tk->column;
	
	for(i = 0; i < strlen(testnonterms[0]); i++){
    		nNode->tTitle.nonterm[i] = testnonterms[0][i];
	}
	for(; i <=10; i++){
    		nNode->tTitle.nonterm[10] = '\0';
	}


    	//initialize children as NULL
	nNode->one = NULL;
    	nNode->two = NULL;
    	nNode->three = NULL;
	nNode->four = NULL;
    	return nNode;
}
struct node* newNode(char* nonTerms){ // Allocate memory for new node
    	node* nNode
        	= (struct node*)malloc(sizeof(struct node));
	int i;
    	//assign title
    	nNode->tk = NULL;
	for(i = 0; i < 10; i++){
    		nNode->tTitle.nonterm[i] = nonTerms[i];
	}
    	nNode->tTitle.nonterm[10] = '\0';
    	//initialize children as NULL
	nNode->one = NULL;
    	nNode->two = NULL;
    	nNode->three = NULL;
	nNode->four = NULL;
    	return nNode;
}
void printPreOrder(struct node* dataNode, int depth){
	
	if(dataNode == NULL){
		return;
	}
	int i;	
	for(i = 0; i < depth; i++){
		fprintf(stdout, " |");
	}
	if(dataNode->tk != NULL){
		fprintf(stdout, " \\-%s: %s at depth %d\n", dataNode->tTitle.nonterm, tokenNames[dataNode->tk->ID], depth);
	}
	else{
		fprintf(stdout, " \\-%s at depth %d\n", dataNode->tTitle.nonterm, depth);
	}
	printPreOrder(dataNode->one, depth+1);
	printPreOrder(dataNode->two, depth+1);
	printPreOrder(dataNode->three, depth+1);
	printPreOrder(dataNode->four, depth+1);
}
/////////////////////////////////////
//Function to delete the parse tree//
/////////////////////////////////////
void deleteTree(struct node* dataNode){
	if(dataNode == NULL){
		fprintf(stderr, "\nERROR: tree.h: deleteTree: tree does not exist\n"); 
		return;
	}
	if(dataNode->one != NULL){
		deleteTree(dataNode->one);
	}
	if(dataNode->two != NULL){
		deleteTree(dataNode->two);
	}
	if(dataNode->three != NULL){
		deleteTree(dataNode->three);
	}
	if(dataNode->four != NULL){
		deleteTree(dataNode->four);
	}
	if(dataNode->tk != NULL){
		free(dataNode->tk);
	}
	free(dataNode);
}
////////////////////////////////////////////////////////////
//Steps through the node branches after checking nodes for//
//relevant data in preorder format//////////////////////////
////////////////////////////////////////////////////////////
void preStat(node *dataNode, int* IDCount){
	if(strcmp(dataNode->tTitle.nonterm, testnonterms[0])){
		chkNode(dataNode, IDCount);
		statSem(dataNode->one, IDCount);
		statSem(dataNode->two, IDCount);
		statSem(dataNode->three, IDCount);
		statSem(dataNode->four, IDCount);
	}
}
////////////////////////////////////////////////////////////////////////
//checks for starts of local and global scopes, else return to prestat//
////////////////////////////////////////////////////////////////////////
void statSem(node* dataNode, int* IDCount){
	//checking for '<program>' and '<block>'
	if(dataNode != NULL){
		if(!strcmp(dataNode->tTitle.nonterm, testnonterms[1]) || !strcmp(dataNode->tTitle.nonterm, testnonterms[3])){
			int i;
			int *newIDCount = (int*) malloc(sizeof(int));
			*newIDCount = 0;
			
			preStat(dataNode, newIDCount);
			for( i = 0; i < *newIDCount; i++){
				pop();
			}
			free(newIDCount);
		}
		else{
			preStat(dataNode, IDCount);
		}
	}
}

//////////////////////////////////////////////////////////
////Function to check each relevant node for data errors//
//////////////////////////////////////////////////////////
void chkNode(node* dataNode, int* IDCount){
	int* row = (int*)malloc(sizeof(int));//var to hold row from stack
	int* col = (int*)malloc(sizeof(int));//var to hold column from stack
	//checks for declarations of func or variables 
	if(strcmp(dataNode->tTitle.nonterm, testnonterms[5])==0 || strcmp(dataNode->tTitle.nonterm, testnonterms[2]) == 0){
		if(dataNode->two->tk->ID == IDTK){
			//search stack
			int dist = find(dataNode->two->tk, row, col);
			//if found, error out due to conflict
			if( dist != -1){
				fprintf(stderr, "\nERROR: IDStack.c: searchVar:"
					" Variable of ID '%s' already defined"
					" in this scope.\n"
					": Repeat definition on line %d at "
					"char %d\n"
					": Initial definition on line "
					"%d at char %d.", dataNode->two->tk->tokenInstance, 
					dataNode->two->tk->row, dataNode->two->tk->column,
					*row, *col);
				exit(-1);	
			}
			//else push onto stack
			push(dataNode->two->tk);
			*IDCount += 1;
		}
	}
	//looks for any occasions where vars might be used
	else if(strcmp(dataNode->tTitle.nonterm, testnonterms[11])==0||strcmp(dataNode->tTitle.nonterm, testnonterms[22])==0||strcmp(dataNode->tTitle.nonterm, testnonterms[15])==0||strcmp(dataNode->tTitle.nonterm, testnonterms[26])==0||strcmp(dataNode->tTitle.nonterm, testnonterms[27])==0){
		//variables necessary to recover error data
		int dist = -2;
		char* temp;
		int temprow = -2;
		int tempcol = -2;
		//checks for "ID first" nodes
		if(dataNode->one->tk->ID == 0){
			dist = find(dataNode->one->tk, row, col);
			temp = dataNode->one->tk->tokenInstance;
			temprow = dataNode->one->tk->row;
			tempcol = dataNode->one->tk->column;
		}
		//tests for "ID second" nodes, which encompasses all other 
		//valid nodes
		else if(dataNode->two != NULL && dataNode->two->tk != NULL && dataNode->two->tk->ID == 0){
			dist = find(dataNode->two->tk, row, col);
			temp = dataNode->two->tk->tokenInstance;
			temprow = dataNode->two->tk->row;
			tempcol = dataNode->two->tk->column;
		}
		//if not found error out
		if(dist == -1){
			fprintf(stderr, "\nERROR: tree.c: chkNode: Identifier "
					"%s on line %d at char %d has not been"
					" Initialized!\n", 
					temp,temprow,tempcol);
			exit(-1); 
		}
	}
}
#endif
