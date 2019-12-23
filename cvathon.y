%{
#include "stdio.h"
#include "Node.h"

#include "string.h"
extern char *yytext;
extern FILE *yyin;
extern int blanks;
void display(struct Exp *,int);
void displayMessage(int , int);
int nestCodeBlock = 0;
int exp1 = 0;
int showWordArray=0;
int exp2 = 0;
int exp3 = 0;
int exp4 = 0;
int offset[5]={0,0,0,0,0};
enum BlockType blockTypes[10]={OTHER_BLOCK,OTHER_BLOCK,OTHER_BLOCK,OTHER_BLOCK,OTHER_BLOCK,OTHER_BLOCK,OTHER_BLOCK,OTHER_BLOCK,OTHER_BLOCK,OTHER_BLOCK};
extern int yylineno;
%}

%union {
	int type_integer;
	char type_id[32];
	struct Exp *pExp;
};

%type  <pExp> line exp exp_unary declare sub_declare id id_array array if_line condition while_line control assign for_line function function_fire return_line value_list condition_list function_param

%token <type_integer> INTEGER
%token <type_id> ID

%token LP RP BLP BRP MLP MRP 
%token PLUS MINUS STAR DIV ASSIGNOP
%token INT FLOAT CHAR
%token GREATER LESS EQUAL GREATER_EQUAL LESS_EQUAL
%token IF ELSE WHILE CONTINUE BREAK FOR RETURN
%token INC DEC
%token AND OR NOT

%nonassoc VARIABLE ID
%right INT CHAR FLOAT VOID DECLARE_PRI FUNCTION_PRI
%right IF ELSE WHILE CONTINUE BREAK
%left EQUAL GREATER_EQUAL LESS_EQUAL GREATER LESS
%left ASSIGNOP
%left PLUS MINUS
%left STAR DIV
%nonassoc UNARY


%%
input:
	 | input line
	 ;
line : '\n'    { ;}
	 | function '\n' %prec FUNCTION_PRI {if(exp1) {display($1,nestCodeBlock*blanks);}
					if(exp2 || exp3 || exp4) {if(!insertIntoTable($1,nestCodeBlock,yylineno)) return;
					blockTypes[nestCodeBlock]=FUNCTION_BLOCK;}
					if(exp3 || exp4) {translateStmt($1);}}
	 | function_fire '\n' {if(exp1) {display($1,nestCodeBlock*blanks);}
						if(exp2 || exp3 || exp4) {if(checkTable($1,nestCodeBlock,yylineno)) return;
						blockTypes[nestCodeBlock]=OTHER_BLOCK;}
						if(exp3 || exp4) {translateStmt($1);}}
	 | declare '\n' %prec DECLARE_PRI {if(exp1) {display($1,nestCodeBlock*blanks);}
					if(exp2 || exp3 || exp4) {if(!insertIntoTable($1,nestCodeBlock,yylineno)) return;
					blockTypes[nestCodeBlock]=OTHER_BLOCK;}
					if(exp3 || exp4) {translateStmt($1);}}
	 | assign '\n' {if(exp1) {display($1,nestCodeBlock*blanks);}
					if(exp2 || exp3 || exp4) {if(checkTable($1,nestCodeBlock,yylineno)) return;
					blockTypes[nestCodeBlock]=OTHER_BLOCK;}
					if(exp3 || exp4) {translateStmt($1);}}
	 | exp_unary {if(exp1) {display($1,nestCodeBlock*blanks);}
				if(exp2 || exp3 || exp4) {if(checkTable($1,nestCodeBlock,yylineno)) return;
				blockTypes[nestCodeBlock]=OTHER_BLOCK;}
				if(exp3 || exp4) {translateStmt($1);}}
	 | BLP '\n' {if(exp1) {displayMessage(0,(nestCodeBlock+1)*blanks);}nestCodeBlock+=1;}
	 | BRP '\n' {nestCodeBlock-=1;
				if(exp3 || exp4) parsePreCode();
				if(exp2 || exp3 || exp4) {tableOut(nestCodeBlock+1);}
				if(exp2 || exp3 || exp4){offset[nestCodeBlock]+=offset[nestCodeBlock+1];offset[nestCodeBlock+1]=0;}}
	 | if_line '\n'	{if(exp1) {display($1,nestCodeBlock*blanks);}
					if(exp2 || exp3 || exp4) blockTypes[nestCodeBlock]=IF_BLOCK;
					if(exp3 || exp4) translateStmt($1);}
	 | while_line '\n'{if(exp1) {display($1,nestCodeBlock*blanks);}
					if(exp2 || exp3 || exp4) blockTypes[nestCodeBlock]=WHILE_BLOCK;
					if(exp3 || exp4) translateStmt($1);}
	 | for_line '\n'{if(exp1) {display($1,nestCodeBlock*blanks);}
					if(exp2 || exp3 || exp4) blockTypes[nestCodeBlock]=FOR_BLOCK;
					if(exp3 || exp4) translateStmt($1);}
	 | control '\n' {if(exp1) {display($1,nestCodeBlock*blanks);}
					if(exp2 || exp3 || exp4) {if(checkTable($1,nestCodeBlock,yylineno)) return;
					blockTypes[nestCodeBlock]=OTHER_BLOCK;}
					if(exp3 || exp4) translateStmt($1);}
	 | return_line '\n' {if(exp1) {display($1,nestCodeBlock*blanks);}
					if(exp2 || exp3 || exp4) {if(checkTable($1,nestCodeBlock,yylineno)) return;
					blockTypes[nestCodeBlock]=OTHER_BLOCK;}
					if(exp3 || exp4) {translateStmt($1);}}
	 | error '\n' {printf("line error!\n");}
	 ;

function : VOID ID LP RP {$$=(PEXP)malloc(sizeof(struct Exp)); $$->kind=FUNCTION_DECLARE_NODE;$$->function.returnType=VOID_FUNCTION;$$->function.pExp=NULL;strcpy($$->function.function_name,$2);}
		 | INT ID LP  RP {$$=(PEXP)malloc(sizeof(struct Exp)); $$->kind=FUNCTION_DECLARE_NODE;$$->function.returnType=INT_FUNCTION;$$->function.pExp=NULL;strcpy($$->function.function_name,$2);}
		 | CHAR ID LP  RP {$$=(PEXP)malloc(sizeof(struct Exp)); $$->kind=FUNCTION_DECLARE_NODE;$$->function.returnType=CHAR_FUNCTION;$$->function.pExp=NULL;strcpy($$->function.function_name,$2);}
		 | FLOAT ID LP RP {$$=(PEXP)malloc(sizeof(struct Exp)); $$->kind=FUNCTION_DECLARE_NODE;$$->function.returnType=FLOAT_FUNCTION;$$->function.pExp=NULL;strcpy($$->function.function_name,$2);}
		 | INT ID LP function_param RP {$$=(PEXP)malloc(sizeof(struct Exp)); $$->kind=FUNCTION_DECLARE_NODE;$$->function.returnType=INT_FUNCTION;$$->function.pExp=$4;strcpy($$->function.function_name,$2);}
		 | CHAR ID LP function_param RP {$$=(PEXP)malloc(sizeof(struct Exp)); $$->kind=FUNCTION_DECLARE_NODE;$$->function.returnType=CHAR_FUNCTION;$$->function.pExp=$4;strcpy($$->function.function_name,$2);}
		 | FLOAT ID LP function_param RP {$$=(PEXP)malloc(sizeof(struct Exp)); $$->kind=FUNCTION_DECLARE_NODE;$$->function.returnType=FLOAT_FUNCTION;$$->function.pExp=$4;strcpy($$->function.function_name,$2);}
function_param : INT ID {$$=(PEXP)malloc(sizeof(struct Exp)); $$->kind=FUNCTION_PARAM_NODE;$$->function_param.param_type=INT_PARAM;$$->function_param.pExp=NULL;strcpy($$->function_param.param_name, $2);}
		| CHAR ID {$$=(PEXP)malloc(sizeof(struct Exp)); $$->kind=FUNCTION_PARAM_NODE;$$->function_param.param_type=CHAR_PARAM;$$->function_param.pExp=NULL;strcpy($$->function_param.param_name, $2);}
		| FLOAT ID {$$=(PEXP)malloc(sizeof(struct Exp)); $$->kind=FUNCTION_PARAM_NODE;$$->function_param.param_type=FLOAT_PARAM;$$->function_param.pExp=NULL;strcpy($$->function_param.param_name, $2);}
		| INT ID ',' function_param {$$=(PEXP)malloc(sizeof(struct Exp)); $$->kind=FUNCTION_PARAM_NODE;$$->function_param.param_type=INT_PARAM;$$->function_param.pExp=$4;strcpy($$->function_param.param_name, $2);}
		| CHAR ID ',' function_param {$$=(PEXP)malloc(sizeof(struct Exp)); $$->kind=FUNCTION_PARAM_NODE;$$->function_param.param_type=CHAR_PARAM;$$->function_param.pExp=$4;strcpy($$->function_param.param_name, $2);}
		| FLOAT ID ',' function_param {$$=(PEXP)malloc(sizeof(struct Exp)); $$->kind=FUNCTION_PARAM_NODE;$$->function_param.param_type=FLOAT_PARAM;$$->function_param.pExp=$4;strcpy($$->function_param.param_name, $2);}

function_fire : ID LP RP {$$=(PEXP)malloc(sizeof(struct Exp)); $$->kind=FUNCTION_FIRE_NODE;strcpy($$->fire.fire_id,$1);$$->fire.valueList=NULL;}
			  | ID LP value_list RP {$$=(PEXP)malloc(sizeof(struct Exp)); $$->kind=FUNCTION_FIRE_NODE;strcpy($$->fire.fire_id,$1);$$->fire.valueList=$3;}
value_list : exp {$$=(PEXP)malloc(sizeof(struct Exp)); $$->kind=VALUE_LIST_NODE; $$->ptr.pExp1=$1;$$->ptr.pExp2=NULL;}
		   | exp ',' value_list {$$=(PEXP)malloc(sizeof(struct Exp)); $$->kind=VALUE_LIST_NODE; $$->ptr.pExp1=$1; $$->ptr.pExp2=$3;}
control : CONTINUE  {$$=(PEXP)malloc(sizeof(struct Exp)); $$->kind=CONTINUE_NODE;}
		| BREAK 	{$$=(PEXP)malloc(sizeof(struct Exp)); $$->kind=BREAK_NODE;}
		| ELSE 		{if(exp1) displayMessage(1,nestCodeBlock*blanks);
					$$=(PEXP)malloc(sizeof(struct Exp)); $$->kind=ELSE_NODE;}
return_line : RETURN {$$=(PEXP)malloc(sizeof(struct Exp)); $$->kind=RETURN_NODE;$$->return_exp.returnType=RETURN_VOID;$$->return_exp.pExp=NULL}
			| RETURN exp {$$=(PEXP)malloc(sizeof(struct Exp)); $$->kind=RETURN_NODE;$$->return_exp.returnType=RETURN_EXP;$$->return_exp.pExp=$2;}
while_line : WHILE LP condition_list RP {$$=(PEXP)malloc(sizeof(struct Exp)); $$->kind=WHILE_NODE;$$->ptr.pExp1=$3;}
for_line: FOR LP assign ';' condition_list ';' exp_unary RP {$$=(PEXP)malloc(sizeof(struct Exp)); $$->kind=FOR_NODE;$$->for_exp.p1=$3;$$->for_exp.p2=$5;$$->for_exp.p3=$7;}
		| FOR LP ';' condition_list ';' exp_unary RP {$$=(PEXP)malloc(sizeof(struct Exp)); $$->kind=FOR_NODE;$$->for_exp.p1=NULL;$$->for_exp.p2=$4;$$->for_exp.p3=$6;}
		| FOR LP assign ';' condition_list ';'  RP {$$=(PEXP)malloc(sizeof(struct Exp)); $$->kind=FOR_NODE;$$->for_exp.p1=$3;$$->for_exp.p2=$5;$$->for_exp.p3=NULL;}
		| FOR LP  ';' condition_list ';'  RP {$$=(PEXP)malloc(sizeof(struct Exp)); $$->kind=FOR_NODE;$$->for_exp.p1=NULL;$$->for_exp.p2=$4;$$->for_exp.p3=NULL;}
if_line : IF LP condition_list RP {$$=(PEXP)malloc(sizeof(struct Exp)); $$->kind=IF_NODE;$$->ptr.pExp1=$3;}
condition_list: condition {$$=$1;}
			  | NOT condition {$$=(PEXP)malloc(sizeof(struct Exp)); $$->kind=CONDITION_LIST_NODE; $$->condition_list.pExp1=$2;$$->condition_list.pExp2=NULL;$$->condition_list.type=LINK_NOT;}
			  | condition_list AND condition_list {$$=(PEXP)malloc(sizeof(struct Exp)); $$->kind=CONDITION_LIST_NODE; $$->condition_list.pExp1=$1;$$->condition_list.pExp2=$3;$$->condition_list.type=LINK_AND;}
			  | condition_list OR condition_list {$$=(PEXP)malloc(sizeof(struct Exp)); $$->kind=CONDITION_LIST_NODE; $$->condition_list.pExp1=$1;$$->condition_list.pExp2=$3;$$->condition_list.type=LINK_OR;}
condition: exp EQUAL exp {$$=(PEXP)malloc(sizeof(struct Exp)); $$->kind=EQUAL_NODE;$$->ptr.pExp1=$1;$$->ptr.pExp2=$3;}
		 | exp GREATER exp {$$=(PEXP)malloc(sizeof(struct Exp)); $$->kind=GREATER_NODE;$$->ptr.pExp1=$1;$$->ptr.pExp2=$3;}
		 | exp LESS exp {$$=(PEXP)malloc(sizeof(struct Exp)); $$->kind=LESS_NODE;$$->ptr.pExp1=$1;$$->ptr.pExp2=$3;}
		 | exp GREATER_EQUAL exp {$$=(PEXP)malloc(sizeof(struct Exp)); $$->kind=GREATER_EQUAL_NODE;$$->ptr.pExp1=$1;$$->ptr.pExp2=$3;}
		 | exp LESS_EQUAL exp {$$=(PEXP)malloc(sizeof(struct Exp)); $$->kind=LESS_EQUAL_NODE;$$->ptr.pExp1=$1;$$->ptr.pExp2=$3;}
declare	: INT sub_declare {$$=(PEXP)malloc(sizeof(struct Exp)); $$->kind=INT_NODE;$$->ptr.pExp1=$2;}
		| CHAR sub_declare {$$=(PEXP)malloc(sizeof(struct Exp)); $$->kind=CHAR_NODE;$$->ptr.pExp1=$2;}
		| FLOAT sub_declare {$$=(PEXP)malloc(sizeof(struct Exp)); $$->kind=FLOAT_NODE;$$->ptr.pExp1=$2;}
sub_declare	: id ',' sub_declare {$$=(PEXP)malloc(sizeof(struct Exp)); $$->kind=DECLARE_SUB_NODE;$$->ptr.pExp1=$1;$$->ptr.pExp2=$3;}
			| id {$$=(PEXP)malloc(sizeof(struct Exp)); $$->kind=DECLARE_SUB_NODE; $$->ptr.pExp1=$1;$$->ptr.pExp2=NULL;}
			| id_array ',' sub_declare {$$=(PEXP)malloc(sizeof(struct Exp)); $$->kind=DECLARE_SUB_NODE;$$->ptr.pExp1=$1;$$->ptr.pExp2=$3;}
			| id_array {$$=(PEXP)malloc(sizeof(struct Exp)); $$->kind=DECLARE_SUB_NODE; $$->ptr.pExp1=$1;$$->ptr.pExp2=NULL;}
exp	 : function_fire {$$=$1;}
	 | exp_unary %prec UNARY{$$=$1;}
	 | INTEGER {$$=(PEXP)malloc(sizeof(struct Exp)); $$->kind=INTEGER_NODE;$$->type_integer=$1;}
	 | id %prec VARIABLE {$$=$1;}
	 | array %prec VARIABLE{$$=$1;}
	 | exp PLUS exp {$$=(PEXP)malloc(sizeof(struct Exp)); $$->kind=PLUS_NODE;  $$->ptr.pExp1=$1;$$->ptr.pExp2=$3;}
	 | exp STAR exp {$$=(PEXP)malloc(sizeof(struct Exp)); $$->kind=STAR_NODE;  $$->ptr.pExp1=$1;$$->ptr.pExp2=$3;}
	 | exp MINUS exp {$$=(PEXP)malloc(sizeof(struct Exp)); $$->kind=MINUS_NODE;  $$->ptr.pExp1=$1;$$->ptr.pExp2=$3;}
	 | exp DIV exp {$$=(PEXP)malloc(sizeof(struct Exp)); $$->kind=DIV_NODE;  $$->ptr.pExp1=$1;$$->ptr.pExp2=$3;}
	 | LP exp RP   {$$=$2;}
assign : id ASSIGNOP exp {$$=(PEXP)malloc(sizeof(struct Exp)); $$->kind=ASSIGN_NODE; $$->ptr.pExp1=$1; $$->ptr.pExp2=$3;} 
	   | array ASSIGNOP exp {$$=(PEXP)malloc(sizeof(struct Exp)); $$->kind=ASSIGN_NODE; $$->ptr.pExp1=$1; $$->ptr.pExp2=$3;} 
exp_unary : MINUS exp     {$$=(PEXP)malloc(sizeof(struct Exp)); $$->kind=UMINUS_NODE;  $$->ptr.pExp1=$2;}  
		  | INC id  {$$=(PEXP)malloc(sizeof(struct Exp)); $$->kind=INC_PREFIX_NODE;  $$->ptr.pExp1=$2;}
		  | DEC id  {$$=(PEXP)malloc(sizeof(struct Exp)); $$->kind=DEC_PREFIX_NODE;  $$->ptr.pExp1=$2;}
		  | id INC %prec UNARY {$$=(PEXP)malloc(sizeof(struct Exp)); $$->kind=INC_SUFFIX_NODE;  $$->ptr.pExp1=$1;}
		  | id DEC %prec UNARY {$$=(PEXP)malloc(sizeof(struct Exp)); $$->kind=DEC_SUFFIX_NODE;  $$->ptr.pExp1=$1;}
id	: ID {$$=(PEXP)malloc(sizeof(struct Exp)); $$->kind=ID_NODE; strcpy($$->id.type_id,$1);
		  $$->id.dimension=0;$$->id.index1=0;$$->id.index2=0;$$->id.index3=0;}
id_array : ID MLP INTEGER MRP {$$=(PEXP)malloc(sizeof(struct Exp)); $$->kind=ID_ARRAY_NODE; strcpy($$->id.type_id,$1);
								$$->id.dimension=1;$$->id.index1=$3;$$->id.index2=0;$$->id.index3=0;}
		 | ID MLP INTEGER MRP MLP INTEGER MRP {$$=(PEXP)malloc(sizeof(struct Exp)); $$->kind=ID_ARRAY_NODE; 
									strcpy($$->id.type_id,$1);$$->id.dimension=2;$$->id.index1=$3;$$->id.index2=$6;$$->id.index3=0;}
		 | ID MLP INTEGER MRP MLP INTEGER MRP MLP INTEGER MRP
									{$$=(PEXP)malloc(sizeof(struct Exp)); $$->kind=ID_ARRAY_NODE; 
									strcpy($$->id.type_id,$1);$$->id.dimension=3;$$->id.index1=$3;$$->id.index2=$6;$$->id.index3=$9;}
array : ID MLP exp MRP {$$=(PEXP)malloc(sizeof(struct Exp)); $$->kind=ARRAY_NODE; strcpy($$->array.array_id,$1);
						$$->id.dimension=1;$$->array.pExp1=$3;$$->array.pExp2=NULL;$$->array.pExp3=NULL;}
	  | ID MLP exp MRP MLP exp MRP {$$=(PEXP)malloc(sizeof(struct Exp)); $$->kind=ARRAY_NODE; 
									strcpy($$->array.array_id,$1);$$->array.dimension=2;$$->array.pExp1=$3;$$->array.pExp2=$6;$$->array.pExp3=NULL;}
	  | ID MLP exp MRP MLP exp MRP MLP exp MRP{$$=(PEXP)malloc(sizeof(struct Exp)); $$->kind=ARRAY_NODE; 
											  strcpy($$->array.array_id,$1);$$->array.dimension=3;$$->array.pExp1=$3;
											  $$->array.pExp2=$6;$$->array.pExp3=$9;}

%%

int main(int argc, char *argv[]){
	yyin=fopen(argv[1],"r");
	if (argc >= 3) {
		if (strcmp(argv[2],"-exp1")==0) exp1 = 1;
		if (strcmp(argv[2],"-exp2")==0) exp2 = 1;
		if (strcmp(argv[2],"-exp3")==0) exp3 = 1;
		if (strcmp(argv[2],"-exp4")==0) exp4 = 1;
	}
	if (!yyin) return;
	yyparse();
	return 0;
	}
	
yyerror(char *s){
   printf("%s   %s ,line: %d\n",s,yytext,yylineno);
	if(!exp1) {
		exit(1);
	}
}


