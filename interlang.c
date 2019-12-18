#include "Node.h"
#include <stdio.h>

extern int getIntLen(int);
extern void log(char *);

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
    struct CodeNode *next, *pre;
} * CodeNodeP;

void printCodeToFile()
{
    log("start print\n");
    if (codeList == NULL)
    {
        log("intermediate code unvailable\n");
        exit(1);
    }
    FILE *fp;
    if ((fp = fopen("test.inter", "w")) == NULL)
    {
        log("file cannot open \n");
        exit(1);
    }
    else
    {
        log("file opened for writing \n");
    }

    CodeNodeP current = codeList;
    while (current != NULL)
    {
        switch (current->op)
        {
        case OP_LABEL:
            fprintf(fp, "LABEL %s :\n", current->result);
            break;
        case OP_FUNCTION:
            fprintf(fp, "FUNCTION %s :\n", current->result);
            break;
        case OP_ASSIGN:
            fprintf(fp, "%s := %s\n", current->result, current->opn1);
            break;
        case OP_PLUS:
            fprintf(fp, "%s := %s + %s\n", current->result, current->opn1, current->opn2);
            break;
        case OP_MINUS:
            fprintf(fp, "%s := %s - %s\n", current->result, current->opn1, current->opn2);
            break;
        case OP_STAR:
            fprintf(fp, "%s := %s * %s\n", current->result, current->opn1, current->opn2);
            break;
        case OP_DIV:
            fprintf(fp, "%s := %s / %s\n", current->result, current->opn1, current->opn2);
            break;
        case OP_GOTO:
            fprintf(fp, "GOTO %s\n", current->result);
            break;
        case OP_RETURN:
            fprintf(fp, "RETURN %s\n", current->result);
            break;
        case OP_ARG:
            fprintf(fp, "ARG %s\n", current->result);
            break;
        case OP_CALL:
            fprintf(fp, "%s :=CALL %s\n", current->result, current->opn1);
            break;
        case OP_PARAM:
            fprintf(fp, "PARAM %s\n", current->result);
            break;
        case OP_GREATER:
            fprintf(fp, "IF %s > %s GOTO %s\n", current->opn1, current->opn2, current->result);
            break;
        case OP_LESS:
            fprintf(fp, "IF %s < %s GOTO %s\n", current->opn1, current->opn2, current->result);
            break;
        case OP_GE:
            fprintf(fp, "IF %s >= %s GOTO %s\n", current->opn1, current->opn2, current->result);
            break;
        case OP_LE:
            fprintf(fp, "IF %s <= %s GOTO %s\n", current->opn1, current->opn2, current->result);
            break;
        case OP_EQUAL:
            fprintf(fp, "IF %s == %s GOTO %s\n", current->opn1, current->opn2, current->result);
            break;
        case OP_UE:
            fprintf(fp, "IF %s != %s GOTO %s\n", current->opn1, current->opn2, current->result);
            break;
        }
        current = current->next;
    }

    if (fclose(fp) != 0)
    {
        log("file cannot be closed \n");
        exit(1);
    }
    else
    {
        log("print finished\n");
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
    while(currentCode->next != NULL) {
        currentCode = currentCode->next;
    }
}

CodeNodeP getCode(enum Operation op, char opn1[], char opn2[], char result[])
{
    CodeNodeP temp = (CodeNodeP)malloc(sizeof(struct CodeNode));
    temp->op = op;
    if (opn1 != NULL)
    {
        strcpy(temp->opn1, opn1);
    }
    if (opn2 != NULL)
    {
        strcpy(temp->opn2, opn2);
    }
    if (result != NULL)
    {
        strcpy(temp->result, result);
    }
    temp->next = NULL;
    temp->pre = NULL;
}

char *intToStr(int num)
{
    int len = getIntLen(num);
    char *tempStr = (char *)malloc(sizeof(char) * (len + 2));
    tempStr[0] = '#';
    int i = len;
    while (num > 0)
    {
        tempStr[i] = num % 10 + '0';
        num /= 10;
        i--;
    }
    tempStr[len+1]='\0';
    return tempStr;
}

char *newTemp()
{
    temp_index++;
    int len = getIntLen(temp_index);
    char *temp = (char *)malloc(sizeof(char) * (1 + len + 1));
    temp[0] = 't';
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
    label[0] = 'l';
    label[1] = 'a';
    label[2] = 'b';
    label[3] = 'e';
    label[4] = 'l';
    int index = label_index;
    for (int i = len; i >= 1; i--)
    {
        label[i] = index % 10 + '0';
        index /= 10;
    }
    label[len + 1] = '\0';
    return label;
}

//1-preffix ,0-suffix, -1-not exist
int isIncOrDecExist(PEXP exp)
{
    if (exp == NULL)
    {
        return -1;
    }
    switch (exp->kind)
    {
    case INC_PREFIX_NODE:
    case DEC_PREFIX_NODE:
        return 1;
    case INC_SUFFIX_NODE:
    case DEC_SUFFIX_NODE:
        return 0;

    case PLUS_NODE:
    case MINUS_NODE:
    case STAR_NODE:
    case DIV_NODE:
    case UMINUS_NODE:
        if (exp->ptr.pExp1 != NULL)
        {
            int result;
            if ((result = isIncOrDecExist(exp->ptr.pExp1)) != -1)
            {
                return result;
            }
        }
        if (exp->ptr.pExp2 != NULL)
        {
            int result;
            if ((result = isIncOrDecExist(exp->ptr.pExp2)) != -1)
            {
                return result;
            }
        }
        return -1;
    default:
        return -1;
    }
}

CodeNodeP mergeCode(CodeNodeP p1, CodeNodeP p2)
{
    CodeNodeP current = p1;
    while(current->next != NULL) {
        current = current -> next;
    }
    current->next=p2;
    return p1;
}

CodeNodeP translateExp(PEXP exp, char place[])
{
    log("start translate exp");
    CodeNodeP tempCode = NULL;
    char zero[] = "#0";
    char one[] = "#1";
    switch (exp->kind)
    {
    case INTEGER_NODE:
    {
        int value = exp->type_integer;
        char *value_str = intToStr(value);
        tempCode = getCode(OP_ASSIGN, value_str, NULL, place);
        break;
    }
    case ID_NODE:
    {
        char *alias = getVariableAlias(exp->id.type_id);
        tempCode = getCode(OP_ASSIGN, alias, NULL, place);
        break;
    }
    case ASSIGN_NODE:
    {
        char *alias = getVariableAlias((exp->ptr.pExp1)->id.type_id);
        log(alias);
        char *t1 = newTemp();
        CodeNodeP p1, p2;
        p1 = translateExp(exp->ptr.pExp2, t1);
        p2 = getCode(OP_ASSIGN, t1, NULL, alias);
        tempCode = mergeCode(p1,p2);
        break;
    }
    case PLUS_NODE:
    {
        char *t1 = newTemp();
        char *t2 = newTemp();
        CodeNodeP first, next, third;
        if (isIncOrDecExist(exp->ptr.pExp1) == 0 || isIncOrDecExist(exp->ptr.pExp2) == 1)
        {
            first = translateExp(exp->ptr.pExp2, t1);
            next = translateExp(exp->ptr.pExp1, t2);
        }
        else
        {
            first = translateExp(exp->ptr.pExp1, t1);
            next = translateExp(exp->ptr.pExp2, t2);
        }
        tempCode = mergeCode(first,next);
        third = getCode(OP_PLUS, t1, t2, place);
        tempCode = mergeCode(tempCode, third);
        break;
    }
    case MINUS_NODE:
    {
        char *t1 = newTemp();
        char *t2 = newTemp();
        CodeNodeP first, next, third;
        if (isIncOrDecExist(exp->ptr.pExp1) == 0 || isIncOrDecExist(exp->ptr.pExp2) == 1)
        {
            first = translateExp(exp->ptr.pExp2, t1);
            next = translateExp(exp->ptr.pExp1, t2);
        }
        else
        {
            first = translateExp(exp->ptr.pExp1, t1);
            next = translateExp(exp->ptr.pExp2, t2);
        }
        tempCode = mergeCode(first,next);
        third = getCode(OP_MINUS, t1, t2, place);
        tempCode = mergeCode(tempCode, third);
        break;
    }
    case STAR_NODE:
    {
        char *t1 = newTemp();
        char *t2 = newTemp();
        CodeNodeP first, next, third;
        if (isIncOrDecExist(exp->ptr.pExp1) == 0 || isIncOrDecExist(exp->ptr.pExp2) == 1)
        {
            first = translateExp(exp->ptr.pExp2, t1);
            next = translateExp(exp->ptr.pExp1, t2);
        }
        else
        {
            first = translateExp(exp->ptr.pExp1, t1);
            next = translateExp(exp->ptr.pExp2, t2);
        }
        tempCode = mergeCode(first,next);
        third = getCode(OP_STAR, t1, t2, place);
        tempCode = mergeCode(tempCode, third);
        break;
    }
    case DIV_NODE:
    {
        char *t1 = newTemp();
        char *t2 = newTemp();
        CodeNodeP first, next, third;
        if (isIncOrDecExist(exp->ptr.pExp1) == 0 || isIncOrDecExist(exp->ptr.pExp2) == 1)
        {
            first = translateExp(exp->ptr.pExp2, t1);
            next = translateExp(exp->ptr.pExp1, t2);
        }
        else
        {
            first = translateExp(exp->ptr.pExp1, t1);
            next = translateExp(exp->ptr.pExp2, t2);
        }
        tempCode = mergeCode(first,next);
        third = getCode(OP_DIV, t1, t2, place);
        tempCode = mergeCode(tempCode, third);
        break;
    }

    case UMINUS_NODE:
    {
        char *t1 = newTemp();
        tempCode = translateExp(exp->ptr.pExp1, t1);
        tempCode->next = getCode(OP_MINUS, zero, t1, place);
        break;
    }
    case INC_PREFIX_NODE:
    case INC_SUFFIX_NODE:
    {
        char *t1 = newTemp;
        tempCode = getCode(OP_PLUS, exp->ptr.pExp1, one, t1);
        tempCode->next = getCode(OP_ASSIGN, t1, NULL, place);
        break;
    }
    case DEC_SUFFIX_NODE:
    case DEC_PREFIX_NODE:
    {
        char *t1 = newTemp;
        tempCode = getCode(OP_MINUS, exp->ptr.pExp1, one, t1);
        tempCode->next = getCode(OP_ASSIGN, t1, NULL, place);
        break;
    }
    }
    return tempCode;
}

CodeNodeP translateCond(PEXP exp, char label_true, char label_false)
{
}

CodeNodeP translateStmt(PEXP exp)
{
    log("start translate stmt\n");
    CodeNodeP tempCode;
    switch (exp->kind)
    {
    case INTEGER_NODE:
    case ID_NODE:
    case ASSIGN_NODE:
    case PLUS_NODE:
    case MINUS_NODE:
    case STAR_NODE:
    case DIV_NODE:
    case UMINUS_NODE:
    case INC_PREFIX_NODE:
    case INC_SUFFIX_NODE:
    case DEC_SUFFIX_NODE:
    case DEC_PREFIX_NODE:
        tempCode = translateExp(exp, NULL);
        addToList(tempCode);
        break;
    }
    return tempCode;
}