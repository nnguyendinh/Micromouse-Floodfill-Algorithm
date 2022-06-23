#ifndef SOLVER_H
#define SOLVER_H

typedef enum Heading { NORTH, EAST, SOUTH, WEST } Heading;
typedef enum Action { LEFT, FORWARD, RIGHT, IDLE } Action;

struct Cell
{
    int row;
    int col;
};

struct Cell* newCell(int x_, int y_);  // Initialize an existing cell with values x and y
void insertQueue(struct Cell* input);
void popQueueFront();
struct Cell* queueFront();

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
