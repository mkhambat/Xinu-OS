#include <xinu.h>
#include <future.h>
uint32 future_test(int nargs, char *args[]);
int ffib(int n);
future **fibfut;

uint future_prod(future* fut,int n) {
  printf("Produced %d\n",n);
  future_set(fut, &n);
  return OK;
}

uint future_cons(future* fut) {
  int i, status;
  status = (int)future_get(fut, &i);
  if (status < 1) {
    printf("future_get failed\n");
    return -1;
  }
  printf("Consumed %d\n", i);
  return OK;
}


shellcmd xsh_run(int argc, char *args[])
{
  future_test(argc,args);

  return SHELL_OK;      
}


uint32 future_test(int nargs, char *args[])
{

  if (nargs == 3 && strncmp(args[2], "-pc", 2) == 0 && strcmp(args[1], "future_test")==0) {

    future* f_exclusive,* f_shared,* f_queue;
 
    f_exclusive = future_alloc(FUTURE_EXCLUSIVE);
    f_shared    = future_alloc(FUTURE_SHARED);
    f_queue     = future_alloc(FUTURE_QUEUE);
   
    // Test FUTURE_EXCLUSIVE
    resume( create(future_cons, 1024, 20, "fcons1", 1, f_exclusive) );
    resume( create(future_prod, 1024, 20, "fprod1", 2, f_exclusive, 1) );

    // Test FUTURE_SHARED
    resume( create(future_cons, 1024, 20, "fcons2", 1, f_shared) );
    resume( create(future_cons, 1024, 20, "fcons3", 1, f_shared) );
    resume( create(future_cons, 1024, 20, "fcons4", 1, f_shared) ); 
    resume( create(future_cons, 1024, 20, "fcons5", 1, f_shared) );
    resume( create(future_prod, 1024, 20, "fprod2", 2, f_shared, 2) );

    // Test FUTURE_QUEUE
    resume( create(future_cons, 1024, 20, "fcons6", 1, f_queue) );
    resume( create(future_cons, 1024, 20, "fcons7", 1, f_queue) );
    resume( create(future_cons, 1024, 20, "fcons8", 1, f_queue) );
    resume( create(future_cons, 1024, 20, "fcons9", 1, f_queue) );
    resume( create(future_prod, 1024, 20, "fprod3", 2, f_queue, 3) );
    resume( create(future_prod, 1024, 20, "fprod4", 2, f_queue, 4) );
    resume( create(future_prod, 1024, 20, "fprod5", 2, f_queue, 5) );
    resume( create(future_prod, 1024, 20, "fprod6", 2, f_queue, 6) );
    sleep(1);
    return OK;

  }

  if (nargs == 4 && strncmp(args[2], "-f", 2 ) == 0 && strcmp(args[1], "future_test")==0) {

    int n,i;
    n = atoi(args[3]);

    int result = 0;
    fibfut[n+1];
    fibfut = (future**)getmem((n+1)*sizeof(future*));

    for(i=0; i<=n;i++)
    {
      fibfut[i] = future_alloc(FUTURE_SHARED);
    }
    for(i=0; i<=n; i++)
    {
      char name = "ffib"+i;
      resume(create(ffib, 1024, 20, name, 1, i));
    }
    printf("before future_get %d\n", result);
    future_get(fibfut[n], &result);
    printf("after future_get result: %d\n", result);

    printf("%d\n",&result );
    printf("Futures Fibonacci for N=%d\n",n);
    printf("Nth Fibonacci value for N=%d is %d\n", n,result);
    return OK;
  }
  else {
    printf("No valid options\n");
    return(OK);
  }    
}  

int ffib(int n) {

  int minus1 = 0;
  int minus2 = 0;
  int this = 0;
  int zero = 0;
  int one = 1;

  if (n == 0) {
    future_set(fibfut[0], &zero);
    return OK;
  }

  if (n == 1) {
    future_set(fibfut[1], &one);
    return OK;
  }

  future_get(fibfut[n-2], &minus2);
  future_get(fibfut[n-1], &minus1);

  this = minus1 + minus2;

  future_set(fibfut[n], &this);

  return(0);

}