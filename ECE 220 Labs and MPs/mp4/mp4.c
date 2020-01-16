#include <stdlib.h>
#include <stdio.h>


int is_prime(int number);
int print_semiprimes(int a, int b);


int main(){
   int a, b;
   printf("Input two numbers: "); //prompt input from user
   scanf("%d %d", &a, &b);        //get input from user
   if( a <= 0 || b <= 0 ){    //if a or b is negative, throw error
     printf("Inputs should be positive integers\n");
     return 1;
   }

   if( a > b ){ //if a > b, throw error
     printf("The first number should be smaller than or equal to the second number\n");
     return 1;
   }

   // TODO: call the print_semiprimes function to print semiprimes in [a,b] (including a and b)
   else{  //else, print semiprimes
     int result = print_semiprimes(a, b);
     if (result == 0){  //if print_semiprimes returns 0, print "0"
       printf("0");
     }
     printf("\n");
     return 0;
   }

}


/*
 * TODO: implement this function to check the number is prime or not.
 * Input    : a number
 * Return   : 0 if the number is not prime, else 1
 */
int is_prime(int number){
  if(number<2){ //catches error that 1 is a prime number
    return 0;
  }
  for(int i=2; i<number; i++){
    int rem = number%i;
    if(rem==0){   //if number is divisible (remainder = 0), it's not prime
      return 0;
    }
  }

  return 1;
}


/*
 * TODO: implement this function to print all semiprimes in [a,b] (including a, b).
 * Input   : a, b (a should be smaller than or equal to b)
 * Return  : 0 if there is no semiprime in [a,b], else 1
 */
int print_semiprimes(int a, int b){
  int spExists = 0;
  for(int n=a; n<=b; n++){    //loop through all n between a and b
    for(int k=2; k<n; k++){ //loop through all potential factors of n
      if(is_prime(k) == 1 && n%k == 0 && is_prime(n/k) == 1){   //if k is prime, and n%k=0 (k is a factor of n), and n/k (other factor) is prime, print n (it is semiprime)
        printf("%d ", n);
        spExists = 1;   //mark that semiprimes exist
        break;
      }
    }
  }

  if(spExists == 1)
    return 1;
  else
    return 0;
}
