#include "sparsemat.h"

#include <stdio.h>
#include <stdlib.h>





sp_tuples * load_tuples(char* input_file)
{
  FILE *fp; //create fp to read input file, local variables to hold dimensions and nz
  int row, col;
  sp_tuples_node *head = (sp_tuples_node *)malloc(sizeof(sp_tuples_node));
  fp = fopen(input_file, "r");
  if(fp == NULL){
    return NULL;
  }
  fscanf(fp, "%d %d", &row, &col);
  sp_tuples *tuples = (sp_tuples *)malloc(sizeof(sp_tuples)); //create sp_tuples object
  tuples->m = row;
  tuples->n =col;
  tuples->nz = 0;
  double val;
  while(fscanf(fp, "%d %d %lf", &row, &col, &val) != EOF){ //read each tuple and create a node for it, then insert node into list
    if(val != 0){
      sp_tuples_node *current = head; //create current, prev, and temp nodes used to insert node
      sp_tuples_node *prev = head;
      sp_tuples_node *temp = (sp_tuples_node *)malloc(sizeof(sp_tuples_node));
      temp->row = row; //fill data besides next pointer in temp, to be used in placement
      temp->col = col;
      temp->value = val;
      int tempRowMaj = temp->row * tuples->m + temp->col;
      while(current != NULL){ //go through linked list to place the node
        int currentRowMaj = current->row * tuples->m + current->col;
        if(tempRowMaj < currentRowMaj){ //if place for node is found, place it
          temp->next = current; //temp's next points to current, because it gets placed before
          if(current == head){ //if the current node is the head, update the head pointer
            head = temp;
          }
          else{ //otherwise, update the previous node's next pointer
            prev->next = temp;
          }
        }
        if(current->next == NULL){ //if we've reached the end of the list, add it to the end
          current->next = temp;
          temp->next = NULL;
        }
        tuples->nz += 1;
        prev = current; //update prev and current to move on in list;
        current = current->next;
      }
    }
  }
  tuples->tuples_head = head; //set tuples_head to head node from created list
  return tuples;
}



double gv_tuples(sp_tuples * mat_t,int row,int col)

{
    sp_tuples_node *current = mat_t->tuples_head;
    while(current != NULL){ //traverse list looking for value
      if(current->row == row && current->col == col){ //if found a tuple at (row, col), return tuple->value
        return current->value;
      }
      current = current->next;
    }
    return 0; //otherwise, return 0
}



void set_tuples(sp_tuples * mat_t, int row, int col, double value)
{
  sp_tuples_node *temp, *prev;
  if(mat_t->tuples_head == NULL && value != 0){ //if val isnt zero and head doesn't exist, create it and make it point to value
    mat_t->tuples_head = (sp_tuples_node *)malloc(sizeof(sp_tuples_node));
    sp_tuples_node *head = mat_t->tuples_head;
    head->next = (sp_tuples_node *)malloc(sizeof(sp_tuples_node));
    head->next->row = row;
    head->next->col = col;
    head->next->value = value;
    head->next->next = NULL;
    mat_t->nz += 1;
    return;
  }
  temp = mat_t->tuples_head;
  prev = NULL;
  while(temp != NULL && row > temp->row){ //traverse list until row <= temp->row
    prev = temp;
    temp = temp->next;
  }
  while(temp != NULL && col > temp->col && row >= temp->row){ //traverse until temp is in correct place on row
    prev = temp;
    temp = temp->next;
  }
  if(temp != NULL && col == temp->col && row == temp->row){ //if tuple at (row, col) already exists, modify it
    if(value != 0){
      temp->value = value;
    }
    else{
      if(prev != NULL){ //if prev isn't null, set prev to link to value atfer next
        prev->next = temp->next;
      }
      else{ //if prev == NULL, temp == head. change head to point to value after next
        mat_t->tuples_head = temp->next;
      }
      free(temp);
      mat_t->nz -= 1;
    }
  }
  else if(value != 0){
    if(prev != NULL){ //if temp isn't head
      prev->next = (sp_tuples_node *)malloc(sizeof(sp_tuples_node)); //insert value between prev and temp
      prev->next->row = row;
      prev->next->col = col;
      prev->next->value = value;
      prev->next->next = temp;
      mat_t->nz += 1;
    }
    else{ //if temp is head, insert value after head
      mat_t->tuples_head = (sp_tuples_node *)malloc(sizeof(sp_tuples_node));
      mat_t->tuples_head->next = (sp_tuples_node *)malloc(sizeof(sp_tuples_node));
      sp_tuples_node *place = mat_t->tuples_head->next;
      place->row = row;
      place->col = col;
      place->value = value;
      place->next = temp;
      mat_t->nz += 1;
    }
  }
}



void save_tuples(char * file_name, sp_tuples * mat_t)
{
  sp_tuples_node *current;
  FILE *fp;
  double output[mat_t->m][mat_t->n]; //create 2D arr to hold matrix
  for(int i=0; i<mat_t->m; i++){
    for(int j=0; j<mat_t->n; j++){ //fill with zeroes;
      output[i][j] = (double)0;
    }
  }
  current = mat_t->tuples_head;
  while(current != NULL){ //traverse matrix, replace nonzero values in output
    output[current->row][current->col] = current->value;
    current = current->next;
  }
  printf("output matrix populated.\n");
  fp = fopen (file_name,"w"); //create output file named file_name
  if(fp!=NULL){
    fprintf(fp, "%d %d\n", mat_t->m, mat_t->n); //print first row, listing dimensions of the matrix
    for(int i=0; i<mat_t->m; i++){
      for(int j=0; j<mat_t->n; j++){ //loop through indices of matrix
        if(j < mat_t->n - 1){
          fprintf(fp, "%lf ", output[i][j]); //if its not the last entry in the row, print the value and a space
        }
        else{
          fprintf(fp, "%lf\n", output[i][j]); //if its the last entry, just print it and a newline
        }
      }
    }
  }
  fclose(fp);
}




sp_tuples * add_tuples(sp_tuples * matA, sp_tuples * matB){
  if(matA->m != matB->m || matA->n != matB->n){ //if matrices aren't same size, return NULL (addition impossible)
    return NULL;
  }
  sp_tuples *retmat = (sp_tuples *)malloc(sizeof(sp_tuples)); //create retmat with dimensions m=mA=mB x n=nA=nB and nz = 0
  retmat->m = matA->m;
  retmat->n = matA->n;
  retmat->tuples_head = NULL; //set to NULL, will be defined with use of set_tuples
  retmat->nz = 0;
  int row, col;
  sp_tuples_node *aCurrent = matA->tuples_head;//create nodes to traverse a and b
  sp_tuples_node *bCurrent = matB->tuples_head;
  double val1 = 0;
  while(aCurrent != NULL){ //traverse a, adding values to retmat
    row = aCurrent->row;
    col = aCurrent->col;
    val1 = gv_tuples(retmat, row, col) + aCurrent->value;
    set_tuples(retmat, row, col, val1);
    aCurrent = aCurrent->next;
  }
  double val2 = 0;
  while(bCurrent != NULL){ //traverse b, adding values to retmat
    row = bCurrent->row;
    col = bCurrent->col;
    val2 = gv_tuples(retmat, row, col) + bCurrent->value;
    set_tuples(retmat, row, col, val2);
    bCurrent = bCurrent->next;
  }
  sp_tuples_node *retNode = retmat->tuples_head;
  while(retNode != NULL){ //traverse retmat and count nz values
    retmat->nz += 1;
    retNode = retNode->next;
  }
  return retmat;
}



sp_tuples * mult_tuples(sp_tuples * matA, sp_tuples * matB){
  int iA, jA, iB, jB;
  if(matA->n != matB->m){ //if multiplication is impossible, return NULL
    return NULL;
  }
  sp_tuples *retmat = (sp_tuples *)malloc(sizeof(sp_tuples)); //create retmat with m = mA and n = nB and nz = 0
  retmat->m = matA->m;
  retmat->n = matB->n;
  retmat->nz = 0;
  sp_tuples_node *aCurrent = matA->tuples_head;
  double val = 0;

  while(aCurrent != NULL){ //traverse a, defining iA and jA
    iA = aCurrent->row;
    jA = aCurrent->col;
    sp_tuples_node *bCurrent = matB->tuples_head;
    while(bCurrent != NULL){ //traverse b, defining iB and jB
      iB = bCurrent->row;
      jB = bCurrent->col;
      if(iB == jA){ //if iB jA, set C(iA, jB) == C(iA, jB) + A(iA, jA) * B(iB, jB)
        val = gv_tuples(retmat, iA, jB) + aCurrent->value * bCurrent->value;
        set_tuples(retmat, iA, jB, val);
      }
      bCurrent = bCurrent->next;
    }
    aCurrent = aCurrent->next;
  }
  sp_tuples_node *retCurrent = retmat->tuples_head; //create node to count nonzero values in retmat
  while(retCurrent != NULL){
    retmat->nz += 1;
    retCurrent = retCurrent->next;
  }
  return retmat;
}



void destroy_tuples(sp_tuples * mat_t){
  sp_tuples_node *current = mat_t->tuples_head;
  while(current->next != NULL){
    current->next = current->next->next;
    free(current->next);
  }
  free(current);
  return;
}
