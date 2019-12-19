%option yylineno
%x COMMENT
%{
#include "cvathon.tab.h"
#include "string.h"
#include "Node.h"

typedef union {
	int type_int;
	char type_id[32];
	PEXP *pexp;
} YYLVAL;
#define YYSTYPE YYLVAL
extern int exp1;
extern int showWordArray;
extern int exp2;
extern int exp3;
ARRAYP array = NULL,current;
%}
id   [A-Za-z][A-Za-z0-9]*  
integer    [0-9]+
%%
"//".* {}
"/*" {BEGIN COMMENT;}
<COMMENT>"*/" {BEGIN INITIAL;}
<COMMENT>. {}
{integer}      {addWord(INTEGER_ARRAY,yytext);yylval.type_integer=atoi(yytext); return INTEGER;}
int			{addWord(INT_ARRAY,yytext);return INT;}
char		{addWord(CHAR_ARRAY,yytext);return CHAR;}
float		{addWord(FLOAT_ARRAY,yytext);return FLOAT;}
void		{return VOID;}
if			{addWord(IF_ARRAY,yytext);return IF;}
else		{addWord(ELSE_ARRAY,yytext);return ELSE;}
while		{addWord(WHILE_ARRAY,yytext);return WHILE;}
"for"		{addWord(FOR_ARRAY,yytext);return FOR;}
break		{addWord(BREAK_ARRAY,yytext);return BREAK;}
continue	{addWord(CONTINUE_ARRAY,yytext);return CONTINUE;}
return		{return RETURN;}
{id}       {addWord(ID_ARRAY,yytext);strcpy(yylval.type_id,  yytext); return ID;}
(==)	   {addWord(EQUAL_ARRAY,yytext);return EQUAL;}
(>=)	   {addWord(GREATER_EQUAL_ARRAY,yytext);return GREATER_EQUAL;}
(<=)	   {addWord(LESS_EQUAL_ARRAY,yytext);return LESS_EQUAL;}
"++"	   {addWord(INC_ARRAY,yytext);return INC;}
"--"	   {addWord(DEC_ARRAY,yytext);return DEC;}
"||"	   {return OR;}
"&&"	   {return AND;}
"!"		   {return NOT;}
[=]        {addWord(ASSIGNOP_ARRAY,yytext);return ASSIGNOP;}
[+]        {addWord(PLUS_ARRAY,yytext);return PLUS;}
[-]        {addWord(MINUS_ARRAY,yytext);return MINUS;}
[*]        {addWord(STAR_ARRAY,yytext);return STAR;}
[/]        {addWord(DIV_ARRAY,yytext);return DIV;}
[(]        {addWord(LP_ARRAY,yytext);return LP;}
[)]        {addWord(RP_ARRAY,yytext);return RP;}
[{]	   	   {addWord(BLP_ARRAY,yytext);return BLP;}
[}]	       {addWord(BRP_ARRAY,yytext);return BRP;}
[\[]	   {addWord(MLP_ARRAY,yytext);return MLP;}
[\]]	   {addWord(MRP_ARRAY,yytext);return MRP;}
[,]        {return yytext[0];}  
[;]        {return yytext[0];}   
[ \t]      {;}   
[\n]       {return yytext[0];}   
[>]		   {addWord(GREATER_ARRAY,yytext);return GREATER;}
[<]		   {addWord(LESS_ARRAY,yytext);return LESS;}
.          {printf("error in line %d\n",yylineno);}
<<EOF>>	   {if(exp1&&showWordArray) {displayMessage(2,0);displayWordsArray(array);}
			if(exp3) {printCodeToFile();}yyterminate();}
%%
/*
int main()
{
yylex();
return 0;
}
*/

void addWord(enum array_kind kind, char value[]) {
	if(array==NULL) {
		array = (ARRAYP)malloc(sizeof(struct Array));
		current = array;
	}	else {
		current->next = (ARRAYP)malloc(sizeof(struct Array));
		current = current->next;
	}
	current->next=NULL;
	current->kind=kind;
	strcpy(current->value,value);
}


int yywrap()
{
	return 1;
}

