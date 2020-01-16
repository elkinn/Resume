#include "sudoku.h"

//-------------------------------------------------------------------------------------------------
// Start here to work on your MP7
//-------------------------------------------------------------------------------------------------

// You are free to declare any private functions if needed.

// Function: is_val_in_row
// Return true if "val" already existed in ith row of array sudoku.
int is_val_in_row(const int val, const int i, const int sudoku[9][9]) {

  assert(i>=0 && i<9);

  // BEG TODO
  for(int j = 0; j<9; j++){//loop through row and check vals
    if(sudoku[i][j] == val){
      return 1; //if val is in row, return 1 (true)
    }
  }
  return 0;
  // END TODO
}

// Function: is_val_in_col
// Return true if "val" already existed in jth column of array sudoku.
int is_val_in_col(const int val, const int j, const int sudoku[9][9]) {

  assert(j>=0 && j<9);

  // BEG TODO
  for(int i=0; i<9; i++){ //loop through column
    if(sudoku[i][j] == val){
      return 1; //if val is found, return 1 (true)
    }
  }
  return 0;
  // END TODO
}

// Function: is_val_in_3x3_zone
// Return true if val already existed in the 3x3 zone corresponding to (i, j)
int is_val_in_3x3_zone(const int val, const int i, const int j, const int sudoku[9][9]) {

  assert(i>=0 && i<9);

  // BEG TODO
  int icorner, jcorner; //declare variables that hold top corner of 3x3 zone
  switch(i/3){ //divide i by 3 to round down to correspond to top row of 3x3 zone
    case 0:
      icorner = 0; //top row, i=0
      break;
    case 1:
      icorner = 3; //middle row, i=3
      break;
    case 2:
      icorner = 6; //bottom row, i=6
      break;
    default:
      icorner = 0; //default shouldn't occur but to be safe
  }
  switch(j/3){ //divide j by 3 to do same as above, but for column
    case 0:
      jcorner = 0;
      break;
    case 1:
      jcorner = 3;
      break;
    case 2:
      jcorner = 6;
      break;
    default:
      jcorner = 0;
  }

  for(int x=icorner; x<icorner + 3; x++){
    for(int y=jcorner; y<jcorner + 3; y++){ //loop through 3x3 zone and check if val is present
      if(sudoku[x][y] == val){
        return 1; //if val is found return true
      }
    }
  }
  return 0;
  // END TODO
}

// Function: is_val_valid
// Return true if the val is can be filled in the given entry.
int is_val_valid(const int val, const int i, const int j, const int sudoku[9][9]) {

  //printf("i = %d, j = %d\n", i, j);
  assert(i>=0 && i<9 && j>=0 && j<9);

  // BEG TODO
  if(is_val_in_row(val, i, sudoku) == 0 && is_val_in_col(val, j, sudoku) == 0 && is_val_in_3x3_zone(val, i, j, sudoku) == 0){
    return 1; //return 1 (true) if the value isn't in the row, column, or 3x3 zone
  }
  else{
    return 0;
  }
  // END TODO
}

// Procedure: solve_sudoku
// Solve the given sudoku instance.
int solve_sudoku(int sudoku[9][9]) {

  // BEG TODO.
  int i,j;
  int isEmpty = 0;
  for(j=0; j<9; j++){ //check to see if there are any empty cells
    for(i=0; i<9; i++){
      if(sudoku[i][j] == 0){
        isEmpty = 1;
        break;
      }
    }
    if(isEmpty){ //break out of outer loop as well if empty cell is found
      break;
    }
  }
  if(isEmpty == 0){ //if there are no empty cells, return 1
    return 1;
  }

  for(int num = 1; num <= 9; num++){
    if(is_val_valid(num, i, j, sudoku)){ //if val is valid, set it in the solution
      sudoku[i][j] = num;
      if(solve_sudoku(sudoku)){ //recursively call solution to see if this solution leads to further solutions
        return 1; //if yes, finish
      }
      sudoku[i][j] = 0; //if 1 is not returned, solve didn't complete, clear cells
    }
  }
  return 0;
  // END TODO.
}

// Procedure: print_sudoku
void print_sudoku(int sudoku[9][9])
{
  int i, j;
  for(i=0; i<9; i++) {
    for(j=0; j<9; j++) {
      printf("%2d", sudoku[i][j]);
    }
    printf("\n");
  }
}

// Procedure: parse_sudoku
void parse_sudoku(const char fpath[], int sudoku[9][9]) {
  FILE *reader = fopen(fpath, "r");
  assert(reader != NULL);
  int i, j;
  for(i=0; i<9; i++) {
    for(j=0; j<9; j++) {
      fscanf(reader, "%d", &sudoku[i][j]);
    }
  }
  fclose(reader);
}
