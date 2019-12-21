#include <stdio.h>
#include <stdlib.h>

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

void insertStrIntoArray(char *a, char **array)
{
    if(array[0] == NULL) {
        array[0] = a;
    }   else {
        int index=0;
        while(array[index] != NULL) {
            index++;
        }
        while(index > 0) {
            array[index] = array[index-1];
            index--;
        }
        array[0]=a;
    }
}

void log(char msg[]) {
    if(msg == NULL) return;
    char log_enable = 1;
    if(log_enable) {
        printf("%s\n", msg);
    }
}

void showSyntaxError() {
    printf("Syntax error Process terminates\n");
    getchar();
    exit(1);
}