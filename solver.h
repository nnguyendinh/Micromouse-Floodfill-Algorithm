#ifndef SOLVER_H
#define SOLVER_H

typedef enum Heading {NORTH, EAST, SOUTH, WEST} Heading;
typedef enum Action {LEFT, FORWARD, RIGHT, IDLE} Action;

struct Cell
{
    int row;
    int col;
};

struct Cell* newCell(int x_, int y_);  // Initialize an existing cell with values x and y
void insertQueue();     // TODO: Need to implement queue data structure

/*
Rn its capable of initializing the default Manhattan distances and moving in the direction
of decreasing distance until it can't anymore

We still need to implement the queue, store locations of known walls, and then be able to 
recalculate manhattan distances when it gets stuck
*/


Action solver();
Action leftWallFollower();
Action floodFill();

struct Cell* currPos;
Heading currHead;
int Manhattans[16][16];
struct Cell queue[512];

#endif