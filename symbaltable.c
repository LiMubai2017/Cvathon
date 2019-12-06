#include "string.h"
#include "Node.h"
enum SymbalType{TYPE_INT,TYPE_FLOAT,TYPE_CHAR,TYPE_VOID};
enum FlagType{FLAG_F,FLAG_P,FLAG_V};
typedef struct Symbal{
	char name[30];
	char no[10];
	int level;
	enum SymbalType type;
	char flag;
	int offset;
}*SymbalP;
int index=0;
extern int offset[5];
SymbalP table[100];
char no[] = "V";

void push(SymbalP newOne)
{
	table[index++]=newOne;
	displayTable();
	getchar();
}

void tableOut(int level)
{
	while(table[index-1]->level==level) {
		index--;
		free(table[index]);
	}
	displayTable();
	getchar();
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
				strcpy(current->no,no);
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
				push(current);
				if(T->function.returnType!=VOID_FUNCTION && T->function.pExp!=NULL) {
					insertIntoTable(T->function.pExp,level+1,line);
				}
				return 1;
			} else {
				printf("错误  行号：%d  函数名重复定义%s\n",line,name);
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
				printf("错误  行号：%d  变量名重复定义%s\n",line,name);
				getchar();
				return 0;
			}
			current=(SymbalP)malloc(sizeof(struct Symbal));
			strcpy(current->name,T->function.function_name);
			strcpy(current->no,no);
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

//1-no exist ; 0-exist
int checkTable(PEXP T,int level,int line)
{
	char *name;
	int result;
	switch(T->kind) {
		case FUNCTION_FIRE_NODE:
			name=T->fire.fire_id;
			result = _checkExist(name,level,line,FLAG_F);
			if(result) {
				printf("错误  行号：%d  函数未定义%s\n",line,name);
				getchar();
			} 
			return result;
			break;
		case CONTINUE_NODE:
			break;
		case BREAK_NODE:
			break;
		case ELSE_NODE:
			break;
		case ID_NODE:
			name=T->id.type_id;
			result = _checkExist(name,level,line,FLAG_V);
			if(result) {
				printf("错误  行号：%d  变量未定义%s\n",line,name);
				getchar();
			} 
			return result;
			break;
		case ASSIGN_NODE:
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

/*
char name[30];
	char no[10];
	int level;
	enum SymbalType type;
	char flag;
	int offset;
*/

void displayTable()
{
	printf("变量名\t别名\t层号\t类型\t标记\t偏移量\n");
	for(int i = 0; i < index; i++) {
		printf("%s\t%s\t%d\t",table[i]->name,table[i]->no,table[i]->level);
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
