// #include <string.h>
#include<xinu.h>
#include<stdio.h>
#include<stdlib.h>
#include<process_ring.h>
#include<ctype.h>
/*
 *
 * POLLING
 *
 *
shellcmd xsh_process_ring(int nargs, char *args[]) 
{
	int32 produce = -1;
	volatile int32 consume = -1;
	int i;
	int process=3,rounds=2;
	int mailbox[process];
	//for(i=0;i<args[1];i++)
	//{
		resume(create(poll_producer, 1024, 20, "producer", 2,&produce, &consume));
		resume(create(poll_consumer, 1024, 20, "consumer", 2,&produce, &consume));*/
	/* Polling to see if both producer and consumer are done */
	//}
	//while (consume < 3);
	//return SHELL_OK;
//}


// SEMAPHORES


// shellcmd xsh_process_ring(int nargs, char *args[]) {

// int32 i =0;
// int32 round = atoi(args[2]);
// int32 no_of_process = atoi(args[1]);
// sid32 done_sem = semcreate(0);
// sid32 produce_sem=semcreate(0);
// int32 produce = -1;

// int32 array[no_of_process];
// array[0]=(no_of_process*round)-1;
//   for(i = 0; i < no_of_process; i++)
//   {
//   	resume(create(sem_producer, 1024, 20, "producer", 7, produce_sem, done_sem, &produce, i, round,no_of_process,array));
//   }

//  signal(produce_sem); 
//  wait(done_sem);

//  semdelete(produce_sem);
//  semdelete(done_sem);

//  return SHELL_OK;
// }

/* int32 strlen(char *string){
	int32 i;
	for(i = 0; string[i]! = '\0'; i++);
	return i
}*/


shellcmd xsh_process_ring(int argc, char *args[])
{
	int32 i;
  	int32 p = 2, r = 3; /* specified default */
	char def_impl [] = "poll";
	char *impl;
	impl = def_impl;
	
	for(i = 1; i < argc; i++) {

		if(0 == strncmp("-", args[i],1)) { //If the first character of the argument is a '-'

			if(0 == strcmp("--help",args[i]) || 0==strcmp("-h",args[i])) { //If --help or -h is an argmument
				printf("Usage: \nThe process_ring command uses polling or semaphores to print the process number and corresponding value that process holds.\nOptions:\n -i choose which function to implement.\n \t Arguments: \n\t\t-poll\n \t\t-sync.\n -p: No of process to execute[0-64] \n -r: No of rounds to execute\n\nIf no arguments given, default process = 2, rounds = 3 and implementation = poll\n");
				return SHELL_OK;

			} else if (0 == strncmp("-p", args[i], strlen(args[i]))) { //Argument -p
	      		/* parse numeric argument to -p */
				if (! (i + 1 < argc)) { 
	        		/* No argument after flag */
					// print_usage();
					printf("-p requires a valid integer argument\n");
					return SHELL_ERROR;
				}
				
				int32 len = strlen(args[i+1]);
				char input_arr[len];
				strncpy(input_arr, args[i+1], len); //Copy argument char by char into input_arr
				int32 j;
				for(j= 0;j<len;j++)
				{
					if (0==isdigit(input_arr[j])) {
						// print_usage();
						printf("-p requires a valid integer argument\n");
						return SHELL_ERROR;
					}
				}
				p = atoi(args[i + 1]);

				if(args[i+1] == '\0'){
	       			/* There was trailing garbage in the string that
	          		wasn't converted to an integer. */
					// print_usage();
					printf("-p recieved an invalid argument\n");
					return SHELL_ERROR;
				} else if (!(0 <= p && p <32)) {
	        	/* The number is out of range */
					// print_usage();
					printf("-p requires a number between 0 - 32\n");
					return SHELL_ERROR;
				}
	      		/* Skip pass the numeric argument,
	         	that was successfully parsed */
				i += 1; 

	    	} else if (0 == strncmp("-r", args[i], strlen(args[i]))) { // To check for value -r
	      		/* parse numeric argument to -p */
	    		if (! (i + 1 < argc)) {
	        		/* No argument after flag */
	    			// print_usage();
	    			printf("-r requires a valid integer argument\n");
	    			return SHELL_ERROR;
	    		}


	    		int32 len = strlen(args[i+1]);
				//printf("Len of p is: %d\n", len);
	    		char input_arr[len];
	    		strncpy(input_arr, args[i+1], len);
	    		int32 j;
	    		for(j= 0;j<len;j++)
	    		{
					//printf("input_arr[j] is: %d\n", isdigit(input_arr[j]));
	    			if (0==isdigit(input_arr[j])) {
	    				// print_usage();
	    				printf("-r requires a valid integer argument\n");
	    				return SHELL_ERROR;
	    			}
	    		}
	    		r = atoi(args[i + 1]);	
	    		if(args[i+1] == '\0'){
		       	/* There was trailing garbage in the string that
		          wasn't converted to an integer. */
	    			// print_usage();
	    			printf("-r recieved an invalid argument\n");
	    			return SHELL_ERROR;
	    		} else if (!(0 < r && r <= 100)) {
		        /* The number is out of range */
	    			// print_usage();
	    			printf("-r requires a number between 0 - 100\n");
	    			return SHELL_ERROR;
	    		}
		    	/* Skip pass the numeric argument,
		        that was successfully parsed */

	    		i += 1; 

	    	} else if (0 == strncmp("-i", args[i], strlen(args[i]))) { // To check for value -i
	      		/* parse numeric argument to -p */
	    		if (! (i + 1 < argc)) {
	        		/* No argument after flag */
	    			// print_usage();
	    			printf("-i requires an argument either poll or sync\n");
	    			return SHELL_ERROR;
	    		}


	    		if (0 == strcmp("poll",args[ i + 1])) {
	    			//write the polling code
	    			impl = "poll";	
	    		}
	    		else if (0 == strcmp("sync",args[ i + 1])){
	    			//write the code for sync
	    			impl = "sync";	
	    		}
	    		else{
	    			// print_usage();
	    			printf("-i requires an argument either poll or sync\n");
	    			return SHELL_ERROR;
	    		}

	    		i += 1; 

	    	} else {
	    		// print_usage();
	    		printf("Process ring command accepts --help, -p, -r and -i flags\nCheck help for more details\n");
	    		return SHELL_ERROR; 
	    	}

	    } else {
	    	// printf("Inside else\n");
	    	int32 r_len = strlen(args[i]);
	    	char temp_r [r_len];
	    	strncpy(temp_r, args[i], r_len);
	    	int32 j;
	    	for(j = 0; j < r_len; j++)
	    	{
	    		if (0==isdigit(temp_r[j])){
	    			// print_usage();
	    			printf("Enter a valid integer\n");
	    			return SHELL_ERROR;
	    		}
	    		if(argc>3)
	    		{
	    			printf("Check help to enter the correct number of arguments\n");
	    			return SHELL_ERROR;
	    		}
	    	}

	    	int32 p_len = strlen(args[i+1]);
	    	
	    	char temp_p [p_len];
	    	strncpy(temp_p, args[i+1], p_len);
	    
	    	for(j = 0; j < p_len; j++)
	    	{
	    		if (!isdigit(temp_p[j])){
	    			// print_usage();
	    			printf("-p requires a valid integer argument\n");
	    			return SHELL_ERROR;
	    		}
	    	}

	    	r = atoi(args[i]);
	    	p = atoi(args[i+1]);
	    	i += 1;

	    }
	}
	if(p>32 || r>100)
	    	{
	    		printf("Number of processes should be between 0-32 and rounds should be between 0-100\n");
	    		return SHELL_ERROR;
	    	}
	if(0==strncmp("sync",impl,4)){ //If argument is semaphore

		sid32 sem_array[p];
		sid32 done_sem=semcreate(0);
		sem_array[0] = semcreate(1);
		int i;
		uint32 start_time, end_time;
		int32 mailbox[p]; //Create an array of size = no_of_processes

		mailbox[0]=(p*r)-1;
		gettime(&start_time);
		for(i=1;i<p;i++)
		{
			sem_array[i] = semcreate(0);
		}


		for(i=0;i<p;i++)
		{
			resume(create(sem_producer, 1024, 20, "producer", 6, i, r, p, sem_array, mailbox,done_sem ));
		}

		for(i=0;i<p;i++)
		{
			wait(done_sem);
		}

		for(i=0;i<p;i++)
		{
			semdelete(done_sem);
			semdelete(sem_array[i]);
		}
		gettime(&end_time);
		// sleep(0.5);
		printf("Elapsed Seconds: %d\n", end_time-start_time);

	}

	if(0==strncmp("poll",impl,4)){ //If argument is poll, perform polling

		volatile int32 array[p];
		array[0] = (p*r)-1;
		int i=0;
		uint32 end_time,start_time;
		
		gettime(&start_time);
		for(i=0;i<p;i++){
			resume(create(poll_producer, 1024, 20, "producer",4, &array, p, r,i));		
		}

		while(array[p - 1]>0);
		gettime(&end_time);
		sleep(0.5);
		printf("Elapsed Seconds: %d\n",end_time- start_time);

		return SHELL_OK;
	}
}

