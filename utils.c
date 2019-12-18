

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

void log(char *msg) {
    char log_enable = 1;
    if(log_enable) {
        printf("%s\n", msg);
    }
}