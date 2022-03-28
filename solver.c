#include "solver.h"
#include "API.h"
#include <stdlib.h>

int initialized = 0;

void initElements()
{
    currPos = newCell(15, 0);           // Sets current position to row 15, column 0
    currHead = NORTH;                    // Sets current heading to north
    for (int j = 0; j < 8; j++)                // Initializes default Manhattan distances for empty maze
    {
        for (int i = 0; i < 8; i++)
        {
            Manhattans[i][j] = 14 - i - j;
            Manhattans[15 - i][j] = 14 - i - j;
            Manhattans[i][15 - j] = 14 - i - j;
            Manhattans[15 - i][15 - j] = 14 - i - j;
        }
    }
    
    for (int j = 0; j < 15; j++)
        for (int i = 0; i < 16; i++)
        {
            
        }

    queueStart = 0;
    queueEnd = 0;
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
        vertWall[currPos->row][currPos->col + 2] = 1;   // May need to check 2D array logic my head hurts lol
        API_setWall(currX, currY, 'e');
        break;
    case SOUTH:
        horzWall[currPos->row + 2][currPos->col] = 1;
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

struct Cell* newCell(int r, int c)           // Acts as a constructor for a cell cuz C is annoying
{
    struct Cell* p = malloc(sizeof(struct Cell));
    p->row = r;
    p->col = c;
}

void insertQueue(struct Cell* input) {
    queueEnd++;
    
    if (queueEnd == 512) {
        queueEnd = 0;
        //reset cause circular queue
    }
    
    queue[queueEnd] = input;
    //check me on this i might've messed up on pointers, i'm doing this right off of github and not from a compiler lol
}

void popQueueFront()
{
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

Action solver() {
    return floodFill();
}

Action leftWallFollower() {       // The simple left wall following algorithm that they provided
    if(API_wallFront()) {
        if(API_wallLeft()){
            return RIGHT;
        }
        return LEFT;
    }
    return FORWARD;
}

Action floodFill() {
    if (!initialized)           // Initializes all the elements once (there might be a better way to do this idk)
    {
        initElements();
        initialized = 1;
    }
    
    detectWalls();  // Lights up detected walls and adds them to the 2D wall arrays

    int nextHead = -1;
    int row = currPos->row;
    int col = currPos->col;

    if (row != 0 && Manhattans[row - 1][col] < Manhattans[row][col])
        nextHead = NORTH;
    if (col != 15 && Manhattans[row][col + 1] < Manhattans[row][col])
        nextHead = EAST;
    if (row != 15 && Manhattans[row + 1][col] < Manhattans[row][col])
        nextHead = SOUTH;
    if (col != 0 && Manhattans[row][col - 1] < Manhattans[row][col])       // Find next heading
        nextHead = WEST;

    if (nextHead == -1)                     // If no path available, then idle (not correct)
        return IDLE;

    if (nextHead == currHead)               // If next heading is in same direction, move forward
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

    if ((nextHead - currHead) % 2 == 0)         // If next heading is in opposite direction, turn right
    {
        if (currHead == WEST)
            currHead = NORTH;
        else
            currHead++;
        return RIGHT;
    }

    if ((nextHead - currHead) == 1 || nextHead - currHead == -3)  // If next heading is right, turn right
    {
        if (currHead == WEST)
            currHead = NORTH;
        else
            currHead++;
        return RIGHT;
    }

    if (currHead == NORTH)  // else, turn left
        currHead = WEST;
    else
        currHead--;
    return LEFT;
}
