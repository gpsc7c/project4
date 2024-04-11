#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <limits.h>
#include "ttoken.h"
#include "langscan.c"
#define MAX_LENGTH 8//max length of token definition
#ifndef SCANL_H
#define SCANL_H
//prints error based on error code and where error is
void printErr(int err, int row, int column){
	int errNo = 0 - err;
	fprintf(stderr,"ERROR: scanner.h: FSADriver: %s on row %d at character %d.\n", errorTexts[errNo], row, column);
}
////sets each part of a token, uses incrementor to set token string
Ttoken tokenSetter(int tkID, char* tkStr, int tkRow, int tkCol){
	Ttoken token;
	token.ID = tkID;
	int i;
	for(i = 0; i < strlen(tkStr); i++){
		token.tokenInstance[i] = tkStr[i];
	}
	for(; i < MAX_LENGTH + 1; i++){
		token.tokenInstance[i] = '\0';
	}
	token.row = tkRow;
	token.column = tkCol;
	return token;
}

//FSADriver to generate tokens
Ttoken FSADriver(FILE* fName, char* nextChar, int row, int* col){//Arg exists for making col int a ptr
	int column = *col;  //saves the position of the first character in line
	state_t state = s1; //0 = state1 here
	state_t nextState;  //lookahead
	char tokString[9] = {'\0','\0','\0','\0','\0','\0','\0','\0','\0'};
	//string not malloc'd because it was behaving poorly when being passed back

	//while token incomplete
	while(state != FINAL){
		//consult next row of the table in langscan.h based on state
		nextState = FSAStates[state][selector(nextChar[0])];
		//if error state achieved
		if(nextState < s1){
			printErr(nextState, row, *col);
			exit(-1);
		}
		//in case of comments, skip to the end of the line
		if(nextState == s30){	
			return(tokenSetter(-1, "n/a", row, column));
		}
		//ensure length of token isn't overrun
		if(strlen(tokString) > 7){
			nextState = FINAL;
		}
		//when token is complete
		if(nextState == FINAL){
			//if ID, check for keywords
			if(state == s2){
				return(tokenSetter(kwFinder(tokString),tokString,row,column));
			}
			//else find what other token it is
			else{
				return(tokenSetter(tokFinder(state), tokString, row, column));
			}
		}
		//if token isn;t complete
		else{
			//progress state, add to string, check next character
			state = nextState;
			strcat(tokString,nextChar);
			nextChar[0] = fgetc(fName);
			//ensure line doesn't go out of bounds
			if(*col == INT_MAX){
				fprintf(stderr, "ERROR: scanner.h: FSADriver: Line too long (character count)");
				exit(-1);
			}
			//increment column
			*col = *col + 1;
		}
	}
	//errorstate
	return tokenSetter(-2, "n/a", -1, -1);	
}
//filter to remove whitespace and filter comments once identified
Ttoken scanner(FILE* fName, char* c, int* row, int* col){//row and column should start at 1 and 0, respectively
	//creates a token pointer
	Ttoken* token = malloc(sizeof(Ttoken));
	//sets token ID to unused number
	token->ID = -3;
	//get first character
	if(*row == 1 && *col == 0){
		c[0] = fgetc(fName);
		*col = *col + 1;
	}
	//repeat until a token is generated or EOF is hit
	while (c[0] != EOF || isspace(c[0])) {
		//enter if not space
		if(!isspace(c[0])){
			*token = FSADriver(fName, c, *row, col);
		}
		//increment row if end of line reached, ensure no int overflow
		if(c[0] == '\n'){
			if(*row == INT_MAX){
				fprintf(stderr, "ERROR: scanner.h: Filter: File too long (row count)");
				exit(-1);
			}
			*row = *row + 1;
			*col = 1;
		}
		//skip spaces
		if(isspace(c[0])){
			c[0] = fgetc(fName);
		}
		//skip to end of line if a comment
		if(token->ID == -1){
			while(c[0] != '\n' && c[0] != EOF){
				c[0] = fgetc(fName);
			}
			token->ID = -3;
		}
		//if token successfully retrieved
		if(token->ID >= 0){
			return tokenSetter(token->ID, token->tokenInstance, token->row, token->column);
		}
		//whitespace state
		//else{
			//return tokenSetter(-2, "n/a", -1, -1);	
		//}		
	}
	//Generate EOF token
	if (c[0] == EOF){
		*token = FSADriver(fName, c, *row, col);	
		return tokenSetter(token->ID, token->tokenInstance, token->row, token->column);
	}
	//Error state
	return tokenSetter(-2, "n/a", -1, -1);	
}

#endif
