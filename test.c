//测试代码
/*
	测试代码
	待填
*/
int a,b,c
int a
float m,n
int fibo(int a) 
{
	int d,e
	if (a==1 || a==2) 
	{
		return 1
	}
	return fibo(a-1)+fibo(a-2)
}
int main()
{
	int m,n,i
	m=read()
	i=1
	while (i<=m)
	{
		n=fibo(i)
		write(n)
		i=i+1
	}
}
