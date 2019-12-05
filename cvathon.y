%{
#include "stdio.h"
//#include "math.h"
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
int offset[5]={0,0,0,0,0};
extern int yylineno;
%}

%union {
	int type_integer;
	char type_id[32];
	struct Exp *pExp;
};

%type  <pExp> line exp exp_unary declare sub_declare id if_line condition while_line control id_array assign for_line function function_fire return_line value_list condition_list 

%token <type_integer> INTEGER
%token <type_id> ID

%token LP RP BLP BRP MLP MRP 
%token PLUS MINUS STAR DIV ASSIGNOP
%token INT FLOAT CHAR
%token GREATER LESS EQUAL GREATER_EQUAL LESS_EQUAL
%token IF ELSE WHILE CONTINUE BREAK FOR RETURN
%token INC DEC
%token AND OR

%right INT CHAR FLOAT VOID
%right IF ELSE WHILE CONTINUE BREAK
%left EQUAL GREATER_EQUAL LESS_EQUAL GREATER LESS
%left ASSIGNOP
%left PLUS MINUS
%left STAR DIV
%right UMINUS


%%
input:
	 | input line
	 ;
line : '\n'    { ;}
	 | function '\n' {if(exp1) {display($1,nestCodeBlock*blanks);}
					if(exp2) {if(!insertIntoTable($1,nestCodeBlock,yylineno)) return;}}
	 | function_fire '\n' {if(exp1) {display($1,nestCodeBlock*blanks);}}
	 | declare '\n' {if(exp1) {display($1,nestCodeBlock*blanks);}if(exp2) {if(!insertIntoTable($1,nestCodeBlock,yylineno)) return;}}
	 | assign '\n' {if(exp1) {display($1,nestCodeBlock*blanks);}}
	 | exp_unary {if(exp1) {display($1,nestCodeBlock*blanks);}}
	 | BLP '\n' {if(exp1) {displayMessage(0,(nestCodeBlock+1)*blanks);}nestCodeBlock+=1;}
	 | BRP '\n' {tableOut(nestCodeBlock);nestCodeBlock-=1;offset[nestCodeBlock]+=offset[nestCodeBlock+1];offset[nestCodeBlock+1]=0;}
	 | if_line '\n'	{if(exp1) {display($1,nestCodeBlock*blanks);}}
	 | while_line '\n'{if(exp1) {display($1,nestCodeBlock*blanks);}}
	 | for_line '\n'{if(exp1) {display($1,nestCodeBlock*blanks);}}
	 | control '\n' {if(exp1) {display($1,nestCodeBlock*blanks);}}
	 | return_line '\n' {if(exp1) {display($1,nestCodeBlock*blanks);}}
	 | error '\n' { printf("line error!\n");}
	 ;

function : VOID ID LP RP {$$=(PEXP)malloc(sizeof(struct Exp)); $$->kind=FUNCTION_DECLARE_NODE;$$->function.returnType=VOID_FUNCTION;$$->function.pExp=NULL;strcpy($$->function.function_name,$2);}
		 | INT ID LP  RP {$$=(PEXP)malloc(sizeof(struct Exp)); $$->kind=FUNCTION_DECLARE_NODE;$$->function.returnType=INT_FUNCTION;$$->function.pExp=NULL;strcpy($$->function.function_name,$2);}
		 | CHAR ID LP  RP {$$=(PEXP)malloc(sizeof(struct Exp)); $$->kind=FUNCTION_DECLARE_NODE;$$->function.returnType=CHAR_FUNCTION;$$->function.pExp=NULL;strcpy($$->function.function_name,$2);}
		 | FLOAT ID LP RP {$$=(PEXP)malloc(sizeof(struct Exp)); $$->kind=FUNCTION_DECLARE_NODE;$$->function.returnType=FLOAT_FUNCTION;$$->function.pExp=NULL;strcpy($$->function.function_name,$2);}
		 | INT ID LP declare RP {$$=(PEXP)malloc(sizeof(struct Exp)); $$->kind=FUNCTION_DECLARE_NODE;$$->function.returnType=INT_FUNCTION;$$->function.pExp=$4;strcpy($$->function.function_name,$2);}
		 | CHAR ID LP declare RP {$$=(PEXP)malloc(sizeof(struct Exp)); $$->kind=FUNCTION_DECLARE_NODE;$$->function.returnType=CHAR_FUNCTION;$$->function.pExp=$4;strcpy($$->function.function_name,$2);}
		 | FLOAT ID LP declare RP {$$=(PEXP)malloc(sizeof(struct Exp)); $$->kind=FUNCTION_DECLARE_NODE;$$->function.returnType=FLOAT_FUNCTION;$$->function.pExp=$4;strcpy($$->function.function_name,$2);}
function_fire : ID LP RP {$$=(PEXP)malloc(sizeof(struct Exp)); $$->kind=FUNCTION_FIRE_NODE;strcpy($$->fire.fire_id,$1);$$->fire.valueList=NULL;}
			  | ID LP value_list RP {$$=(PEXP)malloc(sizeof(struct Exp)); $$->kind=FUNCTION_FIRE_NODE;strcpy($$->fire.fire_id,$1);$$->fire.valueList=$3;}
value_list : exp {$$=(PEXP)malloc(sizeof(struct Exp)); $$->kind=VALUE_LIST_NODE; $$->ptr.pExp1=$1;$$->ptr.pExp2=NULL;}
		   | exp ',' value_list {$$=(PEXP)malloc(sizeof(struct Exp)); $$->kind=VALUE_LIST_NODE; $$->ptr.pExp1=$1; $$->ptr.pExp2=$3;}
control : CONTINUE  {$$=(PEXP)malloc(sizeof(struct Exp)); $$->kind=CONTINUE_NODE;}
		| BREAK 	{$$=(PEXP)malloc(sizeof(struct Exp)); $$->kind=BREAK_NODE;}
		| ELSE 		{if(exp1) displayMessage(1,nestCodeBlock*blanks);$$=(PEXP)malloc(sizeof(struct Exp)); $$->kind=ELSE_NODE;}
return_line : RETURN {$$=(PEXP)malloc(sizeof(struct Exp)); $$->kind=RETURN_NODE;$$->return_exp.returnType=RETURN_VOID;}
			| RETURN exp {$$=(PEXP)malloc(sizeof(struct Exp)); $$->kind=RETURN_NODE;$$->return_exp.returnType=RETURN_EXP;$$->return_exp.pExp=$2;}
while_line : WHILE LP condition_list RP {$$=(PEXP)malloc(sizeof(struct Exp)); $$->kind=WHILE_NODE;$$->ptr.pExp1=$3;}
for_line: FOR LP assign ';' condition_list ';' exp_unary RP {$$=(PEXP)malloc(sizeof(struct Exp)); $$->kind=FOR_NODE;$$->for_exp.p1=$3;$$->for_exp.p2=$5;$$->for_exp.p3=$7;}
		| FOR LP ';' condition_list ';' exp_unary RP {$$=(PEXP)malloc(sizeof(struct Exp)); $$->kind=FOR_NODE;$$->for_exp.p1=NULL;$$->for_exp.p2=$4;$$->for_exp.p3=$6;}
		| FOR LP assign ';' condition_list ';'  RP {$$=(PEXP)malloc(sizeof(struct Exp)); $$->kind=FOR_NODE;$$->for_exp.p1=$3;$$->for_exp.p2=$5;$$->for_exp.p3=NULL;}
		| FOR LP  ';' condition_list ';'  RP {$$=(PEXP)malloc(sizeof(struct Exp)); $$->kind=FOR_NODE;$$->for_exp.p1=NULL;$$->for_exp.p2=$4;$$->for_exp.p3=NULL;}
if_line : IF LP condition_list RP {$$=(PEXP)malloc(sizeof(struct Exp)); $$->kind=IF_NODE;$$->ptr.pExp1=$3;}
condition_list: condition {$$=(PEXP)malloc(sizeof(struct Exp)); $$->kind=CONDITION_LIST_NODE; $$->condition_list.pExp1=$1;$$->condition_list.pExp2=NULL;}
			  | condition AND condition_list {$$=(PEXP)malloc(sizeof(struct Exp)); $$->kind=CONDITION_LIST_NODE; $$->condition_list.pExp1=$1;$$->condition_list.pExp2=$3;$$->condition_list.type=LINK_AND;}
			  | condition OR condition_list {$$=(PEXP)malloc(sizeof(struct Exp)); $$->kind=CONDITION_LIST_NODE; $$->condition_list.pExp1=$1;$$->condition_list.pExp2=$3;$$->condition_list.type=LINK_OR;}
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
	 | assign {$$=$1;}
	 | exp_unary {$$=$1;}
	 | INTEGER {$$=(PEXP)malloc(sizeof(struct Exp)); $$->kind=INTEGER_NODE;$$->type_integer=$1;}
	 | id {$$=$1;}
	 | exp PLUS exp {$$=(PEXP)malloc(sizeof(struct Exp)); $$->kind=PLUS_NODE;  $$->ptr.pExp1=$1;$$->ptr.pExp2=$3;}
	 | exp STAR exp {$$=(PEXP)malloc(sizeof(struct Exp)); $$->kind=STAR_NODE;  $$->ptr.pExp1=$1;$$->ptr.pExp2=$3;}
	 | exp MINUS exp {$$=(PEXP)malloc(sizeof(struct Exp)); $$->kind=MINUS_NODE;  $$->ptr.pExp1=$1;$$->ptr.pExp2=$3;}
	 | exp DIV exp {$$=(PEXP)malloc(sizeof(struct Exp)); $$->kind=DIV_NODE;  $$->ptr.pExp1=$1;$$->ptr.pExp2=$3;}
	 | LP exp RP   {$$=$2;}
assign : id ASSIGNOP exp {$$=(PEXP)malloc(sizeof(struct Exp)); $$->kind=ASSIGN_NODE; $$->ptr.pExp1=$1; $$->ptr.pExp2=$3;} 
exp_unary : MINUS exp   %prec UMINUS  {$$=(PEXP)malloc(sizeof(struct Exp)); $$->kind=UMINUS_NODE;  $$->ptr.pExp1=$2;}  
		  | INC id %prec UMINUS {$$=(PEXP)malloc(sizeof(struct Exp)); $$->kind=INC_PREFIX_NODE;  $$->ptr.pExp1=$2;}
		  | DEC id %prec UMINUS {$$=(PEXP)malloc(sizeof(struct Exp)); $$->kind=DEC_PREFIX_NODE;  $$->ptr.pExp1=$2;}
		  | id INC %prec UMINUS {$$=(PEXP)malloc(sizeof(struct Exp)); $$->kind=INC_SUFFIX_NODE;  $$->ptr.pExp1=$1;}
		  | id DEC %prec UMINUS {$$=(PEXP)malloc(sizeof(struct Exp)); $$->kind=DEC_SUFFIX_NODE;  $$->ptr.pExp1=$1;}
id	: ID {$$=(PEXP)malloc(sizeof(struct Exp)); $$->kind=ID_NODE; strcpy($$->id.type_id,$1);}
id_array : ID MLP INTEGER MRP {$$=(PEXP)malloc(sizeof(struct Exp)); $$->kind=ID_ARRAY_NODE; strcpy($$->id.type_id,$1);
								$$->id.dimension=1;$$->id.index1=$3;}
		 | ID MLP INTEGER MRP MLP INTEGER MRP {$$=(PEXP)malloc(sizeof(struct Exp)); $$->kind=ID_ARRAY_NODE; 
									strcpy($$->id.type_id,$1);$$->id.dimension=2;$$->id.index1=$3;$$->id.index2=$6;}
		 | ID MLP INTEGER MRP MLP INTEGER MRP MLP INTEGER MRP
									{$$=(PEXP)malloc(sizeof(struct Exp)); $$->kind=ID_ARRAY_NODE; 
									strcpy($$->id.type_id,$1);$$->id.dimension=2;$$->id.index1=$3;$$->id.index2=$6;$$->id.index3=$9;}
	
%%

int main(int argc, char *argv[]){
	yyin=fopen(argv[1],"r");
	if (argc >= 3) {
		if (strcmp(argv[2],"-exp1")==0) exp1 = 1;
		if (strcmp(argv[2],"-exp2")==0) exp2 = 1;
	}
	if (!yyin) return;
	yyparse();
	return 0;
	}
	
yyerror(char *s){
   printf("%s   %s \n",s,yytext);
 }


