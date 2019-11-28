#include "stdio.h"
#include "Node.h"
#include "string.h"
char WORD_KIND[26][10] = {"LP","RP","BLP","BRP","PLUS","MINUS","STAR","DIV","INTEGER","ID",
				 "ASSIGNOP","INT","FLOAT","CHAR","GREATER","LESS","EQUAL","GREATER_EQUAL",
				 "LESS_EQUAL","IF","ELSE","WHILE","CONTINUE","BREAK","INC","DEC","INT[]","FLOAT[]","CHAR[]"};
int blanks=5;
enum msg{NEST_CODE_BLOCK,ELSE_LINE,ARRAY};
void displayMessage(int index, int indent)
{
	switch(index+NEST_CODE_BLOCK) {
		case NEST_CODE_BLOCK:
			printf("%*c嵌套代码块:\n",indent,' ');
			break;
		case ELSE_LINE:
			printf("%*cELSE语句:\n",indent,' ');
			break;
		case ARRAY:
			printf("\n\n%*c二元组序列:\n",indent,' ');
			break;
	}
}

void display(PEXP T,int indent)
{
	
	switch (T->kind) {
		case IF_NODE:
			printf("%*cIF语句:\n",indent,' ');
			printf("%*c判断条件:\n",indent+blanks,' ');
			display(T->ptr.pExp1,indent+blanks*2);
			break;
		case WHILE_NODE:
			printf("%*cWHILE语句:\n",indent,' ');
			printf("%*c判断条件:\n",indent+blanks,' ');
			display(T->ptr.pExp1,indent+blanks*2);
			break;
		case GREATER_NODE:
			printf("%*cGREATER:\n",indent,' ');
			display(T->ptr.pExp1,indent+blanks);
			display(T->ptr.pExp2,indent+blanks);
			break;
		case LESS_NODE:
			printf("%*cLESS:\n",indent,' ');
			display(T->ptr.pExp1,indent+blanks);
			display(T->ptr.pExp2,indent+blanks);
			break;
		case EQUAL_NODE:
			printf("%*cEQUAL:\n",indent,' ');
			display(T->ptr.pExp1,indent+blanks);
			display(T->ptr.pExp2,indent+blanks);
			break;
		case GREATER_EQUAL_NODE:
			printf("%*cGREATER_EQUAL:\n",indent,' ');
			display(T->ptr.pExp1,indent+blanks);
			display(T->ptr.pExp2,indent+blanks);
			break;
		case LESS_EQUAL_NODE:
			printf("%*cLESS_EQUAL:\n",indent,' ');
			display(T->ptr.pExp1,indent+blanks);
			display(T->ptr.pExp2,indent+blanks);
			break;
		case CHAR_NODE:
			if (indent == 0) {
				printf("%*c外部变量定义:\n",indent,' ');
			} else {
				printf("%*c内部变量定义:\n",indent,' ');
			}
			printf("%*c类型： %s\n",indent+blanks,' ',"CHAR");
			display(T->ptr.pExp1,indent+blanks);
			break;
		case FLOAT_NODE:
			if (indent == 0) {
				printf("%*c外部变量定义:\n",indent,' ');
			} else {
				printf("%*c内部变量定义:\n",indent,' ');
			}
			printf("%*c类型： %s\n",indent+blanks,' ',"FLOAT");
			display(T->ptr.pExp1,indent+blanks);
			break;
		case INT_NODE:
			if (indent == 0) {
				printf("%*c外部变量定义:\n",indent,' ');
			} else {
				printf("%*c内部变量定义:\n",indent,' ');
			}
			printf("%*c类型： %s\n",indent+blanks,' ',"INT");
			display(T->ptr.pExp1,indent+blanks);
			break;
		case CHARS_NODE:
			if (indent == 0) {
				printf("%*c外部变量定义:\n",indent,' ');
			} else {
				printf("%*c内部变量定义:\n",indent,' ');
			}
			printf("%*c类型： %s\n",indent+blanks,' ',"CHAR数组");
			display(T->ptr.pExp1,indent+blanks);
			break;
		case FLOATS_NODE:
			if (indent == 0) {
				printf("%*c外部变量定义:\n",indent,' ');
			} else {
				printf("%*c内部变量定义:\n",indent,' ');
			}
			printf("%*c类型： %s\n",indent+blanks,' ',"FLOAT数组");
			display(T->ptr.pExp1,indent+blanks);
			break;
		case INTS_NODE:
			if (indent == 0) {
				printf("%*c外部变量定义:\n",indent,' ');
			} else {
				printf("%*c内部变量定义:\n",indent,' ');
			}
			printf("%*c类型： %s\n",indent+blanks,' ',"INT数组");
			display(T->ptr.pExp1,indent+blanks);
			break;
		case DECLARE_SUB_NODE:
			display(T->ptr.pExp1,indent);
			if(T->ptr.pExp2 != NULL)
				display(T->ptr.pExp2,indent);
			break;
		case ID_NODE:
			printf("%*cID： %s\n",indent,' ',T->type_id);
			break;
		case INTEGER_NODE:
			printf("%*cINT： %d\n",indent,' ',T->type_integer);
			break;
		case UMINUS_NODE:
			printf("%*c%s\n",indent,' ',"UMINUS");
			display(T->ptr.pExp1,indent+blanks);
			break;
		case PLUS_NODE:
		case MINUS_NODE:
		case STAR_NODE:
		case DIV_NODE:
			printf("%*c%s\n",indent,' ',T->kind==PLUS_NODE?"PLUS":(T->kind==MINUS_NODE?"MINUS":(T->kind==STAR_NODE?"STAR":"DIV")));
			display(T->ptr.pExp1,indent+blanks);
			display(T->ptr.pExp2,indent+blanks);
			break;
		case ASSIGN_NODE:
			printf("%*c%s\n",indent,' ',"ASSIGNOP");
			display(T->ptr.pExp1,indent+blanks);
			display(T->ptr.pExp2,indent+blanks);
			break;
		case INC_PREFIX_NODE:
			printf("%*c%s\n",indent,' ',"INC_PREFIX");
			display(T->ptr.pExp1,indent+blanks);
			break;
		case INC_SUFFIX_NODE:
			printf("%*c%s\n",indent,' ',"INC_SUFFIX");
			display(T->ptr.pExp1,indent+blanks);
			break;
		case DEC_PREFIX_NODE:
			printf("%*c%s\n",indent,' ',"DEC_PREFIX");
			display(T->ptr.pExp1,indent+blanks);
			break;
		case DEC_SUFFIX_NODE:
			printf("%*c%s\n",indent,' ',"DEC_SUFFIX");
			display(T->ptr.pExp1,indent+blanks);
			break;
		case CONTINUE_NODE:
			printf("%*c%s\n",indent,' ',"CONTINUE");
			break;
		case BREAK_NODE:
			printf("%*c%s\n",indent,' ',"BREAK");
			break;
	}

}

void displayWordsArray(ARRAYP array)
{	
	if(array != NULL) {
		int length = strlen(WORD_KIND[array->kind-LP_ARRAY]);
		printf("种类码：%s %*c 值：%s\n",WORD_KIND[array->kind-LP_ARRAY],20-length,' ',array->value);
		displayWordsArray(array->next);
	}
}

