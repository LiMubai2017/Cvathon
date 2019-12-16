#include "symbaltable.c"
#include <stdio.h>

int temp_index = 0;
int label_index = 0;

struct CodeNode *codeList = NULL;
struct CodeNode *currentCode = NULL;

enum Operation
{
    OP_LABEL,
    OP_FUNCTION,
    OP_ASSIGN,
    OP_PLUS,
    OP_MINUS,
    OP_STAR,
    OP_DIV,
    OP_GOTO,
    OP_RETURN,
    OP_ARG,
    OP_CALL,
    OP_PARAM,
    OP_GREATER,
    OP_LESS,
    OP_GE,
    OP_LE,
    OP_EQUAL,
    OP_UE
};

typedef struct CodeNode
{
    enum Operation op;
    char opn1[10], opn2[10], result[10];
    CodeNodeP next, pre;
} * CodeNodeP;

void printCodeToFile()
{
    if (codeList == NULL)
    {
        printf("intermediate code unvailable\n");
        return;
    }
    FILE *fp;
    if ((fp = fopen("test.inter", "w")) == NULL)
    {
        printf("file cannot open \n");
        return;
    }
    else
    {
        printf("file opened for writing \n");
    }

    CodeNodeP current = codeList;
    while (current != NULL)
    {
        switch (current->op)
        {
            case OP_LABEL:
                fprintf(fp,"LABEL %s :\n",current->result);
                break;
            case OP_FUNCTION:
                fprintf(fp,"FUNCTION %s :\n",current->result);
                break;
            case OP_ASSIGN:
                fprintf(fp,"%s := %s\n",current->opn1,current->result);
                break;
            case OP_PLUS:
                fprintf(fp,"%s := %s + %s\n",current->result,current->opn1,current->opn2);
                break;
            case OP_MINUS:
                fprintf(fp,"%s := %s - %s\n",current->result,current->opn1,current->opn2);
                break;
            case OP_STAR:
                fprintf(fp,"%s := %s * %s\n",current->result,current->opn1,current->opn2);
                break;
            case OP_DIV:
                fprintf(fp,"%s := %s / %s\n",current->result,current->opn1,current->opn2);
                break;
            case OP_GOTO:
                fprintf(fp,"GOTO %s\n",current->result);
                break;
            case OP_RETURN:
                fprintf(fp,"RETURN %s\n",current->result);
                break;
            case OP_ARG:
                fprintf(fp,"ARG %s\n",current->result);
                break;
            case OP_CALL:
                fprintf(fp,"%s :=CALL %s\n",current->result,current->opn1);
                break;
            case OP_PARAM:
                fprintf(fp,"PARAM %s\n",current->result);
                break;
            case OP_GREATER:
                fprintf(fp,"IF %s > %s GOTO %s\n",current->opn1,current->opn2,current->result);
                break;
            case OP_LESS:
                fprintf(fp,"IF %s < %s GOTO %s\n",current->opn1,current->opn2,current->result);
                break;
            case OP_GE:
                fprintf(fp,"IF %s >= %s GOTO %s\n",current->opn1,current->opn2,current->result);
                break;
            case OP_LE:
                fprintf(fp,"IF %s <= %s GOTO %s\n",current->opn1,current->opn2,current->result);
                break;
            case OP_EQUAL:
                fprintf(fp,"IF %s == %s GOTO %s\n",current->opn1,current->opn2,current->result);
                break;
            case OP_UE:
                fprintf(fp,"IF %s != %s GOTO %s\n",current->opn1,current->opn2,current->result);
                break;
        }
    }

    if (fclose(fp) != 0)
    {
        printf("file cannot be closed \n");
    }
    else
    {
        printf("print finished\n");
    }
    return 0;
}

void addToList(CodeNodeP temp)
{
    if (codeList != NULL)
    {
        currentCode->next = temp;
        temp->pre = currentCode;
        currentCode = temp;
    }
    else
    {
        codeList = currentCode = temp;
    }
}

CodeNodeP getCode(enum Operation op, char opn1[], char opn2[], char result[])
{
    CodeNodeP temp = (CodeNodeP)malloc(sizeof(struct CodeNode));
    temp->op = op;
    strcpy(temp->opn1, opn1);
    strcpy(temp->opn2, opn2);
    strcpy(temp->result, result);
    temp->next = NULL;
    temp->pre = NULL;
}

int getIntLen(int num)
{
    if (num <= 0)
        return 1;
    int val = 0;
    while (num > 0)
    {
        val++;
        num /= 10;
    }
    return val;
}

char *newTemp()
{
    temp_index++;
    int len = getIntLen(temp_index);
    char *temp = (char *)malloc(sizeof(char) * (1 + len + 1));
    temp[0] = 'V';
    int index = temp_index;
    for (int i = len; i >= 1; i--)
    {
        temp[i] = index % 10 + '0';
        index /= 10;
    }
    temp[len + 1] = '\0';
    return temp;
}

char *newLabel()
{
    label_index++;
    int len = getIntLen(label_index);
    char *label = (char *)malloc(sizeof(char) * (5 + len + 1));
    label[0] = 'l';label[1]='a';label[2]='b';label[3]='e';label[4]='l';
    int index = label_index;
    for (int i = len; i >= 1; i--)
    {
        label[i] = index % 10 + '0';
        index /= 10;
    }
    label[len + 1] = '\0';
    return label;
}

