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
    int a[10],i,b,c
    //求数组中最大值
    for(i = 0; i < 10; i++)
    {
        a[i]=i
    }
    b=a[5]
    for(i=0; i<10; i++)
    {
        if(a[i] > b)
        {
            b=a[i]
        }
    }
    write(b)  
    
    //测试函数调用
    c = test(b) 
    write(c)
    return 0
}


