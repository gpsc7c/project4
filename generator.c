#ifndef GENERATOR_C
#define GENERATOR_C
#include <stdio.h>
#include "generator.h"
#include "tree.h"
#include "node.h"
#include "ttoken.h"
#include "IDStack.h"

static int varsCount = 0;	//counts unique vars generated
static int labelCount = 0;	//counts unique labels generated
#define NAMELEN 20
char gennonterms[28][11] = {"TERMINAL", "<program>", "<func>", "<block>", "<vars>", "<facvars>",/*5*/ "<expr>", "<N>", "<N1>", "<A>", "<M>",/*10*/ "<R>", "<stats>", "<mStat>", "<stat>", "<in>",/*15*/ "<out>", "<if>", "<pick>", "<pickbody>", "<loop1>",/*20*/ "<loop2>", "<assign>", "<RBracket>", "<RTriplet>", "<R0>",/*25*/ "<label>", "<goto>"};
enum nodetypes {TERMt, PROGt, FUNCt, BLOCt, VARSt, FACVt, EXPRt, Nt, N1t, At, Mt, Rt, STTSt, MSTAt, STATt, INt, OUTt, IFt, PICKt, PICBt, LOOP1t, LOOP2t, ASSIt, RBRAt, RTRIt, R0t, LABEt, GOTOt};
char varName[NAMELEN];
char* plusVar(){
	sprintf(varName, "T%d", varsCount++);
	return varName;
}
char labelName[NAMELEN];
char* plusLabel(){
	sprintf(labelName, "L%d", labelCount++);
	return labelName;
}
node* funcNode;
void recGen(node* dataNode, int* IDCount, FILE* out){
	int i;
	char label[NAMELEN], label2[NAMELEN], label3[NAMELEN], label4[NAMELEN], name[NAMELEN], name2[NAMELEN];
	int nodeType = -1;
	int* row = malloc(sizeof(int));
	int* col = malloc(sizeof(int));
	*row = 0;
	*col = 0;
	if(dataNode == NULL){
		return;
	}
	for(i = 0; i < 28 && nodeType == -1; i++){
		if(strcmp(gennonterms[i], dataNode->tTitle.nonterm) == 0){
			nodeType = i;
		}
	}
	//traverse parse tree and perform static semantics/generate code output
	switch(nodeType){
	case INt:	strcpy(name, plusVar());
			chkNode(dataNode, IDCount, out);
			fprintf(out, "\tREAD %s\n", name);
			fprintf(out, "\tLOAD %s\n", name);
			fprintf(out, "\tSTACKW %d\n", find(dataNode->two->tk, row, col));
			break;
	case OUTt:	strcpy(name, plusVar());
			statSem(dataNode->two, IDCount, out);
			fprintf(out, "\tSTORE %s\n", name);
			fprintf(out, "\tWRITE %s\n", name);
			break;
	case EXPRt:	if(dataNode->two != NULL){
				strcpy(name, plusVar());
				statSem(dataNode->three, IDCount, out);
				fprintf(out, "\tSTORE %s\n", name);
				statSem(dataNode->one, IDCount, out);
				fprintf(out, "\tSUB %s\n", name);
			}
			else{
				statSem(dataNode->one, IDCount, out);
			}
			break;
	case N1t:	if(dataNode->one != NULL && dataNode->one->tk->ID == DIVIDETK){
				strcpy(name, plusVar());
				fprintf(out, "\tSTORE %s\n", name);
				strcpy(name2, plusVar());
				statSem(dataNode->two, IDCount, out);
				fprintf(out, "\tSTORE %s\n", name2);
				fprintf(out, "\tLOAD %s\n", name);
				fprintf(out, "\tDIV %s\n", name2);
				statSem(dataNode->three, IDCount, out);
			}
			else if(dataNode->one != NULL && dataNode->one->tk->ID == PLUSTK){
				strcpy(name, plusVar());
				fprintf(out, "\tSTORE %s\n", name);
				strcpy(name2, plusVar());
				statSem(dataNode->two, IDCount, out);
				fprintf(out, "\tSTORE %s\n", name2);
				fprintf(out, "\tLOAD %s\n", name);
				fprintf(out, "\tADD %s\n", name2);
				statSem(dataNode->three, IDCount, out);
			}
			break;
	case At:	if(dataNode->two != NULL){
				strcpy(name, plusVar());
				statSem(dataNode->three, IDCount, out);
				fprintf(out, "\tSTORE %s\n", name);
				statSem(dataNode->one, IDCount, out);
				fprintf(out, "\tMULT %s\n", name);
			}
			else{
				statSem(dataNode->one, IDCount, out);
			}
			break;
	case Mt:	if(dataNode->one->tk != NULL && dataNode->one->tk->ID == EXPONTK){
				strcpy(label, plusLabel());
				strcpy(label2, plusLabel());
				strcpy(label3, plusLabel());
				statSem(dataNode->two, IDCount, out);
				fprintf(out, "\tBRZERO %s\n", label);
				fprintf(out, "\tSUB 1\n");
				fprintf(out, "\tBRZERO %s\n", label2);
				fprintf(out, "\tADD 1\n");
				fprintf(out, "\tMULT -1\n");
				fprintf(out, "\tBR %s\n", label3);
				fprintf(out, "%s:\tADD 1\n", label);
				fprintf(out, "\tBR %s\n", label3);
				fprintf(out, "%s:\tSUB 1\n", label2);
				fprintf(out, "\tBR %s\n", label3);
				fprintf(out, "%s:\tNOOP\n", label3);	
			}
			statSem(dataNode->one, IDCount, out);
			break;
	case Rt:	chkNode(dataNode, IDCount, out);
			if(dataNode->one->tk->ID == OPENPARENTK){
				statSem(dataNode->two, IDCount, out);
			}
			else if(dataNode->one->tk->ID == INTTK){
				fprintf(out, "\tLOAD %s\n", dataNode->one->tk->tokenInstance);
			}
			else if(dataNode->one->tk->ID == IDTK){
				fprintf(out, "\tSTACKR %d\n", find(dataNode->one->tk, row, col));
			}
			break;
	case ASSIt:	statSem(dataNode->four, IDCount, out);
			fprintf(out, "\tSTACKW %d\n", find(dataNode->two->tk, row, col));
			break;
	case LABEt:	chkNode(dataNode, IDCount, out);
			fprintf(out, "%s:\tNOOP\n", dataNode->two->tk->tokenInstance);
			break;
	case GOTOt:	strcpy(name, plusVar());	
			if(funcNode == NULL || strcmp(funcNode->two->tk->tokenInstance, dataNode->two->tk->tokenInstance) != 0){	
				fprintf(out, "\tBR %s\n", name);
				break;
			}
	case FUNCt:	if(funcNode == NULL){
				fprintf(out, "\tLOAD 0\n");
				fprintf(out, "\tPUSH\n");
				fprintf(out, "\tSTACKW 0\n");	
				funcNode = dataNode;
				chkNode(dataNode, IDCount, out);
			}
			else{
				statSem(funcNode->one, IDCount, out);
				statSem(funcNode->two, IDCount, out);
				statSem(funcNode->three, IDCount, out);
				statSem(funcNode->four, IDCount, out);
			}
			break;
	case IFt:	statSem(dataNode->two->two->three, IDCount, out);
			strcpy(name, plusVar());
			fprintf(out, "\tSTORE %s\n", name);		
			statSem(dataNode->two->two->one, IDCount, out);
			fprintf(out, "\tSUB %s\n", name);
			strcpy(label, plusLabel());
			if(dataNode->two->two->two->one->tk->ID == ISEQUALTK){
				fprintf(out, "\tBRNEG %s\n", label);
				fprintf(out, "\tBRPOS %s\n", label);
			}
			if(dataNode->two->two->two->one->tk->ID == NOTEQUALTK){
				fprintf(out, "\tBRZERO %s\n", label);
			}
			if(dataNode->two->two->two->one->tk->ID == GREATTK){
				fprintf(out, "\tBRZNEG %s\n", label);
			}
			if(dataNode->two->two->two->one->tk->ID == LESSTK){
				fprintf(out, "\tBRZPOS %s\n", label);
			}
			if(dataNode->two->two->two->one->tk->ID == DOTTK){
				strcpy(label2, plusLabel());
				strcpy(label3, plusLabel());
				fprintf(out, "\tBRZPOS %s\n", label2);
				fprintf(out, "\tBR %s\n", label3);
				fprintf(out, "%s:\tNOOP/n", label3);
				fprintf(out, "\tADD 2\n");
				fprintf(out, "\tBRPOS %s\n", label2);
				fprintf(out, "\tBR %s\n", label3);
				fprintf(out, "%s:\tNOOP/n", label2);
				fprintf(out, "\tSUB 2\n");
				fprintf(out, "\tBRPOS %s\n", label2);
				fprintf(out, "\tBRNEG %s\n", label);
			}
			statSem(dataNode->four, IDCount, out);
			fprintf(out, "%s:\tNOOP\n", label);
			break;
	case PICKt:	statSem(dataNode->two, IDCount, out);
			strcpy(label, plusLabel());
			strcpy(label2, plusLabel());
			fprintf(out, "\tBRZERO %s\n", label);
			statSem(dataNode->three->one, IDCount, out);
			fprintf(out, "\tBR %s\n", label2);
			fprintf(out, "%s:\tNOOP\n", label);
			statSem(dataNode->three->three, IDCount, out);
			fprintf(out, "%s:\tNOOP\n", label2);
			break;
	case LOOP1t:	strcpy(label4, plusLabel());
			fprintf(out, "%s:\tNOOP\n", label4);
			statSem(dataNode->two->two->three, IDCount, out);
			strcpy(name, plusVar());
			fprintf(out, "\tSTORE %s\n", name);		
			statSem(dataNode->two->two->one, IDCount, out);
			fprintf(out, "\tSUB %s\n", name);
			strcpy(label, plusLabel());
			if(dataNode->two->two->two->one->tk->ID == ISEQUALTK){
				fprintf(out, "\tBRNEG %s\n", label);
				fprintf(out, "\tBRPOS %s\n", label);
			}
			if(dataNode->two->two->two->one->tk->ID == NOTEQUALTK){
				fprintf(out, "\tBRZERO %s\n", label);
			}
			if(dataNode->two->two->two->one->tk->ID == GREATTK){
				fprintf(out, "\tBRZNEG %s\n", label);
			}
			if(dataNode->two->two->two->one->tk->ID == LESSTK){
				fprintf(out, "\tBRZPOS %s\n", label);
			}
			if(dataNode->two->two->two->one->tk->ID == DOTTK){
				strcpy(label2, plusLabel());
				strcpy(label3, plusLabel());
				fprintf(out, "\tBRZPOS %s\n", label2);
				fprintf(out, "\tBR %s\n", label3);
				fprintf(out, "%s:\tNOOP/n", label3);
				fprintf(out, "\tADD 2\n");
				fprintf(out, "\tBRPOS %s\n", label2);
				fprintf(out, "\tBR %s\n", label3);
				fprintf(out, "%s:\tNOOP/n", label2);
				fprintf(out, "\tSUB 2\n");
				fprintf(out, "\tBRPOS %s\n", label2);
				fprintf(out, "\tBRNEG %s\n", label);
			}
			statSem(dataNode->three, IDCount, out);
			fprintf(out, "\tBR %s\n", label4);
			fprintf(out, "%s:\tNOOP\n", label);
			break;
	case LOOP2t:	strcpy(label4, plusLabel());
			fprintf(out, "%s:\tNOOP\n", label4);
			statSem(dataNode->two, IDCount, out);
			statSem(dataNode->four->two->three, IDCount, out);
			strcpy(name, plusVar());
			fprintf(out, "\tSTORE %s\n", name);		
			statSem(dataNode->four->two->one, IDCount, out);
			fprintf(out, "\tSUB %s\n", name);
			strcpy(label, plusLabel());
			if(dataNode->four->two->two->one->tk->ID == ISEQUALTK){
				fprintf(out, "\tBRNEG %s\n", label);
				fprintf(out, "\tBRPOS %s\n", label);
			}
			if(dataNode->four->two->two->one->tk->ID == NOTEQUALTK){
				fprintf(out, "\tBRZERO %s\n", label);
			}
			if(dataNode->four->two->two->one->tk->ID == GREATTK){
				fprintf(out, "\tBRZNEG %s\n", label);
			}
			if(dataNode->four->two->two->one->tk->ID == LESSTK){
				fprintf(out, "\tBRZPOS %s\n", label);
			}
			if(dataNode->four->two->two->one->tk->ID == DOTTK){
				strcpy(label2, plusLabel());
				strcpy(label3, plusLabel());
				fprintf(out, "\tBRZPOS %s\n", label2);
				fprintf(out, "\tBR %s\n", label3);
				fprintf(out, "%s:\tNOOP/n", label3);
				fprintf(out, "\tADD 2\n");
				fprintf(out, "\tBRPOS %s\n", label2);
				fprintf(out, "\tBR %s\n", label3);
				fprintf(out, "%s:\tNOOP/n", label2);
				fprintf(out, "\tSUB 2\n");
				fprintf(out, "\tBRPOS %s\n", label2);
				fprintf(out, "\tBRNEG %s\n", label);
			}
			fprintf(out, "%s:\tNOOP\n", label);
			break;
	default:
		preStat(dataNode, IDCount, out);
		break;
	}
	free(row);
	free(col);
	
}
void generator(node* root, FILE* outFile){
	funcNode = NULL;
	if(root == NULL || strcmp(root->tTitle.nonterm, gennonterms[1])){
		fprintf(stderr, "ERROR: GENERATOR: generator.c: parse tree"
			"can't be read.");
		exit(-1);
	}
	statSem(root, NULL, outFile);

	fprintf(outFile, "STOP\n");
	int i = 0;
	for(; i < varsCount; i++) {
		fprintf(outFile, "T%d 0\n", i);
	}

}
#endif
