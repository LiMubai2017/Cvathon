int test(int a)
{
    int i,ans
    ans = 0
    for(i=1;i<=a;i++)
    {
        ans = ans + i
    }
    return ans
}

int main()
{
    int a,b
    a=5
    b=10
    a=test(b)
    write(a)
    write(b++)
    write(b)
    write(++b)
    return 0
}
