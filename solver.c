#include "solver.h"
#include "API.h"
#include <stdlib.h>
#include <stdio.h>

int initialized = 0;
int goToCenter = 1;

struct Cell* newCell(int r, int c)           // Acts as a constructor for a cell cuz C is annoying
{
    struct Cell* p = malloc(sizeof(struct Cell));
    p->row = r;
    p->col = c;
    return p;
}

void insertQueue(struct Cell* input) {
    queue[queueEnd] = input;

    queueEnd++;

    if (queueEnd == 512) {
        queueEnd = 0;
        //reset cause circular queue
    }
}

void popQueueFront()
{
    free(queueFront());

    queueStart++;
    if (queueStart == 512) {
        queueStart = 0;
        //reset cause circular queue
    }
}

struct Cell* queueFront()
{
    return queue[queueStart];
}

void initElements()
{
    currPos = newCell(15, 0);           // Sets current position to row 15, column 0
    currHead = NORTH;                    // Sets current heading to north

    for (int i = 0; i < 17; i++) {
        for (int j = 0; j < 16; j++) {
            horzWall[i][j] = 0;
        }
    }

    for (int i = 0; i < 16; i++) {
        for (int j = 0; j < 17; j++) {
            vertWall[i][j] = 0;
        }
    }

    queueStart = 0;
    queueEnd = 0;
}

void displayManhatttans()       // Displays all current manhattan distances in grid
{
    for (int row = 0; row < 16; row++)
        for (int col = 0; col < 16; col++)
        {
            char str[4];
            sprintf(str, "%d", Manhattans[row][col]);
            API_setText(col, 15 - row, str);
        }
}

void setWall(int dir)
{
    int currX = currPos->col;
    int currY = 15 - currPos->row;

    switch (dir)
    {
    case NORTH:
        horzWall[currPos->row][currPos->col] = 1;   // Sets the 2D array value to 1 to represent true (there's no bool type in C)
        API_setWall(currX, currY, 'n');             // Light up the discovered wall in the simulator
        break;
    case EAST:
        vertWall[currPos->row][currPos->col + 1] = 1;   // May need to check 2D array logic my head hurts lol
        API_setWall(currX, currY, 'e');
        break;
    case SOUTH:
        horzWall[currPos->row + 1][currPos->col] = 1;
        API_setWall(currX, currY, 's');
        break;
    case WEST:
        vertWall[currPos->row][currPos->col] = 1;
        API_setWall(currX, currY, 'w');
        break;
    }
}

void detectWalls()
{
    switch (currHead)
    {
    case NORTH:
        if (API_wallFront())
        {
            setWall(NORTH);
        }
        if (API_wallLeft())
        {
            setWall(WEST);
        }
        if (API_wallRight())
        {
            setWall(EAST);
        }
        break;
    case EAST:
        if (API_wallFront())
        {
            setWall(EAST);
        }
        if (API_wallLeft())
        {
            setWall(NORTH);
        }
        if (API_wallRight())
        {
            setWall(SOUTH);
        }
        break;
    case SOUTH:
        if (API_wallFront())
        {
            setWall(SOUTH);
        }
        if (API_wallLeft())
        {
            setWall(EAST);
        }
        if (API_wallRight())
        {
            setWall(WEST);
        }
        break;
    case WEST:
        if (API_wallFront())
        {
            setWall(WEST);
        }
        if (API_wallLeft())
        {
            setWall(SOUTH);
        }
        if (API_wallRight())
        {
            setWall(NORTH);
        }
        break;
    }
}

void recalculate()
{
    queueStart = 0;
    queueEnd = 0;

    // Mark all cells as -1, meaning that they have not had their Manhattan set yet
    for (int j = 0; j < 16; j++)                
    {
        for (int i = 0; i < 16; i++)
        {
            Manhattans[i][j] = -1;
        }
    }

    if(goToCenter) 
    {
        // Set middle four manhattan distances to 0, and insert all 4 into queue (set middle as destination)
        Manhattans[7][7] = 0;
        Manhattans[7][8] = 0;
        Manhattans[8][7] = 0;
        Manhattans[8][8] = 0;
        insertQueue(newCell(7, 7));
        insertQueue(newCell(7, 8));
        insertQueue(newCell(8, 7));
        insertQueue(newCell(8, 8));
    }

    else
    {
        // Set starting cell to 0, insert starting cell into queue (set start as destination)
        Manhattans[15][0] = 0;
        insertQueue(newCell(15, 0));
    }


    //while queue is not empty
    while (queueStart != queueEnd) {

        struct Cell* currElement = queueFront(); 
        
        int currRow = currElement->row;
        int currCol = currElement->col;

        char str[50];
        sprintf(str, "Calculating distances at row %d, column %d", currRow, currCol);
        debug_log(str);

        // For all accessable neighbors with no set Manhattan distance, add cell to queue and set Manhattan to current Manhattan + 1

        //north wall
        if (currRow > 0 && horzWall[currRow][currCol] != 1 && Manhattans[currRow - 1][currCol] == -1) {
            Manhattans[currRow - 1][currCol] = Manhattans[currRow][currCol] + 1;
            insertQueue(newCell(currRow - 1, currCol));
        }
        //east wall
        if (currCol < 15 && vertWall[currRow][currCol + 1] != 1 && Manhattans[currRow][currCol + 1] == -1) {
            Manhattans[currRow][currCol + 1] = Manhattans[currRow][currCol] + 1;
            insertQueue(newCell(currRow, currCol + 1));
        }
        //south wall
        if (currRow < 15 && horzWall[currRow + 1][currCol] != 1 && Manhattans[currRow + 1][currCol] == -1) {        
            Manhattans[currRow + 1][currCol] = Manhattans[currRow][currCol] + 1;
            insertQueue(newCell(currRow + 1, currCol));
        }
        //west wall
        if (currCol > 0 && vertWall[currRow][currCol] != 1 && Manhattans[currRow][currCol - 1] == -1) {        
            Manhattans[currRow][currCol - 1] = Manhattans[currRow][currCol] + 1;
            insertQueue(newCell(currRow, currCol - 1));
        }

        // Deletes cell from queue and frees memory
        popQueueFront();      
    }
}

Action solver() {
    return floodFill();
}

Action leftWallFollower() {       // The simple left wall following algorithm that they provided
    if (API_wallFront()) {
        if (API_wallLeft()) {
            return RIGHT;
        }
        return LEFT;
    }
    return FORWARD;
}

Action floodFill() {
    // Initializes all the elements and calculates Manhattan distances
    if (!initialized)           
    {
        initElements();
        recalculate();
        initialized = 1;
    }

    // Lights up detected walls and adds them to the 2D wall arrays
    detectWalls();  

    // Display all Manhattan distances in the API dispaly
    displayManhatttans();

    int row = currPos->row;
    int col = currPos->col;

    // If goal has already been reached, set new destination to either middle or starting cell
    if (Manhattans[row][col] == 0)
    {
        if (goToCenter)
            goToCenter = 0; // Destination is now Starting Cell
        else
            goToCenter = 1; // Destination is now middle four

        recalculate();
        return IDLE;
    }


    int northBlocked = horzWall[row][col];
    int eastBlocked = vertWall[row][col + 1];
    int southBlocked = horzWall[row + 1][col];
    int westBlocked = vertWall[row][col];

    // Find next heading
    int nextHead = -1;

    if (row != 0 && Manhattans[row - 1][col] < Manhattans[row][col] && !northBlocked)
        nextHead = NORTH;
    if (col != 15 && Manhattans[row][col + 1] < Manhattans[row][col] && !eastBlocked)
        nextHead = EAST;
    if (row != 15 && Manhattans[row + 1][col] < Manhattans[row][col] && !southBlocked)
        nextHead = SOUTH;
    if (col != 0 && Manhattans[row][col - 1] < Manhattans[row][col] && !westBlocked)       
        nextHead = WEST;

    // If no path available, then recalculta
    if (nextHead == -1)                     
    {
        recalculate();
        return IDLE;
    }

    // If next heading is in same direction, move forward
    if (nextHead == currHead)               
    {
        switch (currHead)
        {
        case NORTH:
            currPos->row--;
            break;
        case EAST:
            currPos->col++;
            break;
        case SOUTH:
            currPos->row++;
            break;
        case WEST:
            currPos->col--;
            break;
        }
        return FORWARD;
    }

    // If next heading is in opposite direction, turn right
    if ((nextHead - currHead) % 2 == 0)         
    {
        if (currHead == WEST)
            currHead = NORTH;
        else
            currHead++;
        return RIGHT;
    }

    // If next heading is right, turn right
    if ((nextHead - currHead) == 1 || nextHead - currHead == -3)  
    {
        if (currHead == WEST)
            currHead = NORTH;
        else
            currHead++;
        return RIGHT;
    }

    // else, turn left
    if (currHead == NORTH)  
        currHead = WEST;
    else
        currHead--;
    return LEFT;
}
