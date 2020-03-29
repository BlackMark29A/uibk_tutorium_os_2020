#include <stdio.h> 

#include<stdlib.h>  

int fib(int n) {
if(n<= 1)   return n; 

  return fib(n-2 ) + fib( n - 1) 
  ; 
 }

int main( ) 
{   

    
printf("Fibonacci numbers:\n");

  for (int i=0 ; i <=40; ++ i) 
 printf("fib(%d) = %d\n", i, fib(i));
 } 