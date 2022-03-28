#include "solver.h"
#include "API.h"

int initialized = 0;

void initElements()
{
    row = 15;
    col = 0;
    currHead = NORTH;
    for (int j = 0; j < 8; j++)
    {
        for (int i = 0; i < 8; i++)
        {
            Manhattans[i][j] = 14 - i - j;
            Manhattans[15 - i][j] = 14 - i - j;
            Manhattans[i][15 - j] = 14 - i - j;
            Manhattans[15 - i][15 - j] = 14 - i - j;
        }
    }
}

void initCell(struct Cell* a, int x_, int y_)
{
    a->x = x_;
    a->y = y_;
}

Action solver() {
    return floodFill();
}

Action leftWallFollower() {
    if(API_wallFront()) {
        if(API_wallLeft()){
            return RIGHT;
        }
        return LEFT;
    }
    return FORWARD;
}

Action floodFill() {
    if (!initialized)
    {
        initElements();
        initialized = 1;
    }
    
    int nextHead = -1;

    if (row != 0 && Manhattans[row - 1][col] < Manhattans[row][col])
        nextHead = NORTH;
    if (col != 15 && Manhattans[row][col + 1] < Manhattans[row][col])
        nextHead = EAST;
    if (row != 15 && Manhattans[row + 1][col] < Manhattans[row][col])
        nextHead = SOUTH;
    if (col != 0 && Manhattans[row][col - 1] < Manhattans[row][col])       // Find next heading
        nextHead = WEST;

    if (nextHead == -1)
        return IDLE;

    if (nextHead == currHead)
    {
        switch (currHead)
        {
        case NORTH:
            row--;
            break;
        case EAST:
            col++;
            break;
        case SOUTH:
            row++;
            break;
        case WEST:
            col--;
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
