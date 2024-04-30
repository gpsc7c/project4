#ifndef TREE_H_
#define TREE_H_
#include "node.h"
#include <stdio.h>
#include <string.h>
//list of token and nonterminal names for reference

void setChk(int val);
struct node* newTermNode(Ttoken* tk); // Allocate memory for new node (with terminal)
struct node* newNode(char* nonTerms); // Allocate memory for new node

void printPreOrder(struct node* dataNode, int depth);//print in preorder
void deleteTree(struct node* dataNode);//deletes the tree
void statSem(node *dataNode, int* IDCount, FILE* outFile);//perform static semantics
void preStat(node *dataNode, int* IDCount, FILE* outFile);//run through the branches of each node calling statSem
int chkNode(node* dataNode, int* IDCount, FILE* outFile);//check each node for variable errors

#endif
