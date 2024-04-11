#include <stdlib.h>
#include <stdio.h>
#include<string.h>
#include <ctype.h>
#include "ttoken.h"
#ifndef LANGSCAN_C
#define LANGSCAN_C
typedef int state_t;
//numbers for the state table that is further in this file, for enumeration and statecchecking
enum statenums {s1=0,s2,s3,s4,s5,s6,s7,s8,s9,s10,s11,s12,s13,s14,s15,s16,s17,s18,s19,s20,s21,s22,s23,s24,s25,s26,s27,s28,s29,s30};
//enumeration for the different characters that can be entered
enum inputnums {ALPHA = 0, NUMBER, UNDERSCORE, EQUALS, GREATER, LESS, EXCLAMATION, COLON, PLUS, MINUS, MULTI, DIVIDE, EXPONENT, DOT, OPENPAREN, CLOSEPAREN, COMMA, OPENCURL, CLOSECURL, SEMICOLON, OPENSQUARE, CLOSESQUARE, PIPESYMBOL, ANDSYMBOL, WHITESPACE, OTHER, ENDOFFILE};
#define FINAL 1000
//enumeration for number codes to spit out errors
enum errnums { e1 = -1, e2 = -2, e3 = -3, e4 = -4, e5 = -5, e6 = -6, e7 = -7};
//texts relative to errors related to the table
char* errorTexts[8] = {"Unknown Error", "No Token starts with '_'", "No Token starts with '!'", "Unusable character in this language", "Expected '=' symbol to complete '=!=' token", "Expected second '|' symbol to complete '||' token", "Expected second '&' symbol to complete '&&' token", "Whitespace character in initial intake, fatal compiler error"};


//This is the state tableused in all calls to/for state
int FSAStates[30][27] = {/*
/////
A-Z    #s    _     =     >     <     !     :     +     -     *     /     ^     .     (     )     ,     {     }     ;     [     ]     |     &     WS    ELSE  EOF
/////
*/
{s2,   s3,   e1,   s4,   s7,   s8,   e2,   s9,   s11,  s12,  s13,  s14,  s15,  s16,  s17,  s18,  s19,  s20,  s21,  s22,  s23,  s24,  s25,  s27,  e7,   e3,   FINAL},//s1
{s2,   s2,   s2,   FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL},//s2
{FINAL,s3,   FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL},//s3
{FINAL,FINAL,FINAL,s5,   FINAL,FINAL,s6,   FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL},//s4
{FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL},//s5
{e4,   e4,   e4,   s29,  e4,   e4,   e4,   e4,   e4,   e4,   e4,   e4,   e4,   e4,   e4,   e4,   e4,   e4,   e4,   e4,   e4,   e4,   e4,   e4,   e4,   e4,   e4},   //s6
{FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL},//s7
{FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL},//s8
{FINAL,FINAL,FINAL,s10  ,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL},//s9
{FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL},//s10
{FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL},//s11
{FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL},//s12
{FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL},//s13
{FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,s30  ,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL},//s14
{FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL},//s15
{FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL},//s16
{FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL},//s17
{FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL},//s18
{FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL},//s19
{FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL},//s20
{FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL},//s21
{FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL},//s22
{FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL},//s23
{FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL},//s24
{e5,   e5,   e5,   e5,   e5,   e5,   e5,   e5,   e5,   e5,   e5,   e5,   e5,   e5,   e5,   e5,   e5,   e5,   e5,   e5,   e5,   e5,   s26,  e5,   e5,   e5,   e5},   //s25
{FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL},//s26
{e6,   e6,   e6,   e6,   e6,   e6,   e6,   e6,   e6,   e6,   e6,   e6,   e6,   e6,   e6,   e6,   e6,   e6,   e6,   e6,   e6,   e6,   e6,   s28,  e6,   e6,   e6},   //s27
{FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL},//s28
{FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL},//s29
{FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL,FINAL},//s29
};

///////////////////////////////////////////////////////////////////////////////
/////tokfinder exists to return, by number, the token that the state would/////
/////reference/////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
//enum tokenTypes{IDTK, INTTK, ASSIGNTK, GREATTK, LESSTK, ISEQUALTK, NOTEQUALTK, COLONTK, COLONEQLTK, PLUSTK, MINUSTK, MULTIPLYTK, DIVIDETK, EXPONTK, DOTTK, OPENPARENTK, CLOSEPARENTK, COMMATK, OPENCURLTK, CLOSECURLTK, SEMICOLONTK, OPENSQUARETK, CLOSESQUARETK, ORTK, ANDTK, STARTTK, STOPTK, WHILETK, REPEATTK, UNTILTK, LABELTK, RETURNTK, CINTK, COUTTK, TAPETK, JUMPTK, IFTK, THENTK, PICKTK, CREATETK, SETTK, FUNCTK, EOFTK};
//above is a reference to the tokentypes enumeration from ttoken.h
int tokFinder(int state){
	if(state == s3){
		return INTTK;
	}
	if(state == s4){
		return ASSIGNTK;
	}
	if(state == s5){
		return ISEQUALTK;
	}
	if(state == s29){
		return NOTEQUALTK;
	}
	if(state == s7){
		return GREATTK;
	}
	if(state == s8){
		return LESSTK;
	}
	if(state == s9){
		return COLONTK;
	}
	if(state == s10){
		return COLONEQLTK;
	}
	if(state == s11){
		return PLUSTK;
	}
	if(state == s12){
		return MINUSTK;
	}
	if(state == s13){
		return MULTIPLYTK;
	}
	if(state == s14){
		return DIVIDETK;
	}
	if(state == s15){
		return EXPONTK;
	}
	if(state == s16){
		return DOTTK;
	}
	if(state == s17){
		return OPENPARENTK;
	}
	if(state == s18){
		return CLOSEPARENTK;
	}
	if(state == s19){
		return COMMATK;
	}
	if(state == s20){
		return OPENCURLTK;
	}
	if(state == s21){
		return CLOSECURLTK;
	}
	if(state == s22){
		return SEMICOLONTK;
	}
	if(state == s23){
		return OPENSQUARETK;
	}
	if(state == s24){
		return CLOSESQUARETK;
	}
	if(state == s26){
		return ORTK;
	}
	if(state == s28){
		return ANDTK;
	}
	
	return EOFTK;
}
///////////////////////////////////////////////////////////////////////////////
/////selector returns the kind of character as an enumeration//////////////////
///////////////////////////////////////////////////////////////////////////////
//enum inputnums {ALPHA = 0, NUMBER, UNDERSCORE, EQUALS, GREATER, LESS, EXCLAMATION, COLON, PLUS, MINUS, MULTI, DIVIDE, EXPONENT, DOT, OPENPAREN, CLOSEPAREN, COMMA, OPENCURL, CLOSECURL, SEMICOLON, OPENSQUARE, CLOSESQUARE, PIPESYMBOL, ANDSYMBOL, WHITESPACE, OTHER, ENDOFFILE};
//above is a reference to the enumeration from earlier in this file
int selector(char nextChar){
	if(isalpha(nextChar)){
		return ALPHA;
	}
	if(isdigit(nextChar)){
		return NUMBER;
	}
	if(nextChar == '_'){
		return UNDERSCORE;
	}
	if(nextChar == '='){
		return EQUALS;
	}
	if(nextChar == '>'){
		return GREATER;
	}
	if(nextChar == '<'){
		return LESS;
	}
	if(nextChar == '!'){
		return EXCLAMATION;
	}
	if(nextChar == ':'){
		return COLON;
	}
	if(nextChar == '+'){
		return PLUS;
	}
	if(nextChar == '-'){
		return MINUS;
	}
	if(nextChar == '*'){
		return MULTI;
	}
	if(nextChar == '/'){
		return DIVIDE;
	}
	if(nextChar == '^'){
		return EXPONENT;
	}
	if(nextChar == '.'){
		return DOT;
	}
	if(nextChar == '('){
		return OPENPAREN;
	}
	if(nextChar == ')'){
		return CLOSEPAREN;
	}
	if(nextChar == ','){
		return COMMA;
	}
	if(nextChar == '{'){
		return OPENCURL;
	}
	if(nextChar == '}'){
		return CLOSECURL;
	}
	if(nextChar == ';'){
		return SEMICOLON;
	}
	if(nextChar == '['){
		return OPENSQUARE;
	}
	if(nextChar == ']'){
		return CLOSESQUARE;
	}
	if(nextChar == '|'){
		return PIPESYMBOL;
	}
	if(nextChar == '&'){
		return ANDSYMBOL;
	}
	if(isspace(nextChar)){
		return WHITESPACE;
	}
	if(nextChar == EOF){
		return ENDOFFILE;
	}

	return OTHER;
}
///////////////////////////////////////////////////////////////////////////////
/////As tokFinder, exclusive to keywords///////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
//enum tokenTypes{IDTK, INTTK, ASSIGNTK, GREATTK, LESSTK, ISEQUALTK, NOTEQUALTK, COLONTK, COLONEQLTK, PLUSTK, MINUSTK, MULTIPLYTK, DIVIDETK, EXPONTK, DOTTK, OPENPARENTK, CLOSEPARENTK, COMMATK, OPENCURLTK, CLOSECURLTK, SEMICOLONTK, OPENSQUARETK, CLOSESQUARETK, ORTK, ANDTK, STARTTK, STOPTK, WHILETK, REPEATTK, UNTILTK, LABELTK, RETURNTK, CINTK, COUTTK, TAPETK, JUMPTK, IFTK, THENTK, PICKTK, CREATETK, SETTK, FUNCTK, EOFTK};
//reference, once again, to enumeration
int kwFinder(char* tokString){
	if(strcmp(tokString, "start") == 0){
		return STARTTK;
	}
	if(strcmp(tokString, "stop") == 0){
		return STOPTK;
	}
	if(strcmp(tokString, "while") == 0){
		return WHILETK;
	}
	if(strcmp(tokString, "repeat") == 0){
		return REPEATTK;
	}
	if(strcmp(tokString, "until") == 0){
		return UNTILTK;
	}
	if(strcmp(tokString, "cin") == 0){
		return CINTK;
	}
	if(strcmp(tokString, "cout") == 0){
		return COUTTK;
	}
	if(strcmp(tokString, "tape") == 0){
		return TAPETK;
	}	
	if(strcmp(tokString, "label") == 0){
		return LABELTK;
	}
	if(strcmp(tokString, "jump") == 0){
		return JUMPTK;
	}
	if(strcmp(tokString, "if") == 0){
		return IFTK;
	}
	if(strcmp(tokString, "then") == 0){
		return THENTK;
	}
	if(strcmp(tokString, "pick") == 0){
		return PICKTK;
	}
	if(strcmp(tokString, "create") == 0){
		return CREATETK;
	}
	if(strcmp(tokString, "set") == 0){
		return SETTK;
	}
	if(strcmp(tokString, "func") == 0){
		return FUNCTK;
	}
	return IDTK;
}

#endif
