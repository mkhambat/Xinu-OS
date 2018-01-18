#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <sys/wait.h>
#include <stdlib.h>
int main(int argc, char *argv[])
{
	int fd[2],pid;
	//char path[50];
	char readbuffer[80];
	pipe(fd);
	char *path = argv[1];

	pid = fork();
	//printf("%s",path);


	if(pid==0)
	{
		printf("Child ID = %d",getpid());
		printf("\n");
		close(fd[1]);
		read(fd[0],readbuffer,sizeof(readbuffer));
		//printf("%d",nbytes);
		//execl(readbuffer,"",0,NULL);
		
		//if(strcmp("/bin/echo",path)==0)
		if(strcmp(argv[1],"/bin/echo")==0)
		{
			execl(readbuffer,"","Hello World!",NULL);
		}
		else
		{
			execl(readbuffer,"",0,NULL);
		}
		
		printf("\nReceived string: %s ", readbuffer);
	}
	if(pid>0)
	{
		printf("Parent ID = %d ",getpid());
		printf("\n");
		close(fd[0]);
		write(fd[1],path, (strlen(path)+1));
		wait(NULL);
		exit(0);
	}
	
	if(pid==-1)
	{
		perror("fork");
		exit(1);
	}		

	
	return 0;
}
