This is a CL version of Conway's Game of Life.

There are 3 ways to run the simulation
1. Random game with random dimensions (max size 10x10)
2. Provide dimensions, random initial state. Dimensions provided as CL arguments
3. Provide file with format as in example file "game.txt". Input file provided as CL argument.

Once the game has been intialized there are two ways to proceed through the simulation
1. Press "C" and watch as steady state is reached (each step printed to CL)
2. Enter number of steps to advance in the simulation. Output of last step printed to stdout.

To compile; g++ GameOfLife.cpp -o GameOfLife.out -std=c++14

To run
1. ./GameOfLife.out
2. ./GameOfLife.out X Y
3 ./GameOfLife.out game.txt# 6.179_finalproj
