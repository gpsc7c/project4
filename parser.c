#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <stdbool.h>
#include "scanner.h"
#include "langscan.h"
#include "ttoken.h"
#include "parser.h"
#include "node.h"
#include "tree.h"
///////enumeration reminder
#ifndef PARSER_C
#define PARSER_C
//generic error handler, forces exit after output
char nonterms[28][11] = {"TERMINAL", "<program>", "<func>", "<block>", "<vars>", "<facvars>", "<expr>", "<N>", "<N1>", "<A>", "<M>", "<R>", "<stats>", "<mStat>", "<stat>", "<in>", "<out>", "<if>", "<pick>", "<pickbody>", "<loop1>", "<loop2>", "<assign>", "<RBracket>", "<RTriplet>", "<R0>", "<label>", "<goto>"};
void error(int tkDesired, int tkReceived, int row, int col){
	char IDtags[47][25] = { /*0*/"Identifier", "Integer", "=", ">", "<", /*5*/"==", "=!=", ":", ":=", "+", /*10*/"-", "*", "/", "^", ".", /*15*/"(", ")", ",", "{", "}", /*20*/";", "[", "]", "or", "and", /*25*/"start", "stop", "while", "repeat", "until", /*30*/"label", "return", "cin", "cout", "tape", /*35*/"jump", "if", "then", "pick", "create", /*40*/"set", "func", "EOF", "<, >, ==, =!=, or ...", "int, ID, or parentheses", /*45*/": or :=" ,"unknown"};
	fprintf(stderr, "\nAn error has occurred: '%s' token was expected,\n"
			"token '%s' received. Error on row %d at column %d\n", IDtags[tkDesired], IDtags[tkReceived], row, col);
	exit(-1);
}
///////////////////////////////////////////////////
//Parse tree node for R0 (comparison/etc)//////////
///////////////////////////////////////////////////
//FORMAT IN R FUNCS IS FOLLOWED IN ALL OTHER NODE//
//RETURNING FUNCTIONS: USE AS LEGEND///////////////
///////////////////////////////////////////////////
node* R0(Ttoken* tk, FILE* file, char* c, int* row, int* col){//passing in all to keep track of location in file
	node* nNode = newNode(nonterms[25]);//creation of node of type in func name
	if(tk->ID == GREATTK){//block checks for token
		nNode->one = newTermNode(tk);//if token is correct, add it as a terminal
					     //else move on
		*tk = scanner(file, c, row, col);//move scanner along
		return nNode;//in case where this is the final token, move along
	}//nodes are assigned by the order they will be in, additional node types added where branches > 4
	if(tk->ID == LESSTK){
		nNode->one = newTermNode(tk);
		*tk = scanner(file, c, row, col);	
		return nNode;
	}
	if(tk->ID == ISEQUALTK){
		nNode->one = newTermNode(tk);
		*tk = scanner(file, c, row, col);	
		return nNode;
	}
	if(tk->ID == NOTEQUALTK){
		nNode->one = newTermNode(tk);
		*tk = scanner(file, c, row, col);	
		return nNode;
	}
	if(tk->ID == DOTTK){
		nNode->one = newTermNode(tk);
		*tk = scanner(file, c, row, col);	
		if(tk->ID == DOTTK){
			nNode->two = newTermNode(tk);
			*tk = scanner(file, c, row, col);	
			if(tk->ID == DOTTK){
				nNode->three = newTermNode(tk);
				*tk = scanner(file, c, row, col);	
				return nNode;
			}
		}
		error(14, tk->ID, tk->row, tk->column);
	}
	error(42, tk->ID, tk->row, tk->column);//in case where an error occurs
	return NULL;
}
//additional node for the <expression>/<R0>/<expression> that R0 is always a part of
node* RTriplet(Ttoken* tk, FILE* file, char* c, int* row, int* col){
	node* nNode = newNode(nonterms[24]);
	nNode->one = expr(tk, file, c, row, col);//send to next  function when what is needed is not a token
	nNode->two = R0(tk, file, c, row, col);
	nNode->three = expr(tk, file, c, row, col);
	return nNode;
}
//additional node to break down [ <RTriplet> ] as its own, as R never appears outside of this, effective end goal is to reduce R to 3s
node* RBrackets(Ttoken* tk, FILE* file, char* c, int* row, int* col){
	node* nNode = newNode(nonterms[23]);
	if(tk->ID == OPENSQUARETK){
		nNode->one = newTermNode(tk);
		*tk = scanner(file, c, row, col);	
		nNode->two = RTriplet(tk, file, c, row, col);
		if(tk->ID == CLOSESQUARETK){
			nNode->three = newTermNode(tk);
			*tk = scanner(file, c, row, col);
			return nNode;
		}
		error(22, tk->ID, tk->row, tk->column);
	}
	error(21, tk->ID, tk->row, tk->column);
	return NULL;
}
//required node for goto pattern: jumptk IDtk
node* gotochk(Ttoken* tk, FILE* file, char* c, int* row, int* col, bool* found){
	node* nNode = newNode(nonterms[27]);
	if(tk->ID == JUMPTK){
		nNode->one = newTermNode(tk);
		*tk = scanner(file, c, row, col);	
		if(tk->ID == IDTK){
			nNode->two = newTermNode(tk);
			*tk = scanner(file, c, row, col);	
			*found = true;
			return nNode;
		}
		error(0, tk->ID, tk->row, tk->column);
		return NULL;
	}
	return nNode;
}
//required node for label pattern: labeltk IDtk
node* label(Ttoken* tk, FILE* file, char* c, int* row, int* col, bool* found){
	node* nNode = newNode(nonterms[26]);
	if(tk->ID == LABELTK){
		nNode->one = newTermNode(tk);
		*tk = scanner(file, c, row, col);	
		if(tk->ID == IDTK){
			nNode->two = newTermNode(tk);
			*tk = scanner(file, c, row, col);	
			*found = true;
			return nNode;
		}
		error(0, tk->ID, tk->row, tk->column);
		return NULL;
	}
	return nNode;
}
//required node for assignment pattern: st identifier = <expr> | Idtk = <expr>
node* assign(Ttoken* tk, FILE* file, char* c, int* row, int* col, bool* found){
	node* nNode = newNode(nonterms[22]);
	bool set = false;//flag to ensure return can't be reached from missing IDTK
	if(tk->ID == SETTK){
		nNode->one = newTermNode(tk);
		*tk = scanner(file, c, row, col);
		set = true;
	}
	if(tk->ID == IDTK){
		nNode->two = newTermNode(tk);
		*tk = scanner(file, c, row, col);
		if(tk->ID == ASSIGNTK){
			nNode->three = newTermNode(tk);
			*tk = scanner(file, c, row, col);
			nNode->four = expr(tk, file, c, row, col);
			*found = true;
			return nNode;
		}
		error(2, tk->ID, tk->row, tk->column);
	}
	if(set == true){
		error(0, tk->ID, tk->row, tk->column);
	}
	return NULL;
}
//required node for loop2 pattern: Repeat <stat> Until RBrackets
node* loop2(Ttoken* tk, FILE* file, char* c, int* row, int* col, bool* found){
	node* nNode = newNode(nonterms[21]);
	if(tk->ID == REPEATTK){
		nNode->one = newTermNode(tk);
		*tk = scanner(file, c, row, col);
		nNode->two = stat(tk, file, c, row, col);
		if(tk->ID == UNTILTK){
			nNode->three = newTermNode(tk);
			*tk = scanner(file, c, row, col);
			nNode->four = RBrackets(tk, file, c, row, col);
			*found = true;
			return nNode;
		}
		error(29, tk->ID, tk->row, tk->column);
	}
	return NULL;	
}
//required node for loop1 pattern: while <RBrackets> <stat>
node* loop1(Ttoken* tk, FILE* file, char* c, int* row, int* col, bool* found){
	node* nNode = newNode(nonterms[20]);
	if(tk->ID == WHILETK){
		nNode->one = newTermNode(tk);
		*tk = scanner(file, c, row, col);
		nNode->two = RBrackets(tk, file, c, row, col);
		nNode->three = stat(tk, file, c, row, col);
		*found = true;
		return nNode;
	}
	return NULL;
}
//required node for pickbody pattern: <stat> : <stat>
node* pickbody(Ttoken* tk, FILE* file, char* c, int* row, int* col){
	node* nNode = newNode(nonterms[19]);
	nNode->one = stat(tk, file, c, row, col);
	if(tk->ID == COLONTK){
		nNode->two = newTermNode(tk);
		*tk = scanner(file, c, row, col);	
		nNode->three = stat(tk, file, c, row, col);
		return nNode;
	}
	error(7, tk->ID, tk->row, tk->column);
	return NULL;
}
//required node for pick pattern: Repeat <stat> Until <RBrackets>
node* pick(Ttoken* tk, FILE* file, char* c, int* row, int* col, bool* found){
	node* nNode = newNode(nonterms[18]);
	if(tk->ID == PICKTK){
		nNode->one = newTermNode(tk);
		*tk = scanner(file, c, row, col);	
		nNode->two = expr(tk, file, c, row, col);
		nNode->three = pickbody(tk, file, c, row, col);
		*found = true;
		return nNode;
	}
	return NULL;
}
//required node for if pattern: If <RBrackets> then <stat>
node* ifchk(Ttoken* tk, FILE* file, char* c, int* row, int* col, bool* found){
	node* nNode = newNode(nonterms[17]);
	if(tk->ID == IFTK){
		nNode->one = newTermNode(tk);	
		*tk = scanner(file, c, row, col);	
		nNode->two = RBrackets(tk, file, c, row, col);
		if(tk->ID == THENTK){
			nNode->three = newTermNode(tk);
			*tk = scanner(file, c, row, col);
			nNode->four = stat(tk, file, c, row, col);
			*found = true;
			return nNode;
		}	
		error(37, tk->ID, tk->row, tk->column);
		return NULL;
	}
	return NULL;
}
//required node for out pattern: cout <expression>
node* out(Ttoken* tk, FILE* file, char* c, int* row, int* col, bool* found){
	node* nNode = newNode(nonterms[16]);
	if(tk->ID == COUTTK){
		nNode->one = newTermNode(tk);
		*tk = scanner(file, c, row, col);	
		nNode->two = expr(tk, file, c, row, col);
		*found = true;
		return nNode;
	}
	return NULL;
}
//required node for in pattern: cin Identidier
node* in(Ttoken* tk, FILE* file, char* c, int* row, int* col, bool* found){
	node* nNode = newNode(nonterms[15]);
	if(tk->ID == CINTK){
		nNode->one = newTermNode(tk);
		*tk = scanner(file, c, row, col);	
		if(tk->ID == IDTK){
			nNode->two = newTermNode(tk);
			*tk = scanner(file, c, row, col);
			*found = true;
			return nNode;
		}
		error(0, tk->ID, tk->row, tk->column);
		return NULL;
	}
	return NULL;
}
//required node for stat pattern: <in> ; | <out> ; | <block | <if> ; | <loop1> ; | <loop2> ; | <assign> ; | <goto> ; } <label> ; | <pick> ; 
node* stat(Ttoken* tk, FILE* file, char* c, int* row, int* col){
	node* nNode = newNode(nonterms[14]);
	bool* found = malloc(sizeof(bool));
	*found = false;
	if(tk->ID != OPENCURLTK){
		nNode->one = in(tk,file,c,row,col,found);
		if(!*found){
			nNode->one = out(tk,file,c,row,col,found);
		}
		if(!*found){
			nNode->one = ifchk(tk,file,c,row,col,found);
		}
		if(!*found){
			nNode->one = pick(tk,file,c,row,col,found);
		}
		if(!*found){
			nNode->one = loop1(tk,file,c,row,col,found);
		}
		if(!*found){
			nNode->one = loop2(tk,file,c,row,col,found);
		}
		if(!*found){
			nNode->one = assign(tk,file,c,row,col,found);
		}
		if(!*found){
			nNode->one = label(tk,file,c,row,col,found);
		}
		if(!*found){
			nNode->one = gotochk(tk,file,c,row,col,found);
		}
		if(tk->ID == SEMICOLONTK){
			nNode->two = newTermNode(tk);
			*tk = scanner(file, c, row, col);
			return nNode;
		}
		error(20, tk->ID, tk->row, tk->column);
		return NULL;
	}
	nNode->one = block(tk, file, c, row, col);
	return nNode;
}
//required node for mStat pattern: [empty] | <stat> <mStat>
node* mStat(Ttoken* tk, FILE* file, char* c, int* row, int* col){
	node* nNode = newNode(nonterms[13]);
	if(tk->ID == IDTK || tk->ID == CINTK || tk->ID == COUTTK ||tk->ID == OPENCURLTK ||tk->ID == IFTK ||tk->ID == WHILETK ||tk->ID == REPEATTK ||tk->ID == SETTK ||tk->ID == JUMPTK ||tk->ID == LABELTK ||tk->ID == PICKTK){		
		nNode->one = stat(tk, file, c, row, col);
		nNode->two = mStat(tk, file, c, row, col);
	}
	return nNode;
}
//required node for stats pattern: <stat> <mStat>
node* stats(Ttoken* tk, FILE* file, char* c, int* row, int* col){	
	node* nNode = newNode(nonterms[12]);
	nNode->one = stat(tk, file, c, row, col);
	nNode->two = mStat(tk, file, c, row, col);
	return nNode;
}
//required node for R pattern: ( <expression> ) | IDTK | INTTK
node* R(Ttoken* tk, FILE* file, char* c, int* row, int* col){
	node* nNode = newNode(nonterms[11]);
	if(tk->ID == OPENPARENTK){
		nNode->one = newTermNode(tk);
		*tk = scanner(file, c, row, col);
		nNode->two = expr(tk, file, c, row, col);
		if(tk->ID == CLOSEPARENTK){
			nNode->three = newTermNode(tk);
			*tk = scanner(file, c, row, col);
			return nNode;
		}
		error(0, tk->ID, tk->row, tk->column);
		return NULL;
	}
	if(tk->ID == IDTK){
		nNode->one = newTermNode(tk);
		*tk = scanner(file, c, row, col);
		return nNode;
	}
	if(tk->ID == INTTK){
		nNode->one = newTermNode(tk);
		*tk = scanner(file, c, row, col);
		return nNode;
	}
	error(44, tk->ID, tk->row, tk->column);
	return NULL;
}
//required node for M pattern: ^ <M> | <R>
node* M(Ttoken* tk, FILE* file, char* c, int* row, int* col){
	node* nNode = newNode(nonterms[10]);
	if(tk->ID == EXPONTK){
		nNode->one = newTermNode(tk);
		*tk = scanner(file, c, row, col);
		nNode->two = M(tk, file, c, row, col);
		return nNode;
	}
	nNode->one = R(tk, file, c, row, col);
	return nNode;
}
//required node for A pattern: <M> * <A> | <M>
node* A(Ttoken* tk, FILE* file, char* c, int* row, int* col){
	node* nNode = newNode(nonterms[9]);
	nNode->one = M(tk, file, c, row, col);
	if(tk->ID == MULTIPLYTK){
		nNode->two = newTermNode(tk);
		*tk = scanner(file, c, row, col);
		nNode->three = A(tk, file, c, row, col);
	}
	return nNode;
}
//additional left recursion-removing node for N1 pattern: / <A> <N1> | - <A> <N1> | [empty]
node* N1(Ttoken* tk, FILE* file, char* c, int* row, int* col){
	node* nNode = newNode(nonterms[8]);
	if(tk->ID == DIVIDETK){
		nNode->one = newTermNode(tk);
		*tk = scanner(file, c, row, col);
		nNode->two = A(tk, file, c, row, col);
		nNode->three = N1(tk, file, c, row, col);
		return nNode;
	}
	if(tk->ID == PLUSTK){
		nNode->one = newTermNode(tk);
		*tk = scanner(file, c, row, col);
		nNode->two = A(tk, file, c, row, col);
		nNode->three = N1(tk, file, c, row, col);
		return nNode;
	}
	return nNode;
}
//edited for left recursion required node for N pattern: <A> <N1> 
node* N(Ttoken* tk, FILE* file, char* c, int* row, int* col){
	node* nNode = newNode(nonterms[7]);
	nNode->one = A(tk, file, c, row, col);
	nNode->two = N1(tk, file, c, row, col);
	return nNode;
}
//required node for expression pattern: <N> - <expression> | <N>
node* expr(Ttoken* tk, FILE* file, char* c, int* row, int* col){
	node* nNode = newNode(nonterms[6]);
	nNode->one = N(tk, file, c, row, col);
	if(tk->ID == MINUSTK){
		nNode->two = newTermNode(tk);
		*tk = scanner(file, c, row, col);
		nNode->three = expr(tk, file, c, row, col);
	}
	return nNode;
}
//additional node for shrinking to 4 branch nodes for facvars pattern: create IDTK := | create IDTk;
node* facvars(Ttoken* tk, FILE* file, char* c, int* row, int* col, bool* coleql){
	node* nNode = newNode(nonterms[5]);
	if(tk->ID == CREATETK){
		nNode->one = newTermNode(tk);
		*tk = scanner(file, c, row, col);
		if(tk->ID == IDTK){
			nNode->two = newTermNode(tk);
			*tk = scanner(file, c, row, col);
			if(tk->ID == COLONEQLTK){
				nNode->three = newTermNode(tk);
				*tk = scanner(file, c, row, col);
				*coleql = true;
				return nNode;
			}
			if(tk->ID ==SEMICOLONTK){
				nNode->three = newTermNode(tk);
				*tk = scanner(file, c, row, col);
				return nNode;	
			}
			error(45, tk->ID, tk->row, tk->column);
		}
		error(0, tk->ID, tk->row, tk->column);
	}
	error(39, tk->ID, tk->row, tk->column);
	return NULL;
}
//edited required node for vars pattern: [empty] | <facvars> | <facvars> INTTK ; <vars>
node* vars(Ttoken* tk, FILE* file, char* c, int* row, int* col){
	node* nNode = newNode(nonterms[4]);
	bool* coleql = malloc(sizeof(bool));//awkward implementation to shrink size
	*coleql = false;
	if(tk->ID == CREATETK){
		nNode->one = facvars(tk, file, c, row, col, coleql);
		if(*coleql == false){
			return nNode;
		}
		if(tk->ID == INTTK){
			nNode->two = newTermNode(tk);
			*tk = scanner(file, c, row, col);
			if(tk->ID == SEMICOLONTK){
				nNode->three = newTermNode(tk);
				*tk = scanner(file, c, row, col);
				nNode->four = vars(tk, file, c, row, col);
				return nNode;
			}
		}
		error(1, tk->ID, tk->row, tk->column);
		return NULL;
	}
	return nNode;
}
//required node for block pattern: { <vars> <stats }
node* block(Ttoken* tk, FILE* file, char* c, int* row, int* col){
	node* nNode = newNode(nonterms[3]);
	if(tk->ID != OPENCURLTK){
		error(18, tk->ID, tk->row, tk->column);
		return NULL;
	}
	nNode->one = newTermNode(tk);
	*tk = scanner(file, c, row, col);
	nNode->two = vars(tk,file,c,row,col);
	nNode->three = stats(tk,file,c,row,col);
	if(tk->ID != CLOSECURLTK){
		error(19, tk->ID, tk->row, tk->column);
		return NULL;
	}
	nNode->four = newTermNode(tk);	
	*tk = scanner(file, c, row, col);
	return nNode;
}
//required node for func pattern: func IDTK <block>
node* func(Ttoken* tk, FILE* file, char* c, int* row, int* col){
	node* nNode = newNode(nonterms[2]);
	if(tk->ID == FUNCTK){	
		nNode->one = newTermNode(tk);
		*tk = scanner(file, c, row, col);
		if(tk->ID == IDTK){
			nNode->two = newTermNode(tk);
			*tk = scanner(file, c, row, col);
			nNode->three = block(tk, file, c, row, col);
			return nNode;
		}
		error(0, tk->ID, tk->row, tk->column);
	}
	error(41, tk->ID, tk->row, tk->column);
	return NULL;
}
//required node for program pattern: <vars> tape <func> <block | <vars> tape <block>
node* program(Ttoken* tk, FILE* file, char* c, int* row, int* col){
	node* root = newNode(nonterms[1]);
	root->one = vars(tk,file,c,row,col);
	if(tk->ID == TAPETK){
		root->two = newTermNode(tk);
		*tk = scanner(file, c, row, col);
		
		if(tk->ID == FUNCTK){
			root->three = func(tk,file,c,row,col);
		}
		root->four = block(tk,file,c,row,col);
		return root;
	}
	error(34, tk->ID, tk->row, tk->column);
	return NULL;
}
//calls the parser to generate the tree
node* parser(FILE* file){
	//create malloc'd variables to send to other functions
	int *col = malloc(sizeof(int));
	int *row = malloc(sizeof(int));
	//int depth = 0;	//variable for testing parse tree structure
	Ttoken* tk = malloc(sizeof(Ttoken));
	*col = 0;
	*row = 1;
	char c[2] = {'\0','\0'};		//character iterator
	node* root;
	*tk = scanner(file, c, row, col);
	root = program(tk, file, c, row, col);
	return root;
}
#endif
