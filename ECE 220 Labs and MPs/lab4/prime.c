#include "prime.h"

int is_prime(int n){
  // Return 1 if n is a prime, and 0 if it is not
  int isPrime = 1;
  for(int i=2; i<n; i++){
    int rem = n%i;
    if(rem==0){
      isPrime = 0;
    }
  }

  return isPrime;
}
