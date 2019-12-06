int a,b,c
//int a							//唯一性检查：变量重复定义
float m,n
/*								//唯一性检查：函数名重复定义
int fibo()
{
	
}
*/
int fibo(int a) 
{
	int d,e
//	f++							//上下文相关性检查：未定义的变量f
//	a=((b+c)-f)*d+e				//上下文相关性检查：未定义的变量f
//	test()						//上下文相关性检查：未定义的函数test()
//	a()							//上下文相关性检查：试图混淆变量与函数
	if (a==1 || a==2) 
	{
		return 1
	}
	return fibo(a-1)+fibo(a-2)
}
int main()
{
	int m,n,i
	i=1
	while (i<=m)
	{
		n=fibo(i)
		write(n)
		i=i+1
	}
}
