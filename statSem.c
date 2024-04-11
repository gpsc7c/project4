#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "scanner.h"
#include "parser.h"
#include "tree.h"
#include "IDStack.h"
#include "node.h"
//standard help function
void help(){
	fprintf(stdout, "This program exists as a scanner to find tokens for\n"
			"a programming language and output them to a file.\n"
			"requires a file or redirected input from a file.\n"
			"OPTIONS: \n"
			"-h	: help. Also activated on bad input.\n"
			"scanner [filename]: intakes a file naturally.\n"
			"scanner < [filename]: redirects a file thru stdin.\n");
}
int argHandler(int argc, char** argv, char** fName){
	int option;
	if(argc > 4){	//case where impossible number of args
		fprintf(stderr, "Too many arguments");
		return 1;
	}
	while((option = getopt(argc, argv, "h"))!= -1){//standard -h case
		switch(option){
			case 'h': return 1;
		}
	}
	int remaining = argc - optind;//count of remaining args, assume -h 1sr
	switch(remaining){
		case 0: *fName = NULL;
			break;
		case 1: *fName = argv[optind]; 
			break;
		default: return 1;
	}
	return 0;
}

int main(int argc, char** argv){
	char* fName = NULL;
	FILE* fHandle = NULL;
	if(argHandler(argc, argv, &fName) != 0){
		help();
		return 0;
	}
	if(fName == NULL){ //takes user to keyboard entry or redirected entry
		printf("\nInputting information into a file via keyboard"
			"\nor by file input redirection.\n"
			"If user is using keyboard entry, remember to type"
			" [CTRL-D] to submit.\n");
		fHandle = stdin;
		fName = "output";
	}else{//takes file input
		printf("Intaking file %s\n", fName);
		fHandle = fopen(fName, "r");
		if(fHandle == NULL){
			fprintf(stderr, "ERROR: scanner.c: main: File does not" 
			" exist at specified location or can't be opened\n");
			return -1;
		}
	}
	node* root;
	fprintf(stdout,"parsing file.\n");
	root = parser(fHandle);
	fprintf(stdout,"parsing file complete.\n");
	fprintf(stdout,"performing static semantics check on file.\n");
	statSem(root, NULL);
	fprintf(stdout,"performing static semantics check on file complete.\n");
	deleteTree(root);
	fclose(fHandle);
	return 0;
}
