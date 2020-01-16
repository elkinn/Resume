#include <stdio.h>
#include <stdlib.h>
#include "maze.h"


/*
 * createMaze -- Creates and fills a maze structure from the given file
 * INPUTS:       fileName - character array containing the name of the maze file
 * OUTPUTS:      None 
 * RETURN:       A filled maze structure that represents the contents of the input file
 * SIDE EFFECTS: None
 */
maze_t * createMaze(char * fileName)
{
    // Your code here. Make sure to replace following line with your own code.
    //instantiate local variables h and w to hold height and width read from maze, and create bounds to read height and width from file
    FILE* mazefile; //open the maze file to read it
    mazefile = fopen(fileName, "r");
    int h, w;
    char bounds[5];
    if(fgets(bounds, 5, mazefile)){
        //scan the string for bounds and assign them to h and w
        if(bounds[1] != ' '){   //if width is two digits, scan for width over 2 digits
            int wten = (bounds[0] - '0') * 10;
            w = wten + (bounds[1] - '0');
            if(bounds[4] != ' '){ //if height is two digits scan for height over 2 digits
                int hten = (bounds[3] - '0') * 10;
                h = (bounds[4] * 10) + hten;
            }
            else{ //if height is one digit, scan one digit
                h = bounds[3] - '0';
            }
        }
        else{ //same logic as above, but if width is 1 digit
            w = bounds[0] - '0';
            if(bounds[4] != ' '){
                int hten = (bounds[3] - '0') * 10;
                h = (bounds[4] * 10) + hten;
            }
            else{
                h = bounds[3] - '0';
            }
        }
    }
    maze_t *maze; //create pointer to maze object
    maze->height = h; //assign height to maze object
    maze->width = w; //assign width to maze object
    char **cells; //allocate memory for 2d arrays
    cells = (char **)malloc(sizeof(char) * h);
    for(int i=0; i<h; i++){
        cells[i] = (char *)malloc(sizeof(char) * w);
    }
    char line[w]; //create a line string to read lines into
    int i; //line counter
    while(fgets(line, w, mazefile)){ //read through the rest of the maze.txt file and transcribe it to the cells array
        for(int j=0; j<w; j++){
            cells[i][j] = line[j];
            if(line[j] == 'S'){ //when S is encountered, update startColumn and startRow
                maze->startColumn = j;
                maze->startRow = i;
            }
            else if(line[j] == 'E'){ //do the same for the end row
                maze->endColumn = j;
                maze->endRow = i;
            }
        }
        i++; //increment line counter
    }
    fclose(mazefile); //close mazefile
    return maze;
}

/*
 * destroyMaze -- Frees all memory associated with the maze structure, including the structure itself
 * INPUTS:        maze -- pointer to maze structure that contains all necessary information 
 * OUTPUTS:       None
 * RETURN:        None
 * SIDE EFFECTS:  All memory that has been allocated for the maze is freed
 */
void destroyMaze(maze_t * maze)
{
    // Your code here.
    free(maze->width);
    free(maze->height);
    free(maze->startColumn);
    free(maze->startRow);
    free(maze->endColumn);
    free(maze->endRow);
    free(maze->cells);
}

/*
 * printMaze --  Prints out the maze in a human readable format (should look like examples)
 * INPUTS:       maze -- pointer to maze structure that contains all necessary information 
 *               width -- width of the maze
 *               height -- height of the maze
 * OUTPUTS:      None
 * RETURN:       None
 * SIDE EFFECTS: Prints the maze to the console
 */
void printMaze(maze_t * maze)
{
    // Your code here.
    int h=maze->height, w=maze->width;
    char **cells[h][w];
    **cells = maze->cells;
    for(int i=0; i<h; i++){
        for(int j=0; j<w; j++){
            printf('%d', cells[i][j]);
        }
        printf("\n");
    }
}

/*
 * solveMazeManhattanDFS -- recursively solves the maze using depth first search,
 * INPUTS:               maze -- pointer to maze structure with all necessary maze information
 *                       col -- the column of the cell currently beinging visited within the maze
 *                       row -- the row of the cell currently being visited within the maze
 * OUTPUTS:              None
 * RETURNS:              0 if the maze is unsolvable, 1 if it is solved
 * SIDE EFFECTS:         Marks maze cells as visited or part of the solution path
 */ 
int solveMazeDFS(maze_t * maze, int col, int row)
{
    // Your code here. Make sure to replace following line with your own code.
    int h=maze->height, w=maze->width;
    char **cells[h][w];
    **cells = maze->cells;
    if(col >= w || col < 0 || row >= h || row < 0 || cells[row][col] != ' '){
        return 0;
    }
    else if(col == maze->endColumn && row == maze->endRow){
        return 1;
    }
    cells[row][col] = '*';
    if(solveMazeDFS(maze, col-1, row)){
        return 1;
    }
    else if(solveMazeDFS(maze, col+1, row)){
        return 1;
    }
    else if(solveMazeDFS(maze, col, row-1)){
        return 1;
    }
    else if(solveMazeDFS(maze, col, row+1)){
        return 1;
    }
    else{
        cells[row][col] = '~';
        return 0;
    }
}
