#ifndef SOLVER_H
#define SOLVER_H

typedef enum Heading {NORTH, EAST, SOUTH, WEST} Heading;
typedef enum Action {LEFT, FORWARD, RIGHT, IDLE} Action;

struct Cell
{
    int x;
    int y;
};

void initCell(struct Cell* a, int x_, int y_);  // Initialize an existing cell with values x and y
void insertQueue();

Action solver();
Action leftWallFollower();
Action floodFill();

int row;
int col;
Heading currHead;
int Manhattans[16][16];
queue Cell[512];

#endif