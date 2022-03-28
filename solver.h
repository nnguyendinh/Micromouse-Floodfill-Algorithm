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

Queue is implement (I hope its correct), and I finished the ability to detect and remember walls

Now we just need to implement the pseudocode to be able to 
recalculate manhattan distances when it gets stuck
*/



Action solver();
Action leftWallFollower();
Action floodFill();

struct Cell* currPos;
Heading currHead;
int Manhattans[16][16];
struct Cell* queue[512];
int queueStart; //assuming circular queue, this helps us keep track of what position we are in in the queue, in terms of the "start"
int queueEnd; //keep track of end of queue, where to add next

int horzWall[17][16]; // got rid of the extra in the array to have hopefully less confusion
int vertWall[16][17]; 

#endif
