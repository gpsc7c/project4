#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <stdbool.h>
#include "langscan.h"
#include "ttoken.h"
#include "node.h"
#ifndef PARSER_H
#define PARSER_H

void error(int tkDesired, int tkRecieved, int row, int col);

node* gotochk(Ttoken* tk, FILE* file, char* c, int* row, int* col, bool* found);
node* label(Ttoken* tk, FILE* file, char* c, int* row, int* col, bool* found);
node* R0(Ttoken* tk, FILE* file, char* c, int* row, int* col);
node* RTriplet(Ttoken* tk, FILE* file, char* c, int* row, int* col);
node* RBrackets(Ttoken* tk, FILE* file, char* c, int* row, int* col);
node* assign(Ttoken* tk, FILE* file, char* c, int* row, int* col, bool* found);
node* loop2(Ttoken* tk, FILE* file, char* c, int* row, int* col, bool* found);
node* loop1(Ttoken* tk, FILE* file, char* c, int* row, int* col, bool* found);
node* pickbody(Ttoken* tk, FILE* file, char* c, int* row, int* col);
node* pick(Ttoken* tk, FILE* file, char* c, int* row, int* col, bool* found);
node* ifchk(Ttoken* tk, FILE* file, char* c, int* row, int* col, bool* found);
node* out(Ttoken* tk, FILE* file, char* c, int* row, int* col, bool* found);
node* in(Ttoken* tk, FILE* file, char* c, int* row, int* col, bool* found);
node* stat(Ttoken* tk, FILE* file, char* c, int* row, int* col);
node* mStat(Ttoken* tk, FILE* file, char* c, int* row, int* col);
node* stats(Ttoken* tk, FILE* file, char* c, int* row, int* col);
node* R(Ttoken* tk, FILE* file, char* c, int* row, int* col);
node* M(Ttoken* tk, FILE* file, char* c, int* row, int* col);
node* A(Ttoken* tk, FILE* file, char* c, int* row, int* col);
node* N(Ttoken* tk, FILE* file, char* c, int* row, int* col);
node* expr(Ttoken* tk, FILE* file, char* c, int* row, int* col);
node* facvars(Ttoken* tk,FILE* file, char* c, int* row, int* col, bool* coleql);
node* vars(Ttoken* tk, FILE* file, char* c, int* row, int* col);
node* block(Ttoken* tk, FILE* file, char* c, int* row, int* col);
node* func(Ttoken* tk, FILE* file, char* c, int* row, int* col);
node* program(Ttoken* tk, FILE* file, char* c, int* row, int* col);
node* parser(FILE* file);
#endif
