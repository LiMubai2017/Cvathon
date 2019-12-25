#include <stdio.h>
#include "Node.h"
#include "offsetcontainer.c"

extern int getOffset(char *name);
extern void log(char*);
extern char *intToStr(int);
extern int isInStack(char *name);
extern void pushToStack(char *name);
extern void popStack();
extern int getStackSize();
 

char read[] = {"read:\n  li $v0, 4\n  la $a0, _prompt \n  syscall\n  li $v0, 5\n  syscall\n  jr $ra\n"};
char write[] = {"write:\n  li $v0, 1\n  syscall\n  li $v0, 4\n  la $a0, _ret\n  syscall\n  move $v0, $0\n  jr $ra\n"};
char prompt[] = {"_prompt: .asciiz \"Enter an integer\"\n"};
char ret[] = {"_ret: .asciiz \"\\n\"\n"};
char data[] = {".data\n"};
char global_main[] = {".globl main\n"};
char text[] = {".text\n"};

enum MIPS32_OP {MIPS_LI, MIPS_SW, MIPS_LW, MIPS_MOVE, MIPS_ADD, MIPS_SUB, MIPS_MUL, MIPS_DIV, MIPS_JR, MIPS_J,MIPS_JAL,
                MIPS_MFLO,  MIPS_BEQ, MIPS_BNE, MIPS_BGT, MIPS_BGE, MIPS_BLE, MIPS_BLT, MIPS_LABEL, MIPS_PUSH, MIPS_POP};

typedef struct ObjCode
{
    char *code;
    struct ObjCode *next;
} *ObjCodeP;


ObjCodeP getMIPScode(enum MIPS32_OP op, char*, char*, char*);
ObjCodeP getObjCode(char*);
void addToCodes(ObjCodeP );
ObjCodeP mergeObjCode(ObjCodeP, ObjCodeP );

ObjCodeP objCodes = NULL;
ObjCodeP current = NULL;


ObjCodeP getObjCode(char *code)
{
    ObjCodeP objCode = (ObjCodeP)malloc(sizeof(struct ObjCode));
    objCode->code = code;
    objCode->next = NULL;
}

void addToCodes(ObjCodeP code)
{
    if(code == NULL) {
        return;
    }
    if(objCodes == NULL) {
        current = objCodes = code;
    } else {
        current->next = code;
    }
    while(current->next != NULL) {
        current = current->next;
    }
    return;
}

ObjCodeP mergeObjCode(ObjCodeP code1, ObjCodeP code2)
{
    if(code1 == NULL) {
        return code2;
    }
    if(code2 == NULL) {
        return code1;
    }
    ObjCodeP current = code1;
    while(current->next != NULL) {
        current = current->next;
    }
    current->next = code2;
    return code1;
}

void printObjCode()
{
    FILE *fp;
    if ((fp = fopen("test.asm", "w")) == NULL)
    {
        log("file cannot open \n");
        exit(1);
    }
    else
    {
        log("file opened for writing \n");
    }

    fprintf(fp, "%s", data);
    fprintf(fp, "%s", prompt);
    fprintf(fp, "%s", ret);
    fprintf(fp, "%s", global_main);
    fprintf(fp, "%s", text);
    fprintf(fp, "%s", read);
    fprintf(fp, "%s", write);

    if(objCodes == NULL) {
        printf("objCodes is null\n");
        getchar();
        return;
    }
    ObjCodeP code = objCodes;
    while(code != NULL) {
        fprintf(fp, "%s\n", code->code);
        code = code->next;
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
}

// 1-num , 0-not num
int isStrNum(char *str)
{
    if(str[0] == '#') {
        return 1;
    } else {
        return 0;
    }
}

char* deleteNumFlag(char *num)
{
    int len = strlen(num);
    char *temp = (char*)malloc(sizeof(char)*len);
    for(int i = 0; i < len; i++) {
        temp[i] = num[i+1];
    }
    return temp;
}

void *translateInterCode(CodeNodeP codeNode)
{
    char *opn1 = codeNode->opn1;
    char *opn2 = codeNode->opn2;
    char *result = codeNode->result;
    ObjCodeP code1=NULL, code2=NULL, code3=NULL, code4=NULL, tempCode=NULL;
    switch (codeNode->op)
    {
    case OP_ASSIGN:
    {
        log("OBJ translate assign");
        if(!isInStack(result)) {
            log("not in stack");
            pushToStack(result);
            tempCode = getMIPScode(MIPS_PUSH,NULL,NULL,NULL);
        }

        if(isStrNum(opn1)) {
            code1 = getMIPScode(MIPS_LI,"$t3",deleteNumFlag(opn1),NULL);
            code2 = getMIPScode(MIPS_SW,"$t3",intToStr(getOffset(result)),NULL);
            tempCode = mergeObjCode(tempCode, code1);
            tempCode = mergeObjCode(tempCode, code2);
        } else {
            code1 = getMIPScode(MIPS_LW,"$t1",intToStr(getOffset(opn1)),NULL);
            code2 = getMIPScode(MIPS_MOVE, "$t3", "$t1", NULL);
            code3 = getMIPScode(MIPS_SW, "$t3", intToStr(getOffset(result)),NULL);
            tempCode = mergeObjCode(tempCode, code1);
            tempCode = mergeObjCode(tempCode, code2);
            tempCode = mergeObjCode(tempCode, code3);
        }
        break;
    }
    case OP_PLUS:
    {
        log("OBJ translate plus");
        if(!isInStack(result)) {
            log("not in stack");
            pushToStack(result);
            tempCode = getMIPScode(MIPS_PUSH,NULL,NULL,NULL);
        }

        code1 = getMIPScode(MIPS_LW, "$t1",intToStr(getOffset(opn1)),NULL);
        code2 = getMIPScode(MIPS_LW, "$t2",intToStr(getOffset(opn2)),NULL);
        code3 = getMIPScode(MIPS_ADD, "$t3","$t1","$t2");
        code4 = getMIPScode(MIPS_SW, "$t3", intToStr(getOffset(result)),NULL);
        tempCode = mergeObjCode(tempCode, code1);
        tempCode = mergeObjCode(tempCode, code2);
        tempCode = mergeObjCode(tempCode, code3);
        tempCode = mergeObjCode(tempCode, code4);
        break;
    }
    case OP_MINUS:
    {
        log("OBJ translate minus");
        if(!isInStack(result)) {
            log("not in stack");
            pushToStack(result);
            tempCode = getMIPScode(MIPS_PUSH,NULL,NULL,NULL);
        }

        code1 = getMIPScode(MIPS_LW, "$t1",intToStr(getOffset(opn1)),NULL);
        code2 = getMIPScode(MIPS_LW, "$t2",intToStr(getOffset(opn2)),NULL);
        code3 = getMIPScode(MIPS_SUB, "$t3","$t1","$t2");
        code4 = getMIPScode(MIPS_SW, "$t3", intToStr(getOffset(result)),NULL);
        tempCode = mergeObjCode(tempCode, code1);
        tempCode = mergeObjCode(tempCode, code2);
        tempCode = mergeObjCode(tempCode, code3);
        tempCode = mergeObjCode(tempCode, code4);
        break;
    }
    case OP_STAR:
    {
        log("OBJ translate star");
        if(!isInStack(result)) {
            log("not in stack");
            pushToStack(result);
            tempCode = getMIPScode(MIPS_PUSH,NULL,NULL,NULL);
        }

        code1 = getMIPScode(MIPS_LW, "$t1",intToStr(getOffset(opn1)),NULL);
        code2 = getMIPScode(MIPS_LW, "$t2",intToStr(getOffset(opn2)),NULL);
        code3 = getMIPScode(MIPS_MUL, "$t3","$t1","$t2");
        code4 = getMIPScode(MIPS_SW, "$t3", intToStr(getOffset(result)),NULL);
        tempCode = mergeObjCode(tempCode, code1);
        tempCode = mergeObjCode(tempCode, code2);
        tempCode = mergeObjCode(tempCode, code3);
        tempCode = mergeObjCode(tempCode, code4);
        break;
    }
    case OP_DIV:
    {
        log("OBJ translate div");
        if(!isInStack(result)) {
            log("not in stack");
            pushToStack(result);
            tempCode = getMIPScode(MIPS_PUSH,NULL,NULL,NULL);
        }

        code1 = getMIPScode(MIPS_LW, "$t1",intToStr(getOffset(opn1)),NULL);
        code2 = getMIPScode(MIPS_LW, "$t2",intToStr(getOffset(opn2)),NULL);
        code3 = getMIPScode(MIPS_MUL, "$t3","$t1","$t2");
        code4 = getMIPScode(MIPS_DIV, "$t1", "$t2",NULL);
        ObjCodeP code5 = getMIPScode(MIPS_MFLO, "$t3", NULL, NULL);
        ObjCodeP code6 = getMIPScode(MIPS_SW, "$t3", intToStr(getOffset(result)),NULL);
        tempCode = mergeObjCode(tempCode, code1);
        tempCode = mergeObjCode(tempCode, code2);
        tempCode = mergeObjCode(tempCode, code3);
        tempCode = mergeObjCode(tempCode, code4);
        tempCode = mergeObjCode(tempCode, code5);
        tempCode = mergeObjCode(tempCode, code6);
        break;
    }
    case OP_RETURN:
    {
        log("OBJ translate return");
        code1 = getMIPScode(MIPS_LW,"$t1",intToStr(getOffset(result)),NULL);
        code2 = getMIPScode(MIPS_MOVE,"$v0","$t1",NULL);
        code3 = getMIPScode(MIPS_JR,"$ra",NULL,NULL);
        tempCode = mergeObjCode(tempCode, code1);
        tempCode = mergeObjCode(tempCode, code2);
        tempCode = mergeObjCode(tempCode, code3);
        break;
    }
    case OP_EQUAL:
    {
        log("OBJ translate equal");
        code1 = getMIPScode(MIPS_LW, "$t1", intToStr(getOffset(opn1)),NULL);
        code2 = getMIPScode(MIPS_LW, "$t2", intToStr(getOffset(opn2)),NULL);
        code3 = getMIPScode(MIPS_BEQ, "$t1", "$t2", result);
        tempCode = mergeObjCode(tempCode, code1);
        tempCode = mergeObjCode(tempCode, code2);
        tempCode = mergeObjCode(tempCode, code3);
        break;
    }
    case OP_UE:
    {
        log("OBJ translate ue");
        code1 = getMIPScode(MIPS_LW, "$t1", intToStr(getOffset(opn1)),NULL);
        code2 = getMIPScode(MIPS_LW, "$t2", intToStr(getOffset(opn2)),NULL);
        code3 = getMIPScode(MIPS_BNE, "$t1", "$t2", result);
        tempCode = mergeObjCode(tempCode, code1);
        tempCode = mergeObjCode(tempCode, code2);
        tempCode = mergeObjCode(tempCode, code3);
        break;
    }
    case OP_GREATER:
    {
        code1 = getMIPScode(MIPS_LW, "$t1", intToStr(getOffset(opn1)),NULL);
        code2 = getMIPScode(MIPS_LW, "$t2", intToStr(getOffset(opn2)),NULL);
        code3 = getMIPScode(MIPS_BGT, "$t1", "$t2", result);
        tempCode = mergeObjCode(tempCode, code1);
        tempCode = mergeObjCode(tempCode, code2);
        tempCode = mergeObjCode(tempCode, code3);
        break;
    }
    case OP_GE:
    {
        code1 = getMIPScode(MIPS_LW, "$t1", intToStr(getOffset(opn1)),NULL);
        code2 = getMIPScode(MIPS_LW, "$t2", intToStr(getOffset(opn2)),NULL);
        code3 = getMIPScode(MIPS_BGE, "$t1", "$t2", result);
        tempCode = mergeObjCode(tempCode, code1);
        tempCode = mergeObjCode(tempCode, code2);
        tempCode = mergeObjCode(tempCode, code3);
        break;
    }
    case OP_LESS:
    {
        code1 = getMIPScode(MIPS_LW, "$t1", intToStr(getOffset(opn1)),NULL);
        code2 = getMIPScode(MIPS_LW, "$t2", intToStr(getOffset(opn2)),NULL);
        code3 = getMIPScode(MIPS_BLT, "$t1", "$t2", result);
        tempCode = mergeObjCode(tempCode, code1);
        tempCode = mergeObjCode(tempCode, code2);
        tempCode = mergeObjCode(tempCode, code3);
        break;
    }
    case OP_LE:
    {
        code1 = getMIPScode(MIPS_LW, "$t1", intToStr(getOffset(opn1)),NULL);
        code2 = getMIPScode(MIPS_LW, "$t2", intToStr(getOffset(opn2)),NULL);
        code3 = getMIPScode(MIPS_BLE, "$t1", "$t2", result);
        tempCode = mergeObjCode(tempCode, code1);
        tempCode = mergeObjCode(tempCode, code2);
        tempCode = mergeObjCode(tempCode, code3);
        break;
    }
    case OP_LABEL:
    case OP_FUNCTION:
    {
        tempCode = getMIPScode(MIPS_LABEL, result, NULL, NULL);
        if(strcmp(result , "main") == 0) {
            char *code = (char*)malloc(sizeof(char)*20);
            strcpy(code, "addi $sp, $sp, -");
            char *size =  intToStr(getStackSize());
            strcat(code, size);
            tempCode = mergeObjCode(tempCode , getObjCode(code));   
        }
        break;
    }
    case OP_READ:
    {
        log("OBJ translate read");
        code1 = getMIPScode(MIPS_PUSH,NULL,NULL,NULL);
        updateAllOffset(4);

        code2 = getMIPScode(MIPS_SW,"$ra","0",NULL);
        code3 = getObjCode("jal read");
        code4 = getMIPScode(MIPS_LW,"$ra","0",NULL);

        ObjCodeP code5 = getMIPScode(MIPS_POP,NULL,NULL,NULL);
        updateAllOffset(-4);


        tempCode = mergeObjCode(tempCode , code1);
        tempCode = mergeObjCode(tempCode , code2);
        tempCode = mergeObjCode(tempCode , code3);
        tempCode = mergeObjCode(tempCode , code4);
        tempCode = mergeObjCode(tempCode , code5);

        if(!isInStack(result)) {
            log("not in stack");
            pushToStack(result);
            tempCode = mergeObjCode(tempCode , getMIPScode(MIPS_PUSH,NULL,NULL,NULL));
        }
        ObjCodeP code6 = getMIPScode(MIPS_SW,"$v0",intToStr(getOffset(result)),NULL);
        tempCode = mergeObjCode(tempCode , code6);
        break;
    }
    case OP_WRITE:
    {
        log("OBJ translate write");
        code1 = getMIPScode(MIPS_LW, "$t2", intToStr(getOffset(result)),NULL);
        code2 = getMIPScode(MIPS_MOVE, "$a0", "$t2", NULL);
        code3 = getMIPScode(MIPS_PUSH,NULL,NULL,NULL);
        updateAllOffset(4);

        code4 = getMIPScode(MIPS_SW,"$ra","0",NULL);
        ObjCodeP code5 = getObjCode("jal write");
        ObjCodeP code6 = getMIPScode(MIPS_LW,"$ra","0",NULL);

        ObjCodeP code7 = getMIPScode(MIPS_POP,NULL,NULL,NULL);
        updateAllOffset(-4);

        tempCode = mergeObjCode(tempCode, code1);
        tempCode = mergeObjCode(tempCode, code2);
        tempCode = mergeObjCode(tempCode, code3);
        tempCode = mergeObjCode(tempCode, code4);
        tempCode = mergeObjCode(tempCode, code5);
        tempCode = mergeObjCode(tempCode, code6);
        tempCode = mergeObjCode(tempCode, code7);
        break;
    }
    case OP_GOTO:
    {
        log("translate goto");
        tempCode = getMIPScode(MIPS_J, result, NULL, NULL);
    }
    default:
        break;
    }
    addToCodes(tempCode);
}

ObjCodeP getMIPScode(enum MIPS32_OP op, char *param1, char *param2, char *param3)
{
    char *code = (char*)malloc(sizeof(char)*20);
    switch (op)
    {
    case MIPS_LI:
    {
        strcpy(code,"li ");
        strcat(code,param1);
        strcat(code,", ");
        strcat(code,param2);
        break;
    }
    case MIPS_LW:
    {
        strcpy(code,"lw ");
        strcat(code,param1);
        strcat(code,", ");
        strcat(code,param2);
        strcat(code,"($sp)");
        break;
    }
    case MIPS_SW:
    {
        strcpy(code,"sw ");
        strcat(code,param1);
        strcat(code,", ");
        strcat(code,param2);
        strcat(code,"($sp)");
        break;
    }
    case MIPS_MOVE:
    {
        strcpy(code,"move ");
        strcat(code,param1);
        strcat(code,", ");
        strcat(code,param2);
        break;
    }
    case MIPS_ADD:
    {
        strcpy(code,"add ");
        strcat(code,param1);
        strcat(code,", ");
        strcat(code,param2);
        strcat(code,", ");
        strcat(code,param3);
        break;
    }
    case MIPS_SUB:
    {
        strcpy(code,"sub ");
        strcat(code,param1);
        strcat(code,", ");
        strcat(code,param2);
        strcat(code,", ");
        strcat(code,param3);
        break;
    }
    case MIPS_MUL:
    {
        strcpy(code,"mul ");
        strcat(code,param1);
        strcat(code,", ");
        strcat(code,param2);
        strcat(code,", ");
        strcat(code,param3);
        break;
    }
    case MIPS_DIV:
    {
        strcpy(code,"div ");
        strcat(code,param1);
        strcat(code,", ");
        strcat(code,param2);
        break;
    }
    case MIPS_MFLO:
    {
        strcpy(code,"mflo ");
        strcat(code,param1);
        break;   
    }
    case MIPS_J:
    {
        strcpy(code, "j ");
        strcat(code, param1);
        break;
    }
    case MIPS_JR:
    {
        strcpy(code,"jr ");
        strcat(code,param1);
        break;
    }
    case MIPS_JAL:
    {
        strcpy(code, "jal ");
        strcat(code, param1);
        break;
    }
    case MIPS_BEQ:
    {
        strcpy(code,"beq ");
        strcat(code,param1);
        strcat(code,", ");
        strcat(code,param2);
        strcat(code,", ");
        strcat(code,param3);
        break;
    }
    case MIPS_BNE:
    {
        strcpy(code,"bne ");
        strcat(code,param1);
        strcat(code,", ");
        strcat(code,param2);
        strcat(code,", ");
        strcat(code,param3);
        break;
    }
    case MIPS_BGT:
    {
        strcpy(code,"bgt ");
        strcat(code,param1);
        strcat(code,", ");
        strcat(code,param2);
        strcat(code,", ");
        strcat(code,param3);
        break;
    }
    case MIPS_BLT:
    {
        strcpy(code,"blt ");
        strcat(code,param1);
        strcat(code,", ");
        strcat(code,param2);
        strcat(code,", ");
        strcat(code,param3);
        break;
    }
    case MIPS_BLE:
    {
        strcpy(code,"ble ");
        strcat(code,param1);
        strcat(code,", ");
        strcat(code,param2);
        strcat(code,", ");
        strcat(code,param3);
        break;
    }
    case MIPS_LABEL:
    {
        strcpy(code,param1);
        strcat(code,": ");
        break;
    }
    case MIPS_PUSH:
    {
        code = "addi $sp, $sp, -4";
        break;
    }
    case MIPS_POP:
    {
        code = "addi $sp, $sp, 4";
        break;
    }
    }
    return getObjCode(code);
}










