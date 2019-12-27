#include <stdlib.h>
#include <string.h>
#include <stdio.h>
enum VARIABLE_TYPE {NORMAL_VAIRABLE , ARRAY_VARIABLE};
typedef struct OffsetContainer{
    enum VARIABLE_TYPE type;
    int offset;
} *OffsetPointer;

int _getVoffset(char *name);
int _getToffset(char *name);
//extern void log(char*);
int getStackSize();
void updateAllOffset(int delta);

OffsetPointer *v = NULL;
OffsetPointer *t = NULL;

extern int inMain;
// int inMain = 0;

int v_num=0;
int t_num=0;
int all_array_size=0;

int getStackSize()
{
    return (v_num+t_num+1)*4 + all_array_size;
}

void init(int v_number, int t_number, int array_num, char **array_v, int *array_size)
{
    v_num = v_number;
    t_num = t_number;
    v = (OffsetPointer*)malloc(sizeof(OffsetPointer) * (v_num + 1));
    t = (OffsetPointer*)malloc(sizeof(OffsetPointer) * (t_num + 1));
    for(int i = 0; i <= v_num; i++) {
        v[i] = NULL;
    }
    for(int i = 0; i <= t_num; i++) {
        t[i] = NULL;
    }

    //数组
    for(int i = 0; i < array_num; i++) {
        int num = 0;
        int len = strlen(array_v[i]);
        for(int j = 1 ;j < len; j++) {
            num = num * 10 + (array_v[i][j] - '0');
        }
        updateAllOffset(array_size[i]);
        v[num] = (OffsetPointer)malloc(sizeof(struct OffsetContainer));
        v[num]->offset=0;

        all_array_size += array_size[i];
    }

    for(int i = 1; i <= v_num; i++) {
        if(v[i] != NULL) continue;

        updateAllOffset(4);
        v[i] = (OffsetPointer)malloc(sizeof(struct OffsetContainer));
        v[i]->offset=0;
    }
    for(int i = 1; i <= t_num; i++) {
        updateAllOffset(4);
        t[i] = (OffsetPointer)malloc(sizeof(struct OffsetContainer));
        t[i]->offset=0;
    }
}

void updateAllOffset(int delta)
{
    for(int i = 1; i <= v_num; i++) {
        if(v[i] != NULL) {
            v[i]->offset += delta;
        }
    }
    for(int i = 1; i <= t_num; i++) {
        if(t[i] != NULL) {
            t[i]->offset += delta;
        }
    }
}

int getOffset(char *name)
{
    if(name == NULL || (name[0] != 'v' && name[0] != 't')) {
        printf("error in function getOffset, name: %s\n",name);
        getchar();
        exit(1);
    }
    int delta = 0;
    if(!inMain) {
        delta = 4;
    } 
    if(name[0] == 'v') return _getVoffset(name) + delta;
    if(name[0] == 't') return _getToffset(name) + delta;
}

//0 - not exist , 1 - exist
int isInStack(char *name)
{
    if(name == NULL || (name[0] != 'v' && name[0] != 't')) {
        printf("error in function isInStack, name: %s\n",name);
        getchar();
        exit(1);
    }
    int len = strlen(name);
    int num = 0;
    for(int i = 1 ;i < len; i++) {
        num = num * 10 + (name[i] - '0');
    }
    if(name[0] == 'v' && v[num] != NULL) return 1;
    if(name[0] == 't' && t[num] != NULL) return 1;
    return 0;
}

void popStack()
{
    updateAllOffset(-4);
}

void pushToStack(char *name)
{
    if(name == NULL || (name[0] != 'v' && name[0] != 't')) {
        printf("error in function pushToStack, name: %s\n",name);
        getchar();
        exit(1);
    }
    int len = strlen(name);
    int num = 0;
    for(int i = 1 ;i < len; i++) {
        num = num * 10 + (name[i] - '0');
    }
    
    if(name[0] == 'v') {
        if(num > v_num) {
            printf("v index out of num");
            getchar();
            exit(1);
        }

        updateAllOffset(4);
        v[num] = (OffsetPointer)malloc(sizeof(struct OffsetContainer));
        v[num]->offset=0;
    } else if(name[0] == 't') {
        if(num > t_num) {
            printf("t index out of num");
            getchar();
            exit(1);
        }

        updateAllOffset(4);
        t[num] = (OffsetPointer)malloc(sizeof(struct OffsetContainer));
        t[num]->offset=0;
    }
    //log("pushed");
}

//@return -1 for error
int _getVoffset(char *name)
{
    int len = strlen(name);
    int num = 0;
    for(int i = 1 ;i < len; i++) {
        num = num * 10 + (name[i] - '0');
    }
    if(num > v_num) {
        printf("variable index out of num\n");
        getchar();
        exit(1);
    }
    if(v[num] != NULL) {
        return v[num]->offset;
    } else {
        printf("v is null\n");
        getchar();
        exit(1);
    }
}

//@return -1 for error
int _getToffset(char *name)
{
    int len = strlen(name);
    int num = 0;
    for(int i = 1 ;i < len; i++) {
        num = num * 10 + (name[i] - '0');
    }
    if(num > t_num) {
        printf("temp index out of num, index:%d, t_num:%d\n", num, t_num);
        getchar();
        exit(1);
    }
    if(t[num] != NULL) {
        return t[num]->offset;
    } else {
        printf("t is null\n");
        getchar();
        exit(1);
    }
}

// int main()
// {
//     char *arrays[3] = {"v1", "v2","v3"};
//     int size[3] = {40,16,80};
//     init(4,4,3,arrays,size);
//     printf("%d\n", getOffset("v1"));
//     printf("%d\n", getOffset("v2"));
//     printf("%d\n", getOffset("v3"));
//     printf("%d\n", getOffset("v4"));
//     printf("%d\n", getOffset("t1"));
//     printf("%d\n", getOffset("t2"));
//     printf("%d\n", getOffset("t3"));
//     printf("%d\n", getOffset("t4"));
// }







