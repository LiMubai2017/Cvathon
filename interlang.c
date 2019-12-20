#include "Node.h"
#include <stdio.h>

extern int getIntLen(int);
extern void log(char *);
extern int nestCodeBlock;

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
    OP_UE,
    OP_READ,
    OP_WRITE,
    OP_DEC,
};

typedef struct CodeNode
{
    enum Operation op;
    char opn1[10], opn2[10], result[10];
    struct CodeNode *next, *pre;
} * CodeNodeP;

void insertStrIntoArray(char*, char**);
CodeNodeP translateExp(PEXP exp, char place[]);
CodeNodeP _translateArrayDeclare(enum node_kind kind , PEXP array_node);

int temp_index = 0;
int label_index = 0;

CodeNodeP codeList[6] = {NULL, NULL, NULL, NULL, NULL, NULL};
CodeNodeP currentCode[6] = {NULL, NULL, NULL, NULL, NULL, NULL};
PEXP nodeToParse[6] = {NULL, NULL, NULL, NULL, NULL, NULL};

void printCodeToFile()
{
    log("start print\n");
    if (codeList[0] == NULL)
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

    CodeNodeP current = codeList[0];
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
        case OP_READ:
            fprintf(fp, "READ %s\n", current->result);
            break;
        case OP_WRITE:
            fprintf(fp, "WRITE %s\n", current->result);
            break;
        case OP_DEC:
            fprintf(fp, "DEC %s %s\n", current->result,current->opn1);
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
    if(temp == NULL) {
        return;
    }
    if (codeList[nestCodeBlock] != NULL)
    {
        currentCode[nestCodeBlock]->next = temp;
        temp->pre = currentCode;
        currentCode[nestCodeBlock] = temp;
    }
    else
    {
        codeList[nestCodeBlock] = currentCode[nestCodeBlock] = temp;
    }
    while (currentCode[nestCodeBlock]->next != NULL)
    {
        currentCode[nestCodeBlock] = currentCode[nestCodeBlock]->next;
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
    while (i > 0)
    {
        tempStr[i] = num % 10 + '0';
        num /= 10;
        i--;
    }
    tempStr[len + 1] = '\0';
    log(tempStr);
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
    for (int i = len+4; i >= 5; i--)
    {
        label[i] = index % 10 + '0';
        index /= 10;
    }
    label[len + 5] = '\0';
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
    if(p1 == NULL) {
        return p2;
    } else {
        if(p2 == NULL) {
            return p1;
        }
    }

    CodeNodeP p = p1;
    while (p->next != NULL)
    {
        p = p->next;
    }
    p->next = p2;
    p2->pre = p;
    return p1;
}

CodeNodeP translateArgs(PEXP arg_node, char **arg_list)
{
    switch (arg_node->kind)
    {
    case VALUE_LIST_NODE:
    {
        char *t1 = newTemp();
        CodeNodeP code1 = translateExp(arg_node->ptr.pExp1,t1);
        insertStrIntoArray(t1 , arg_list);
        if(arg_node->ptr.pExp2 == NULL) {
            return code1;
        } else {
            CodeNodeP code2 = translateArgs(arg_node->ptr.pExp2,arg_list);
            return mergeCode(code1,code2);
        }
    }
    case FUNCTION_FIRE_NODE:
    case UMINUS_NODE:
    case INC_PREFIX_NODE:
    case DEC_PREFIX_NODE:
    case INC_SUFFIX_NODE:
    case DEC_SUFFIX_NODE:
    case INTEGER_NODE:
    case ID_NODE:
    case PLUS_NODE:
    case MINUS_NODE:
    case STAR_NODE:
    case DIV_NODE:
    {
        char *t1 = newTemp();
        CodeNodeP code1 = translateExp(arg_node,t1);
        insertStrIntoArray(t1 , arg_list);
        return code1;
    }
    }
    return NULL;
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
        char *t1 = newTemp();
        CodeNodeP p1, p2;
        p1 = translateExp(exp->ptr.pExp2, t1);
        p2 = getCode(OP_ASSIGN, t1, NULL, alias);
        tempCode = mergeCode(p1, p2);
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
            log("parse plus left\n");
            first = translateExp(exp->ptr.pExp1, t1);
            log("parse plus right\n");
            next = translateExp(exp->ptr.pExp2, t2);
        }
        tempCode = mergeCode(first, next);
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
        tempCode = mergeCode(first, next);
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
        tempCode = mergeCode(first, next);
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
        tempCode = mergeCode(first, next);
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
    case FUNCTION_FIRE_NODE:
    {
        log("parse function fire\n");
        char function[33];
        strcpy(function, exp->fire.fire_id);
        if(exp->fire.valueList==NULL) {
            if(strcmp(function,"read")==0) {
                tempCode = getCode(OP_READ,NULL,NULL,place);
            } else {
                tempCode = getCode(OP_CALL, function, NULL, place);
            }
        } else {
            char **arg_list=(char**)malloc(sizeof(char*)*10);
            for(int i = 0; i < 10; i++) {
                arg_list[i] = NULL;
            }
            CodeNodeP code1 = translateArgs(exp->fire.valueList,arg_list);
            if(strcmp(function,"write")==0) {
                tempCode = mergeCode(code1,getCode(OP_WRITE,NULL,NULL,arg_list[0]));
            } else {
                CodeNodeP code2 = NULL;
                int index=0;
                while(arg_list[index] != NULL) {
                    code2 = mergeCode(code2, getCode(OP_ARG,NULL,NULL,arg_list[index]));
                    index++;
                }
                tempCode = mergeCode(code1, code2);
                tempCode = mergeCode(tempCode, getCode(OP_CALL,function,NULL,place));
            }
        }
        break;
    }

    }
    return tempCode;
}


CodeNodeP translateCond(PEXP exp, char *label_true, char *label_false)
{
    CodeNodeP tempCode;
    switch (exp->kind)
    {
    case CONDITION_LIST_NODE:
    {
        switch (exp->condition_list.type)
        {
        case LINK_AND:
        {
            char *label1 = newLabel();
            CodeNodeP code1 = translateCond(exp->condition_list.pExp1,label1,label_false);
            CodeNodeP code2 = translateCond(exp->condition_list.pExp2,label_true,label_false);
            CodeNodeP labelCode = getCode(OP_LABEL,NULL,NULL,label1);
            tempCode = mergeCode(code1,labelCode);
            tempCode = mergeCode(tempCode,code2);
            break;
        }
        case LINK_OR:
        {
            char *label1 = newLabel();
            CodeNodeP code1 = translateCond(exp->condition_list.pExp1,label_true,label1);
            CodeNodeP code2 = translateCond(exp->condition_list.pExp2,label_true,label_false);            
            CodeNodeP labelCode = getCode(OP_LABEL,NULL,NULL,label1);
            tempCode = mergeCode(code1,labelCode);
            tempCode = mergeCode(tempCode,code2);
            break;
        }
        case LINK_NOT:
        {
            return translateCond(exp, label_false,label_true);
            break;
        }
        }
        break;
    }
    case GREATER_NODE:
    {
        char *t1 = newTemp();
        char *t2 = newTemp();
        CodeNodeP code1 = translateExp(exp->ptr.pExp1,t1);
        CodeNodeP code2 = translateExp(exp->ptr.pExp2,t2);
        CodeNodeP code3 = getCode(OP_GREATER, t1, t2, label_true);
        CodeNodeP code4 = getCode(OP_GOTO, NULL, NULL, label_false);
        tempCode = mergeCode(code1, code2);
        tempCode = mergeCode(tempCode, code3);
        tempCode = mergeCode(tempCode, code4);
        break;
    }
    case LESS_NODE:
    {
        char *t1 = newTemp();
        char *t2 = newTemp();
        CodeNodeP code1 = translateExp(exp->ptr.pExp1,t1);
        CodeNodeP code2 = translateExp(exp->ptr.pExp2,t2);
        CodeNodeP code3 = getCode(OP_LESS, t1, t2, label_true);
        CodeNodeP code4 = getCode(OP_GOTO, NULL, NULL, label_false);
        tempCode = mergeCode(code1, code2);
        tempCode = mergeCode(tempCode, code3);
        tempCode = mergeCode(tempCode, code4);
        break;
    }
    case EQUAL_NODE:
    {
        char *t1 = newTemp();
        char *t2 = newTemp();
        CodeNodeP code1 = translateExp(exp->ptr.pExp1,t1);
        CodeNodeP code2 = translateExp(exp->ptr.pExp2,t2);
        CodeNodeP code3 = getCode(OP_EQUAL, t1, t2, label_true);
        CodeNodeP code4 = getCode(OP_GOTO, NULL, NULL, label_false);
        tempCode = mergeCode(code1, code2);
        tempCode = mergeCode(tempCode, code3);
        tempCode = mergeCode(tempCode, code4);
        break;
    }
    case GREATER_EQUAL_NODE:
    {
        char *t1 = newTemp();
        char *t2 = newTemp();
        CodeNodeP code1 = translateExp(exp->ptr.pExp1,t1);
        CodeNodeP code2 = translateExp(exp->ptr.pExp2,t2);
        CodeNodeP code3 = getCode(OP_GE, t1, t2, label_true);
        CodeNodeP code4 = getCode(OP_GOTO, NULL, NULL, label_false);
        tempCode = mergeCode(code1, code2);
        tempCode = mergeCode(tempCode, code3);
        tempCode = mergeCode(tempCode, code4);
        break;
    }
    case LESS_EQUAL_NODE:
    {
        char *t1 = newTemp();
        char *t2 = newTemp();
        CodeNodeP code1 = translateExp(exp->ptr.pExp1,t1);
        CodeNodeP code2 = translateExp(exp->ptr.pExp2,t2);
        CodeNodeP code3 = getCode(OP_LE, t1, t2, label_true);
        CodeNodeP code4 = getCode(OP_GOTO, NULL, NULL, label_false);
        tempCode = mergeCode(code1, code2);
        tempCode = mergeCode(tempCode, code3);
        tempCode = mergeCode(tempCode, code4);
        break;
    }
    }
    return tempCode;
}

CodeNodeP translateStmt(PEXP exp)
{
    log("start translate stmt\n");
    CodeNodeP tempCode = NULL;
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
    case FUNCTION_FIRE_NODE:
        tempCode = translateExp(exp, NULL);
        addToList(tempCode);
        break;
    case RETURN_NODE:
    {
        switch (exp->return_exp.returnType)
        {
        case RETURN_VOID:
            tempCode = getCode(OP_RETURN, NULL, NULL, NULL);
            break;
        case RETURN_EXP:
        {
            char *t1 = newTemp();
            CodeNodeP code1 = translateExp(exp->return_exp.pExp, t1);
            CodeNodeP code2 = getCode(OP_RETURN, NULL, NULL, t1);
            tempCode = mergeCode(code1, code2);
            break;
        }
        }
        addToList(tempCode);
        break;
    }
    case IF_NODE:
        nodeToParse[nestCodeBlock] = exp;
        break;
    case ELSE_NODE:
        nodeToParse[nestCodeBlock] = exp;
        break;
    case WHILE_NODE:
        nodeToParse[nestCodeBlock] = exp;
        break;
    case FOR_NODE:
        nodeToParse[nestCodeBlock] = exp;
        break;
    case FUNCTION_DECLARE_NODE:
        nodeToParse[nestCodeBlock] = exp;
        break;
    case INT_NODE:
    {
        PEXP sub_declare = exp->ptr.pExp1;
        while(sub_declare!=NULL) {
            if((sub_declare->ptr.pExp1)->id.dimension != 0) {
                CodeNodeP dec_code = _translateArrayDeclare(INT_NODE,sub_declare->ptr.pExp1);
                tempCode = mergeCode(tempCode , dec_code);
            }
            sub_declare = sub_declare->ptr.pExp2;
        }
        addToList(tempCode);
        break;
    }
    case CHAR_NODE:
    {
        PEXP sub_declare = exp->ptr.pExp1;
        while(sub_declare!=NULL) {
            if((sub_declare->ptr.pExp1)->id.dimension != 0) {
                tempCode = mergeCode(tempCode , _translateArrayDeclare(CHAR_NODE,sub_declare->ptr.pExp1));
            }
            sub_declare = sub_declare->ptr.pExp2;
        }
        addToList(tempCode);
        break;
    }
    case FLOAT_NODE:
    {
        PEXP sub_declare = exp->ptr.pExp1;
        while(sub_declare!=NULL) {
            if((sub_declare->ptr.pExp1)->id.dimension != 0) {
                tempCode = mergeCode(tempCode , _translateArrayDeclare(FLOAT_NODE,sub_declare->ptr.pExp1));
            }
            sub_declare = sub_declare->ptr.pExp2;
        }
        addToList(tempCode);
        break;
    }
    }

    return tempCode;
}

CodeNodeP _translateArrayDeclare(enum node_kind kind , PEXP array_node)
{
    log("parse declare array\n");
    int size;
    switch (kind)
    {
    case INT_NODE:
        size = 4;
        break;
    case CHAR_NODE:
        size = 2;
        break;
    case FLOAT_NODE:
        size = 8;
        break;
    }
    int index1 = array_node->id.index1;
    int index2 = array_node->id.index2;
    int index3 = array_node->id.index3;
    int whole_size=0;
    switch (array_node->id.dimension)
    {
    case 1:
        whole_size = index1 * size;
        break;
    case 2:
        whole_size = index1 * index2 * size;
        break;
    case 3:
        whole_size = index1 * index2 * index3 * size;
        break;
    }
    char *alias = getVariableAlias(array_node->id.type_id);
    char *size_str = intToStr(whole_size);
    return getCode(OP_DEC, size_str,NULL,alias);
}

void parsePreCode()
{
    log("start parse pre code\n");
    if (nodeToParse[nestCodeBlock] == NULL)
    {
        return;
    }

    PEXP node = nodeToParse[nestCodeBlock];
    CodeNodeP tempCode = NULL;
    switch (node->kind)
    {
    case IF_NODE:
    {
        log("parse if\n");
        char *label1, *label2;
        label1 = newLabel();
        label2 = newLabel();
        CodeNodeP exp = node->ptr.pExp1;
        CodeNodeP code1 = translateCond(exp,label1,label2);
        CodeNodeP code2 = codeList[nestCodeBlock+1];
        CodeNodeP code_label1 = getCode(OP_LABEL,NULL,NULL,label1);
        CodeNodeP code_label2 = getCode(OP_LABEL,NULL,NULL,label2);
        tempCode = mergeCode(code1,code_label1);
        tempCode = mergeCode(tempCode , code2);
        tempCode = mergeCode(tempCode, code_label2);
        addToList(tempCode);
        break;
    }
    case ELSE_NODE:
    {
        log("parse else\n");
        char *label3 = newLabel();
        CodeNodeP code3 = codeList[nestCodeBlock+1];
        CodeNodeP code_label3 = getCode(OP_LABEL,NULL,NULL,label3);
        CodeNodeP goto_code = getCode(OP_GOTO,NULL,NULL,label3);
        CodeNodeP p1 = currentCode[nestCodeBlock]->pre;
        CodeNodeP p2 = currentCode[nestCodeBlock];
        p1->next = goto_code;
        goto_code->pre = p1;
        goto_code->next = p2;
        p2->pre = goto_code;
        currentCode[nestCodeBlock]=p2;
        addToList(code3);
        addToList(code_label3);
        break;
    }
    case FOR_NODE:
    {
        break;
    }
    case WHILE_NODE:
    {
        log("parse while\n");
        char *label1 = newLabel();
        char *label2 = newLabel();
        char *label3 = newLabel();
        CodeNodeP code1 = translateCond(node->ptr.pExp1, label2, label3);
        CodeNodeP code2 = codeList[nestCodeBlock+1];
        CodeNodeP code_label1 = getCode(OP_LABEL,NULL,NULL,label1);
        CodeNodeP code_label2 = getCode(OP_LABEL,NULL,NULL,label2);
        CodeNodeP code_label3 = getCode(OP_LABEL,NULL,NULL,label3);
        CodeNodeP code_goto = getCode(OP_GOTO, NULL,NULL,label1);
        tempCode = mergeCode(code_label1, code1);
        tempCode = mergeCode(tempCode, code_label2);
        tempCode = mergeCode(tempCode, code2);
        tempCode = mergeCode(tempCode, code_goto);
        tempCode = mergeCode(tempCode, code_label3);
        addToList(tempCode);
        break;
    }
    case FUNCTION_DECLARE_NODE:
    {
        log("parse function declare\n");
        PEXP function_node = nodeToParse[nestCodeBlock];
        CodeNodeP code1 = NULL;
        CodeNodeP code2 = codeList[nestCodeBlock+1];
        char *function = function_node->function.function_name;
        CodeNodeP function_code = getCode(OP_FUNCTION, NULL, NULL, function);
        PEXP param_list = function_node->function.pExp;
        while(param_list != NULL) {
            char *param = param_list->function_param.param_name;
            char *alias = getVariableAlias(param);
            CodeNodeP *param_code = getCode(OP_PARAM,NULL,NULL,alias);
            code1 = mergeCode(code1 , param_code);
            param_list = param_list->function_param.pExp;
        }
        tempCode = mergeCode(function_code , code1);
        tempCode = mergeCode(tempCode , code2);
        addToList(tempCode);
        break;
    }
    }
    codeList[nestCodeBlock + 1] = NULL;
}