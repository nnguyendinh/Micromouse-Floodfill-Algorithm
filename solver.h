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
void insertQueue(struct Cell* input);    
void popQueueFront();
struct Cell* queueFront();

/*
Rn its capable of initializing the default Manhattan distances and moving in the direction
of decreasing distance until it can't anymore

We still need to implement the queue, store locations of known walls, and then be able to 
recalculate manhattan distances when it gets stuck
*/



//for queue, first in first out, so we can probably simply while "popping" queueNodes just delete current working node,
//   and then update current top of queue variable as it comes

Action solver();
Action leftWallFollower();
Action floodFill();

struct Cell* currPos;
Heading currHead;
int Manhattans[16][16];
struct Cell* queue[512];
int queueStart; //assuming circular queue, this helps us keep track of what position we are in in the queue, in terms of the "start"
int queueEnd; //keep track of end of queue, where to add next


// i am thinking that if it is a previously filled queue value, we just override it, if that makes sense

int horzWall[17][16]; // one extra in the array, accounting starting from left-most wall, need one extra for the right-most wall
int vertWall[16][17]; // one extra in the array, accounting starting from top-most wall, need one extra for the lower-most wall

#endif
