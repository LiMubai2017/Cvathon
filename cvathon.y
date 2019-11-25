%{
#include "stdio.h"
#include "math.h"
#include "Node.h"
#include "string.h"
extern char *yytext;
extern FILE *yyin;
extern int blanks;
void display(struct Exp *,int);
void displayMessage(int , int);
int nestCodeBlock = 0;
int exp1 = 0;
%}

%union {
	int type_integer;
	char type_id[32];
	struct Exp *pExp;
};

%type  <pExp> line exp declare sub_declare id if_line

%token <type_integer> INTEGER
%token <type_id> ID

%token LP RP BLP BRP  
%token PLUS MINUS STAR DIV ASSIGNOP
%token INT FLOAT CHAR
%token GREATER LESS EQUAL GREATER_EQUAL LESS_EQUAL
%token IF ELSE WHILE CONTINUE BREAK
%token INC DEC

%right INT CHAR FLOAT
%right IF ELSE WHILE CONTINUE BREAK
%left EQUAL GREATER_EQUAL LESS_EQUAL GREATER LESS
%left ASSIGNOP
%left PLUS MINUS
%left STAR DIV
%right UMINUS
%right INC_PREFIX DEC_PREFIX
%left INC_SUFFIX DEC_SUFFIX 


%%
input:
	 | input line
	 ;
line : '\n'    { ;}
	 | declare '\n' {display($1,nestCodeBlock*blanks);}
	 | exp '\n' {display($1,nestCodeBlock*blanks);}
	 | BLP '\n' {displayMessage(0,(nestCodeBlock+1)*blanks);nestCodeBlock+=2;}
	 | BRP '\n' {nestCodeBlock-=2;}
	 | if_line '\n'	{display($1,nestCodeBlock*blanks);}
	 | ELSE '\n' {displayMessage(1,nestCodeBlock*blanks);}
	 | error '\n' { printf("line error!\n");}
	 ;
if_line : IF LP if_line RP {$$=(PEXP)malloc(sizeof(struct Exp)); $$->kind=IF_NODE;$$->ptr.pExp1=$3;}
		| exp EQUAL exp {$$=(PEXP)malloc(sizeof(struct Exp)); $$->kind=EQUAL_NODE;$$->ptr.pExp1=$1;$$->ptr.pExp2=$3;}
		| exp GREATER exp {$$=(PEXP)malloc(sizeof(struct Exp)); $$->kind=GREATER_NODE;$$->ptr.pExp1=$1;$$->ptr.pExp2=$3;}
		| exp LESS exp {$$=(PEXP)malloc(sizeof(struct Exp)); $$->kind=LESS_NODE;$$->ptr.pExp1=$1;$$->ptr.pExp2=$3;}
		| exp GREATER_EQUAL exp {$$=(PEXP)malloc(sizeof(struct Exp)); $$->kind=GREATER_EQUAL_NODE;$$->ptr.pExp1=$1;$$->ptr.pExp2=$3;}
		| exp LESS_EQUAL exp {$$=(PEXP)malloc(sizeof(struct Exp)); $$->kind=LESS_EQUAL_NODE;$$->ptr.pExp1=$1;$$->ptr.pExp2=$3;}
declare	: INT sub_declare {$$=(PEXP)malloc(sizeof(struct Exp)); $$->kind=INT_NODE;$$->ptr.pExp1=$2;}
		| CHAR sub_declare {$$=(PEXP)malloc(sizeof(struct Exp)); $$->kind=CHAR_NODE;$$->ptr.pExp1=$2;}
		| FLOAT sub_declare {$$=(PEXP)malloc(sizeof(struct Exp)); $$->kind=FLOAT_NODE;$$->ptr.pExp1=$2;}
sub_declare	: id ',' sub_declare {$$=(PEXP)malloc(sizeof(struct Exp)); $$->kind=DECLARE_SUB_NODE;$$->ptr.pExp1=$1;$$->ptr.pExp2=$3;}
			| id {$$=(PEXP)malloc(sizeof(struct Exp)); $$->kind=DECLARE_SUB_NODE; $$->ptr.pExp1=$1;$$->ptr.pExp2=NULL;}
exp	 : id ASSIGNOP exp {$$=(PEXP)malloc(sizeof(struct Exp)); $$->kind=ASSIGN_NODE; $$->ptr.pExp1=$1; $$->ptr.pExp2=$3;}
	 | INTEGER {$$=(PEXP)malloc(sizeof(struct Exp)); $$->kind=INTEGER_NODE;$$->type_integer=$1;}
	 | id {$$=$1;}
	 | exp PLUS exp {$$=(PEXP)malloc(sizeof(struct Exp)); $$->kind=PLUS_NODE;  $$->ptr.pExp1=$1;$$->ptr.pExp2=$3;}
	 | exp STAR exp {$$=(PEXP)malloc(sizeof(struct Exp)); $$->kind=STAR_NODE;  $$->ptr.pExp1=$1;$$->ptr.pExp2=$3;}
	 | exp MINUS exp {$$=(PEXP)malloc(sizeof(struct Exp)); $$->kind=MINUS_NODE;  $$->ptr.pExp1=$1;$$->ptr.pExp2=$3;}
	 | exp DIV exp {$$=(PEXP)malloc(sizeof(struct Exp)); $$->kind=DIV_NODE;  $$->ptr.pExp1=$1;$$->ptr.pExp2=$3;}
exp	 : LP exp RP   {$$=$2;//(PEXP)malloc(sizeof(struct Exp)); $$->kind=LPRP_NODE;  $$->ptr.pExp1=$2;
		       }
     | MINUS exp   %prec UMINUS  {$$=(PEXP)malloc(sizeof(struct Exp)); $$->kind=UMINUS_NODE;  $$->ptr.pExp1=$2;}  
	 | id INC %prec INC_SUFFIX {$$=(PEXP)malloc(sizeof(struct Exp)); $$->kind=INC_SUFFIX_NODE;  $$->ptr.pExp1=$1;}
	 | id DEC %prec DEC_SUFFIX {$$=(PEXP)malloc(sizeof(struct Exp)); $$->kind=DEC_SUFFIX_NODE;  $$->ptr.pExp1=$1;}
	 | INC id %prec INC_PREFIX {$$=(PEXP)malloc(sizeof(struct Exp)); $$->kind=INC_PREFIX_NODE;  $$->ptr.pExp1=$2;}
	 | DEC id %prec DEC_PREFIX {$$=(PEXP)malloc(sizeof(struct Exp)); $$->kind=DEC_PREFIX_NODE;  $$->ptr.pExp1=$2;}
	 | error  RP {printf("cha )\n");}
id	: ID {$$=(PEXP)malloc(sizeof(struct Exp)); $$->kind=ID_NODE; strcpy($$->type_id,$1);}
	;
%%

int main(int argc, char *argv[]){
	yyin=fopen(argv[1],"r");
	if (argc >= 3 && strcmp(argv[2],"-exp1")==0) {
		exp1 = 1;
	}
	if (!yyin) return;
	yyparse();
	return 0;
	}
	
yyerror(char *s){
   printf("%s   %s \n",s,yytext);
 }


