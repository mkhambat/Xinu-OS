#include <xinu.h>
#include <stdio.h>

shellcmd xsh_hello(int nargs,char *args[])
{
	if(nargs==2)	//Checks the number of arguments given as input
	{
		printf("Hello %s ,Welcome to the world of Xinu!!\n", args[1]);
	}
		

	if(nargs<2) 	//If input arguments less than 2, throw error for too few arguments
	{
		printf("Error: Too few arguments\n");
	}

	if(nargs>2)	//If input arguments more than 2, throw error for too many arguments
	{
		printf("Error: Too many arguments\n");
	}

	return 0;
}
