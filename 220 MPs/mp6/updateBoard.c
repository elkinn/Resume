/*
 *countLiveNeighbor was implemented looping through neigbor cells, checking that the cells were
 *valid and not the cell in question, and adding to the count if the cell was live.
 *updateBoard was implemented by creating a copy of the board to apply updates to without
 *changing the cell mid-generation. once the generation's updates were complete, the
 *copy was applied back to the game board. aliveStable used the same algorithm as
 *updateBoard, but instead of updatig the game board, just stated that the board was
 *not stable if an update would occur.
 */

int countLiveNeighbor(int* board, int boardRowSize, int boardColSize, int row, int col){
	int count = 0; //init variable to count neighbors
	for(int i=row-1; i<=row+1; i++){ //loop through neighbors to right and left
		for(int j = col-1; j<=col+1; j++){ //and above and below
			if( i<0 || i>boardRowSize-1 || j<0 || j>=boardColSize-1 || (i == row && j == col)){continue;} //dont check board[i,j] if (i,j) is out of bounds
			count += board[(i*boardColSize) + j]; //if board[i,j] = 1, add it to live neighbors
		}
	}
	return count;
}
/*
 * Update the game board to the next step.
 * Input:
 * board: 1-D array of the current game board. 1 represents a live cell.
 * 0 represents a dead cell
 * boardRowSize: the number of rows on the game board.
 * boardColSize: the number of cols on the game board.
 * Output: board is updated with new values for next step.
 */
void updateBoard(int* board, int boardRowSize, int boardColSize) {
	int copy[boardColSize*boardRowSize];
	for(int i=0; i<boardRowSize; i++){ //create a copy of the board to update the cells without changing the board
		for(int j=0; j<boardColSize; j++){
			int index = i*boardColSize + j;
			copy[index] = board[index];
		}
	}
	for(int i=0; i<boardRowSize; i++){
		for(int j=0; j<boardColSize; j++){ //loop through cells
			int liveneighbors = countLiveNeighbor(board, boardRowSize, boardColSize, i, j); //count live neighbors at the cell
			int index = i*boardColSize + j; //create index variable based on location
			if(board[index] == 1 && (liveneighbors > 3 || liveneighbors < 2)){ //if cell is live and over/underpopulated, dies
				copy[index] = 0;
			}
			else if(board[index] == 0 && liveneighbors == 3){ //if cell is dead w 3 neighbors, it comes to life
				copy[index] = 1;
			}
		}
	}
	for(int i=0; i<boardRowSize; i++){ //paste copied board back onto actual game board
		for(int j=0; j<boardColSize; j++){
			int index = i*boardColSize + j;
			board[index] = copy[index];
		}
	}
}

/*
 * aliveStable
 * Checks if the alive cells stay the same for next step
 * board: 1-D array of the current game board. 1 represents a live cell.
 * 0 represents a dead cell
 * boardRowSize: the number of rows on the game board.
 * boardColSize: the number of cols on the game board.
 * Output: return 1 if the alive cells for next step is exactly the same with
 * current step or there is no alive cells at all.
 * return 0 if the alive cells change for the next step.
 */
int aliveStable(int* board, int boardRowSize, int boardColSize){
	int isStable = 1;
	for(int i = 0; i<boardRowSize; i++){
		for(int j = 0; j<boardColSize; j++){ //loop through rows and columns
			int neighbors = countLiveNeighbor(board, boardRowSize, boardColSize, i, j);
			int index = (i*boardColSize) + j; //creatie int index used to offset board pointer
			if((board[index] == 1 && neighbors < 2) || (board[index] == 1 && neighbors > 2)){ //if cell is live and neighbors < 2 or > 3, cell dies (is not stable)
				isStable = 0;
			}
			else if(board[index] == 0 && neighbors == 3){	//if cell is dead and neighbors == 3, cell is live (is not stable)
				isStable = 0;
			}
		}
	}
	return isStable;
}
