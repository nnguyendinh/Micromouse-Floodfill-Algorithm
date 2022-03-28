#include <stdio.h>
#include "solver.h"
#include "API.h"

int main(int argc, char* argv[]) {
    debug_log("Running...");
    while (1) {
        Action nextMove = solver();
        switch(nextMove){
            case FORWARD:
                API_moveForward();
                break;
            case LEFT:
                API_turnLeft();
                break;
            case RIGHT:
                API_turnRight();
                break;
            case IDLE:
                break;
        }
    }
}