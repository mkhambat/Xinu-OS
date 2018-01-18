#include <xinu.h>
#include <stdio.h>

shellcmd xsh_hello(int nargs,char *args[])
{
	if(nargs==2)
	{
		printf("Hello %s ,Welcome to the world of Xinu!!\n", args[1]);
	}
		

	if(nargs<2)
	{
		printf("Error: Too few arguments\n");
	}

	if(nargs>2)
	{
		printf("Error: Too many arguments\n");
	}

	return 0;
}
