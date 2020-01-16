#include <stdio.h>
#include "prime.h"

int main(){
  // Write the code to take a number n from user and print all the prime numbers between 1 and n
  int number;
  int answer;

  printf("Enter a number: ");
  scanf("%d", &number);
  printf("Printing primes less than or equal to %d: \n", number);

  for(int i=0; i<=number; i++){
    answer = is_prime(i);
    if(answer == 1 && i > 1){
      printf("%d ", i);
    }
  }
  printf("\n");
  return 0;
}
