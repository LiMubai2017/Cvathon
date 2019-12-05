#include "stdio.h"
#include "Node.h"
#include "string.h"
char WORD_KIND[29][10] = {"LP","RP","BLP","BRP","PLUS","MINUS","STAR","DIV","INTEGER","ID",
				 "ASSIGNOP","INT","FLOAT","CHAR","GREATER","LESS","EQUAL","GREATER_EQUAL",
				 "LESS_EQUAL","IF","ELSE","WHILE","CONTINUE","BREAK","INC","DEC","[","]","for"};
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
		case FOR_NODE:
			printf("%*cFOR语句:\n",indent,' ');
			printf("%*c初始语句:\n",indent+blanks,' ');
			if(T->for_exp.p1!=NULL) display(T->for_exp.p1,indent+blanks*2);
			printf("%*c循环判断条件:\n",indent+blanks,' ');
			if(T->for_exp.p2!=NULL) display(T->for_exp.p2,indent+blanks*2);
			printf("%*c后执行语句:\n",indent+blanks,' ');
			if(T->for_exp.p3!=NULL) display(T->for_exp.p3,indent+blanks*2);
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
		case DECLARE_SUB_NODE:
			display(T->ptr.pExp1,indent);
			if(T->ptr.pExp2 != NULL)
				display(T->ptr.pExp2,indent);
			break;
		case ID_NODE:
			printf("%*cID： %s\n",indent,' ',T->id.type_id);
			break;
		case ID_ARRAY_NODE:
			printf("%*cID_ARRAY： %s 维度：%d ",indent,' ',T->id.type_id,T->id.dimension);
			switch(T->id.dimension) {
				case 1:
					printf("下标：%d\n",T->id.index1);
					break;
				case 2:
					printf("下标：%d %d\n",T->id.index1,T->id.index2);
					break;
				case 3:
					printf("下标：%d %d %d\n",T->id.index1,T->id.index2,T->id.index3);
					break;
			}
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
		case FUNCTION_DECLARE_NODE:
			printf("%*c函数声明语句:\n",indent,' ');
			printf("%*c返回类型:\n",indent+blanks,' ');
			switch(T->function.returnType){
				case INT_FUNCTION:
					printf("%*c int:\n",indent+blanks*2,' ');
					break;
				case CHAR_FUNCTION:
					printf("%*c char:\n",indent+blanks*2,' ');
					break;
				case FLOAT_FUNCTION:
					printf("%*c float:\n",indent+blanks*2,' ');
					break;
				case VOID_FUNCTION:
					printf("%*c void:\n",indent+blanks*2,' ');
					break;
				
			}
			if(T->function.pExp != NULL) {
				printf("%*c参数声明:\n",indent+blanks,' ');
				display(T->function.pExp,indent+blanks*2);
			}
			break;
		case FUNCTION_FIRE_NODE:
			printf("%*c函数调用语句:\n",indent,' ');
			printf("%*c函数名:\n",indent+blanks,' ');
			printf("%*c %s:\n",indent+blanks*2,' ',T->fire.fire_id);
			printf("%*c参数:\n",indent+blanks,' ');
			if(T->fire.valueList == NULL) {
				printf("%*c VOID:\n",indent+blanks*2,' ');
			} else {
				display(T->fire.valueList,indent+blanks*2);
			}
			break;
		case VALUE_LIST_NODE:
			display(T->ptr.pExp1,indent+blanks);
			if(T->ptr.pExp2!=NULL)
				display(T->ptr.pExp2,indent+blanks);
			break;
		case CONDITION_LIST_NODE:
			if(T->condition_list.pExp2!=NULL) {
				switch(T->condition_list.type) {
					case LINK_AND:
						printf("%*cAND:\n",indent,' ');
						break;
					case LINK_OR:
						printf("%*cOR:\n",indent,' ');
						break;
				}
				display(T->condition_list.pExp1,indent+blanks);
				display(T->condition_list.pExp2,indent+blanks);
			}	else {
				display(T->condition_list.pExp1,indent);
			}
			break;
		case RETURN_NODE:
			printf("%*c返回语句:\n",indent,' ');
			printf("%*c返回值:\n",indent+blanks,' ');
			switch(T->return_exp.returnType) {
				case RETURN_VOID:
					printf("%*cVOID:\n",indent+blanks*2,' ');
					break;
				case RETURN_EXP:
					display(T->return_exp.pExp,indent+blanks*2);
					break;
			}
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

