#include<xinu.h>
#include<process_ring.h>


// SEMAPHORE

process sem_producer(int32 i, int32 round, int32 no_of_process, sid32 sem_array[], int32 mailbox[], sid32 done_sem)
{
  int32 last=-1;
  while(last<round-1){
    last = last + 1;
    wait(sem_array[i]);
    printf("Ring Element %d : Round %d : Value : %d\n", i, last,mailbox[i]);
    // sleep(0.5);
    if(no_of_process-1!=i)
    {
      mailbox[i+1] = mailbox[i] - 1;
      signal(sem_array[i+1]);
    }

    if(no_of_process-1 == i)
    {
      mailbox[0] = mailbox[i] - 1;
      signal(sem_array[0]);
    }
  }
  signal(done_sem);
  return OK;
}


// POLLING

process poll_producer(int32 *array[], int32 no_of_process,int32 round,int32 i)
{
  int32 last = -1;
  int32 x=-1;
  int32 tmp=0;
  // int32 tmp=-1;
  while(last<round-1)
  {

    tmp = array[i];
    if(x!=tmp)
    {
    
      last = last + 1;
      
      printf("Ring Element %d : Round %d :  Value: %d\n", i,last,array[i]);
      
      x = array[i];

      if(i!=no_of_process-1)
      { 
        array[i+1]=x-1;
      }
      else
        array[0]=x-1;
    }
  }
  return OK;
}
