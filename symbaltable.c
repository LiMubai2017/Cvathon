#include "string.h"
#include "Node.h"
#include <stdlib.h>

extern enum BlockType blockTypes[10];
extern int getIntLen(int );
extern int offset[5];
extern int exp2;
extern int exp3;
extern void log(char* );

enum SymbalType{TYPE_INT,TYPE_FLOAT,TYPE_CHAR,TYPE_VOID};
enum FlagType{FLAG_F,FLAG_P,FLAG_V};
typedef struct Symbal{
	char name[30];
	char alias[10];
	int level;
	enum SymbalType type;
	char flag;
	int offset;
	PEXP function_param;
}*SymbalP;
int index=0;
SymbalP table[100];
int v_index=0;

char* getNewAlias()
{
	v_index++;
	int len = getIntLen(v_index);
	char *noVal = (char*)malloc(sizeof(char)*(1+len+1));
	noVal[0]='v';
	int temp = v_index;
	for(int i = len; i >= 1; i--) {
		noVal[i]=temp%10+'0';
		temp/=10;
	}
	noVal[len+1]='\0';
	return noVal;
}

void addReadAndWriteFunction()
{
	SymbalP readFunction = (SymbalP)malloc(sizeof(struct Symbal));
	SymbalP writeFunction = (SymbalP)malloc(sizeof(struct Symbal));
	strcpy(readFunction->name , "read");
	strcpy(readFunction->alias,"");
	readFunction->level = 0;
	readFunction->type = TYPE_INT;
	readFunction->flag = FLAG_F;
	readFunction->offset = 0;
	readFunction->function_param=NULL;

	strcpy(writeFunction->name , "write");
	strcpy(writeFunction->alias,"");
	writeFunction->level = 0;
	writeFunction->type = TYPE_VOID;
	writeFunction->flag = FLAG_F;
	writeFunction->offset = 0;
	PEXP param_node = (PEXP)malloc(sizeof(struct Exp));
	strcpy(param_node->function_param.param_name,"num");
	param_node->function_param.param_type=INT_PARAM;
	param_node->function_param.pExp=NULL;
	writeFunction->function_param=param_node;

	table[index++]=readFunction;
	table[index++]=writeFunction;
}

void push(SymbalP newOne)
{
	if(index == 0) {
		addReadAndWriteFunction();
	}

	table[index++]=newOne;
	if(exp2) {
		displayTable();
		getchar();
	}
}

void tableOut(int level)
{
	while(index > 0 && table[index-1]->level==level) {
		index--;
		free(table[index]);
	}
	if(exp2) {
		displayTable();
		getchar();
	}
}

PEXP getParamNode(char *function)
{
	int current = index;
	while(current>=0) {
		current--;
		if(table[current]->flag==FLAG_F&&strcmp(table[current]->name,function)==0) {
			return table[current]->function_param;
		}
	}
	printf("unexpected error\n");
	exit(1);
}

char* getVariableAlias(char id[])
{
	int current = index;
	while(current>=0) {
		current--;
		if(strcmp(table[current]->name,id)==0) {
			return table[current]->alias;
		}
	}
	printf("unexpected error\n");
	exit(1);
}

enum SymbalType _getVariableType(char target[])
{
	int current = index;
	while(current>=0) {
		current--;
		if(strcmp(table[current]->name,target )==0) {
			return table[current]->type;
		}
	}
	printf("unexpected error\n");
	exit(1);
}

//1-success ; 0-failure
int insertIntoTable(PEXP T,int level,int line)
{
	char *name;
	int result;
	switch(T->kind) {
		case FUNCTION_DECLARE_NODE:
			name=T->function.function_name;
			if(_checkDefine(name,level,line)) {
				SymbalP current=(SymbalP)malloc(sizeof(struct Symbal));
				strcpy(current->name,T->function.function_name);
				strcpy(current->alias,"");
				current->level=level;
				switch (T->function.returnType){
					case INT_FUNCTION:
						current->type=TYPE_INT;
						break;
					case CHAR_FUNCTION:
						current->type=TYPE_CHAR;
						break;
					case FLOAT_FUNCTION:
						current->type=TYPE_FLOAT;
						break;
					case VOID_FUNCTION:
						current->type=TYPE_VOID;
						break;
				}
				current->flag=FLAG_F;
				current->offset=offset[level];
				current->function_param=T->function_param.pExp;
				push(current);
				if(T->function.returnType!=VOID_FUNCTION && T->function.pExp!=NULL) {
					insertIntoTable(T->function.pExp,level+1,line);
				}
				return 1;
			} else {
				if(exp2 || exp3) {
					printf("锟斤拷锟斤拷  锟叫号ｏ拷%d  锟斤拷锟斤拷锟斤拷锟截革拷锟斤拷锟斤拷%s\n",line,name);
				}
				return 0;
			}
			break;
		case INT_NODE:
			return insertSub(T->ptr.pExp1,TYPE_INT,level,line);
			break;
		case CHAR_NODE:
			return insertSub(T->ptr.pExp1,TYPE_CHAR,level,line);
			break;
		case FLOAT_NODE:
			return insertSub(T->ptr.pExp1,TYPE_FLOAT,level,line);
			break;
	}
}

int insertSub(PEXP T,enum SymbalType type,int level,int line)
{
	int offsetDelta=0;
	char *name;
	SymbalP current;
	switch(type) {
		case TYPE_INT:
			offsetDelta=4;
			break;
		case TYPE_FLOAT:
			offsetDelta=4;
			break;
		case TYPE_CHAR:
			offsetDelta=1;
			break;
	}
	
	switch(T->kind) {
		case DECLARE_SUB_NODE:
			current=(SymbalP)malloc(sizeof(struct Symbal));
			int result = insertSub(T->ptr.pExp1,type,level,line);
			if(!result) return 0;
			if(T->ptr.pExp2!=NULL) {
				result=insertSub(T->ptr.pExp2,type,level,line);
				if(!result) return 0;
			}
			break;
		case ID_NODE:
			name=T->id.type_id;
			if(!_checkDefine(name,level,line)) {
				if(exp2 || exp3) {
					printf("锟斤拷锟斤拷  锟叫号ｏ拷%d  锟斤拷锟斤拷锟斤拷锟截革拷锟斤拷锟斤拷%s\n",line,name);
					getchar();
				}
				return 0;
			}
			current=(SymbalP)malloc(sizeof(struct Symbal));
			strcpy(current->name,T->function.function_name);
			strcpy(current->alias,getNewAlias());
			current->level=level;
			current->type=type;
			current->flag=FLAG_V;
			current->offset=offset[level];
			offset[level]+=offsetDelta;
			push(current);
			break;
		case ID_ARRAY_NODE:
			break;
	}
	return 1;
}

//1-no exist(or error); 0-exist
int checkTable(PEXP T,int level,int line)
{
	char *name;
	int result;
	int i;
	enum SymbalType type;
	PEXP left;
	switch(T->kind) {
		case INTEGER_NODE:
			return 0;
		case FUNCTION_FIRE_NODE:
			name=T->fire.fire_id;
			result = _checkExist(name,level,line,FLAG_F);
			if(result) {
				if(exp2 || exp3) {
					printf("行号：%d  函数未定义%s\n",line,name);
					getchar();
				}
				return 1;
			} 
			PEXP paramNode = getParamNode(name);
			if(paramNode != NULL) {
				result = _checkParamType(paramNode,T->fire.valueList);
			} else {
				result = 0;
			}
			if(result) {
				if(exp2 || exp3) {
					printf("行号：%d  函数调用参数类型不匹配\n",line);
					getchar();
				}
			}
			log("check function fire finished\n");
			return result;
		case CONTINUE_NODE:
			result=1;
			for(i=level-1;i>=0;i--) {
				if(blockTypes[i] == FOR_BLOCK || blockTypes[i] == WHILE_BLOCK) {
					result=0;
				}
			}
			if(result) {
				if(exp2 || exp3) {
					printf("行号：%d  continue不在循环内\n",line);
					getchar();
				}
			}	else {
			}
			return result;
		case BREAK_NODE:
			result=1;
			for(i=level-1;i>=0;i--) {
				if(blockTypes[i] == FOR_BLOCK || blockTypes[i] == WHILE_BLOCK) {
					result=0;
				}
			}
			if(result) {
				if(exp2 || exp3) {
					printf("行号：%d  break不在循环内\n",line);
					getchar();
				}
			}	
			return result;
			break;
		case ELSE_NODE:
			if(blockTypes[level] == IF_BLOCK) {
				return 0;
			} else {
				if(exp2 || exp3) {
					printf("else不跟在if之后  行号%d \n",line);
					getchar();
				}
				return 1;
			}
			break;
		case RETURN_NODE:
			if(level==0) {
				if(exp2 || exp3) {
					printf("在函数外使用return 行号：%d \n",line);
					getchar();
				}
				return 1;
			}
			return 0;
			break;
		case ID_NODE:
			name=T->id.type_id;
			result = _checkExist(name,level,line,FLAG_V);
			if(result) {
				if(exp2 || exp3) {
					printf("变量未定义  行号 %d  变量名：%s\n",line,name);
					getchar();
				}
			} 
			return result;
			break;
		case ASSIGN_NODE:
			result = checkTable(T->ptr.pExp1,level,line);
			if(!result) {
				result = checkTable(T->ptr.pExp2,level,line);
			}
			if(result) {
				return 1;
			}
			type = _getVariableType((T->ptr.pExp1)->id.type_id);
			result = _checkExpType(type,T->ptr.pExp2);
			if(result) {
				if(exp2 || exp3) {
					printf("类型不匹配，行号：%d\n",line);
					getchar();
				}
			}
			return result;
		case PLUS_NODE:
		case STAR_NODE:
		case MINUS_NODE:
		case DIV_NODE:
			result = checkTable(T->ptr.pExp1,level,line);
			if(!result) {
				result = checkTable(T->ptr.pExp2,level,line);
			}
			return result;
			break;
		case UMINUS_NODE:
		case INC_PREFIX_NODE:
		case DEC_PREFIX_NODE:
		case INC_SUFFIX_NODE:
		case DEC_SUFFIX_NODE:
			checkTable(T->ptr.pExp1,level,line);
			break;
	}
	return 1;
}

//1-error ; 0-no error
int _checkParamType(PEXP paramNode, PEXP valueNode)
{
	PEXP p1 = paramNode;
	PEXP p2 = valueNode;
	
	return 0;
}

//1-error ; 0-no error
int _checkExpType(enum SymbalType type,PEXP T)
{
	switch(T->kind) {
		case VALUE_LIST_NODE:
			return _checkExpType(type,T->ptr.pExp1);
			break;
		case INTEGER_NODE:
			if(type == TYPE_INT) {
				return 0;
			} else {
				return 1;
			}
			break;
		case PLUS_NODE:
		case STAR_NODE:
		case MINUS_NODE:
		case DIV_NODE:
			if(_checkExpType(type,T->ptr.pExp1)||_checkExpType(type,T->ptr.pExp2)) {
				return 1;
			} else {
				return 0;
			}
			break;
		case ID_NODE:
			if(_getVariableType(T->id.type_id) == type) {
				return 0;
			}	else {
				return 1;
			}
			break;
		case FUNCTION_FIRE_NODE:
			if(_getVariableType(T->id.type_id) == type) {
				return 0;
			}	else {
				return 1;
			}
			break;
		case UMINUS_NODE:
		case INC_PREFIX_NODE:
		case INC_SUFFIX_NODE:
		case DEC_PREFIX_NODE:
		case DEC_SUFFIX_NODE:
			return _checkExpType(type,T->ptr.pExp1);
			break;
	}
}

//1-no exist ; 0-exist
int _checkDefine(char target[],int level,int line)
{
	int current = index;
	while(current>0 && table[current-1]->level==level) {
		current--;
		if(strcmp(table[current]->name,target)==0) {
			return 0;
		}
	}
	return 1;
}

//1-no exist ; 0-exist
int _checkExist(char target[],int level,int line,enum FlagType flag)
{
	int current = index;
	while(current>0 && table[current-1]->level<=level) {
		current--;
		if(strcmp(table[current]->name,target)==0 && table[current]->flag==flag) {
			return 0;
		}
	}
	return 1;
}

void displayTable()
{
	printf("变量名\t别名\t层号\t类型\t标记\t偏移量\n");
	for(int i = 0; i < index; i++) {
		printf("%s\t%s\t%d\t",table[i]->name,table[i]->alias,table[i]->level);
		switch(table[i]->type) {
			case TYPE_INT:
				printf("int\t");
				break;
			case TYPE_FLOAT:
				printf("float\t");
				break;
			case TYPE_CHAR:
				printf("char\t");
				break;
			case TYPE_VOID:
				printf("void\t");
				break;
		}
		switch(table[i]->flag) {
			case FLAG_F:
				printf("F\t");
				break;
			case FLAG_V:
				printf("V\t");
				break;
			case FLAG_P:
				printf("P\t");
				break;
		}
		printf("%d\n",table[i]->offset);
	}
}
