#ifndef TTOKEN_H
#define TTOKEN_H
//enumeration for the possible IDs for a token
enum tokenTypes{IDTK, INTTK, ASSIGNTK, GREATTK, LESSTK, ISEQUALTK, NOTEQUALTK, COLONTK, COLONEQLTK, PLUSTK, MINUSTK, MULTIPLYTK, DIVIDETK, EXPONTK, DOTTK, OPENPARENTK, CLOSEPARENTK, COMMATK, OPENCURLTK, CLOSECURLTK, SEMICOLONTK, OPENSQUARETK, CLOSESQUARETK, ORTK, ANDTK, STARTTK, STOPTK, WHILETK, REPEATTK, UNTILTK, LABELTK, RETURNTK, CINTK, COUTTK, TAPETK, JUMPTK, IFTK, THENTK, PICKTK, CREATETK, SETTK, FUNCTK, EOFTK};
//array corresponding to output tokens so that their name can be output

typedef struct {
	int ID;
	char tokenInstance[9];//This is the actual token, not the Token ID, yes?
	int row;
	int column;	
}Ttoken;
#endif
