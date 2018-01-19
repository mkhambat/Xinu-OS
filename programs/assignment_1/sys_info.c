#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <sys/wait.h>
#include <stdlib.h>
int main(int argc, char *argv[])
{
	int fd[2],pid;
	char readbuffer[80];
	pipe(fd);
	char *path = argv[1];

	if(argc==2)
	{
		pid = fork();	//Creates a new process
		if(pid==0)	//Checks if the process is child
		{	
			printf("Child ID = %d",getpid());
			printf("\n");
			close(fd[1]);
			read(fd[0],readbuffer,sizeof(readbuffer));
		
			if(strcmp(argv[1],"/bin/echo")==0)	//If input given is /bin/echo? Then print Hello World!
			{
				execl(readbuffer,"","Hello World!",NULL);
			}
			else
			{
				execl(readbuffer,"",0,NULL);
			}
		
			//printf("\nReceived string: %s ", readbuffer);
		}
		if(pid>0) 	//If process is a parent
		{
			printf("Parent ID = %d ",getpid());
			printf("\n");
			close(fd[0]);
			write(fd[1],path, (strlen(path)+1));
			wait(NULL);
			exit(0);
		}
	
		if(pid==-1) 	//If an error occured during creation of a new process
		{
			perror("fork");
			exit(1);
		}		
	}
	if(argc>2)
	{
		printf("Error: Too many arguments\n");
	}
	if(argc<2)
	{
		printf("Error: Too few arguments\n");
	}

	
	
		return 0;
	
}
