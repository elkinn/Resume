#include <stdio.h>
#include <stdlib.h>

int main()
{
  int row;
  

  printf("Enter a row index: ");
  scanf("%d",&row);


  // Write your code here

  for(int k=1; k<=row; k++){ //loop through the k places in row n of n+1 columns
    unsigned long cf = 1;
    if(k!=0){
      for(int i=1; i<=k; i++){
        cf = cf * (row + 1 - i)/i;
      }
    }
    printf("%lu ", cf);
  }


  printf("\n");
  return 0;
}
