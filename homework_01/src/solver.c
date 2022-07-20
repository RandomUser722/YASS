#include "solver.h"

// states of possible solution, @todo enum
#define POSSIBLE 1	// solution is still possible
#define EXCLUDED 0	// solution was already excluded

uint8_t findSolution(uint8_t x, uint8_t y);
void resetElement(uint8_t x, uint8_t y);
uint8_t countUpSolutions(uint8_t x, uint8_t y);
uint8_t isElementSolved(uint8_t x, uint8_t y);
void removePossibility(uint8_t x, uint8_t y, uint8_t tx, uint8_t ty);
void saveSolution(uint8_t x, uint8_t y);
void subgridCheck(uint8_t x, uint8_t y);

// cube is variable where all the solving happens
// 0th layer is for solved elements
// 1-9th layer stores possible solutions for corresponding element
// 10th dimension stores count of possible solutions for corresponding element (sum of layers 1 to 9)
uint8_t cube[9][9][11];

error_t solver_solveSudoku( uint8_t *Grid){
	uint8_t unsolvedElementFound = 0;
	uint8_t newSolutionFound;

	// load elements
	uint8_t x, y;
	for(x = 0; x <= 8; x++){
		for(y = 0; y <= 8; y++){
			cube[x][y][0] = Grid[x*9 + y];
		}
	}

	while(1){
		newSolutionFound = 0;
		unsolvedElementFound = 0;

		for(x = 0; x <= 8; x++){
			for(y = 0; y <= 8; y++){

				if(isElementSolved(x, y)){
					continue;
				}
				unsolvedElementFound = 1;
				if( !findSolution(x, y)){ // element has been solved
					newSolutionFound = 1;
					break;
				}
			} // for
			if(newSolutionFound){
				break;
			}
		} // for

		if(newSolutionFound){	// progress in solving -> continue
			continue;
		}


		// copy solution back to grid
		for(x = 0; x <= 8; x++){
			for(y = 0; y <= 8; y++){
				Grid[x*9 + y] = cube[x][y][0];
			}
		}

		if( unsolvedElementFound && ! newSolutionFound ){ // can't find solution
			return ERR_SOLUTION;
		}

		if( ! unsolvedElementFound){ // sudoku finished
			return ERR_OK;
		}
	} // while
}

uint8_t isElementSolved(uint8_t x, uint8_t y){
	if(cube[x][y][0] == 0){
		return 0;
	}else{
		return 1;
	}
}

// Enumerate all possible solutions. If only single solution exists, element is solved.
uint8_t findSolution(uint8_t x, uint8_t y){
	uint8_t tx, ty;

	resetElement(x, y);

	// check the up direction
	tx = x;
	while(tx > 0){
		tx--;	// shift up from current element
		if(isElementSolved(tx, y)){
			removePossibility(x, y, tx, y);
		}
	}

	// check the down direction
	tx = x;
	while(tx < 8){
		tx++;	// shift down from current element
		if(isElementSolved(tx, y)){
			removePossibility(x, y, tx, y);
		}
	}

	// check to the left
	ty = y;
	while(ty > 0){
		ty--;	// shift to the left from current element
		if(isElementSolved(x, ty)){
			removePossibility(x, y, x, ty);
		}
	}

	// check to the right
	ty = y;
	while(ty < 8){
		ty++;	// shift to the right from current element
		if(isElementSolved(x, ty)){
			removePossibility(x, y, x, ty);
		}
	}

	// subgrid check
	subgridCheck(x, y);


	if( countUpSolutions(x, y) == 1){	// solution found
		saveSolution(x, y);
		return 0;
	}else{
		return 1;
	}
}

// if only one possibility exist in this element, mark it as a solution
void saveSolution(uint8_t x, uint8_t y){
	uint8_t i = 1;

	for(i = 1; i <= 9; i++){
		if( cube[x][y][i] == POSSIBLE){
			cube[x][y][0] = i;
			break;
		}
	}
}

// removes number from possible solutions
void removePossibility(uint8_t x, uint8_t y, uint8_t tx, uint8_t ty){
	uint8_t index = cube[tx][ty][0];
	cube[x][y][index] = EXCLUDED;
}

void subgridCheck(uint8_t x, uint8_t y){
	uint8_t x_min, x_max, y_min, y_max;
	uint8_t res;
	uint8_t tx, ty;

	res = x/3;
	if(res == 0){
		x_min = 0;
		x_max = 2;
	}else if(res == 1){
		x_min = 3;
		x_max = 5;
	}else if(res == 2){
		x_min = 6;
		x_max = 8;
	}

	res = y/3;
	if(res == 0){
		y_min = 0;
		y_max = 2;
	}else if(res == 1){
		y_min = 3;
		y_max = 5;
	}else if(res == 2){
		y_min = 6;
		y_max = 8;
	}

	for(ty=y_min; ty <= y_max; ty++){
		for(tx=x_min; tx <= x_max; tx++){
			if(isElementSolved(tx, ty)){
				removePossibility(x, y, tx, ty);
			}
		}
	}
}

// return count of possibilities for this element
uint8_t countUpSolutions(uint8_t x, uint8_t y){
	uint8_t count = 0;
	uint8_t i;

	for(i = 1; i <= 9; i++){
		if( cube[x][y][i] == POSSIBLE){
			count++;
		}
	}
	return count;
}

// sets all number as possibilities for this element
void resetElement(uint8_t x, uint8_t y){
	uint8_t i;

	for(i = 1; i <= 9; i++){
		cube[x][y][i] = POSSIBLE;
	}
}
