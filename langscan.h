#include <stdlib.h>
#include <stdio.h>
#include<string.h>
#include <ctype.h>
#include "ttoken.h"
#ifndef LANGSCAN_H
#define LANGSCAN_H
typedef int state_t;
//numbers for the state table that is further in this file, for enumeration and statecchecking
enum statenums {s1=0,s2,s3,s4,s5,s6,s7,s8,s9,s10,s11,s12,s13,s14,s15,s16,s17,s18,s19,s20,s21,s22,s23,s24,s25,s26,s27,s28,s29,s30};
//enumeration for the different characters that can be entered
enum inputnums {ALPHA = 0, NUMBER, UNDERSCORE, EQUALS, GREATER, LESS, EXCLAMATION, COLON, PLUS, MINUS, MULTI, DIVIDE, EXPONENT, DOT, OPENPAREN, CLOSEPAREN, COMMA, OPENCURL, CLOSECURL, SEMICOLON, OPENSQUARE, CLOSESQUARE, PIPESYMBOL, ANDSYMBOL, WHITESPACE, OTHER, ENDOFFILE};
#define FINAL 1000
//enumeration for number codes to spit out errors
enum errnums { e1 = -1, e2 = -2, e3 = -3, e4 = -4, e5 = -5, e6 = -6, e7 = -7};
//texts relative to errors related to the table
char* errorTexts[8];



//This is the state tableused in all calls to/for state
int FSAStates[30][27];
int tokFinder(int state);
int selector(char nextChar);
int kwFinder(char* tokString);

#endif
