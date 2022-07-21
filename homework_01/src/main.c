#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "error.h"
#include "sudoku.h"
#include "loader.h"
#include "formatChecker.h"
#include "USART.h"
#include "solver.h"

#define SUDOKU_RAW_LEN 200
#define ASCII_OFFSET 48

/**
 * @brief when solvabilityOnly not 0, sudoku is not printed, only information if solution was found is printed
 */
uint8_t solvabilityOnly;

void checkArguments(int argc, char** argv);
void unpackSudoku(const char *raw, uint8_t *grid);
void packSudoku(const uint8_t *grid, char *raw);
int errorHandler(error_t e);
void convertSudokuToNumbers(uint8_t *sudoku);
void convertSudokuToASCII(uint8_t *sudoku);
error_t SolveSudoku(const char *Grid);

int main(int argc, char** argv){
	checkArguments(argc, argv);

	char sudoku_raw[SUDOKU_RAW_LEN];	// sudoku with formatting characters
	error_t e;

	while(1){
		e = loader_loadRawSudoku(sudoku_raw, SUDOKU_RAW_LEN);
		if( e != ERR_OK){
			errorHandler(e);
		}
		SolveSudoku(sudoku_raw);	// @todo check return value
	}
	return ERR_OK;
}

error_t SolveSudoku(const char *Grid){
	uint8_t sudoku[81];					// sudoku without formatting characters
	error_t e;
	char sudoku_raw[SUDOKU_RAW_LEN];	// sudoku with formatting characters

	memcpy((void*)sudoku_raw, (const void*) Grid, SUDOKU_RAW_LEN);

	e = formatChecker_checkSudoku(sudoku_raw);
	if(e != ERR_OK){
		errorHandler(e);
	}

	unpackSudoku(sudoku_raw, sudoku);
	convertSudokuToNumbers(sudoku);
	e = solver_solveSudoku(sudoku);

	if(solvabilityOnly){
		if( e==ERR_OK){
			printf("solvable\n");
			fflush(stdout);
		}else{
			printf("unsolvable\n");
			fflush(stdout);
		}
	}else{
		convertSudokuToASCII(sudoku);
		packSudoku(sudoku, sudoku_raw);
		USART_PrintSudokuGrid(sudoku_raw, SUDOKU_ROW_LENGTH);	// @todo check return value
	}
	return e;
}

/**
 * @brief check cmd line arguments
 */
void checkArguments(int argc, char** argv){
	if(argc > 1){
		// @todo check, if argv[1] == "--solvabilityOnly"
		solvabilityOnly = 1;
	}
}

/**
 * @brief removes formatting characters
 * @param raw: sudoku with formatting characters
 * @param grid: sudoku without formatting characters
 *
 */
void unpackSudoku(const char *raw, uint8_t *grid){
	uint8_t line, j, z=0;

	for(line = 0; line <= 10; line++){

		// skip divider lines
		if(line == 3 || line == 7){
			continue;
		}

		for(j = 0; j <= 2; j++){
			grid[z++] = raw[ line * SUDOKU_ROW_LENGTH_RAW + j ];
		}
		for(j = 4; j <= 6; j++){
			grid[z++] = raw[ line * SUDOKU_ROW_LENGTH_RAW + j ];
		}
		for(j = 8; j <= 10; j++){
			grid[z++] = raw[ line * SUDOKU_ROW_LENGTH_RAW + j ];
		}
	}
}

/**
 * @brief adds formatting characters to sudoku
 * @param raw: sudoku with formatting characters
 * @param grid: sudoku without formatting characters
 *
 */
void packSudoku(const uint8_t *grid, char *raw){
	uint8_t row, j, z=0;

	for(row = 0; row <= 11; row++){
		if( row==3 || row==7){
			strcpy(raw + row*SUDOKU_ROW_LENGTH_RAW , "-----------");
			raw[ row*SUDOKU_ROW_LENGTH_RAW + OFFSET_CR ] = CR;
			raw[ row*SUDOKU_ROW_LENGTH_RAW + OFFSET_LF ] = LF;
			continue;
		}

		for(j = 0; j <= SUDOKU_ROW_LENGTH_RAW-1; j++){
			if( j==3 || j==7){
				raw[ row*SUDOKU_ROW_LENGTH_RAW +j ] = '|';
			}else if(j==OFFSET_CR){
				raw[ row*SUDOKU_ROW_LENGTH_RAW +j ] = CR;
			}else if(j==OFFSET_LF){
				raw[row*SUDOKU_ROW_LENGTH_RAW +j ] = LF;
			}else{
				raw[ row*SUDOKU_ROW_LENGTH_RAW +j ] = grid[z++];
			}
		}
	}
}

int errorHandler(error_t e){
	printf("Error detected. Error code:%d\n", e);
	fflush(stdout);
	return 1;
}


void convertSudokuToNumbers(uint8_t *sudoku){
	int i;
	for(i = 0; i <= 80; i++){
		sudoku[i] -= ASCII_OFFSET;
	}
}

void convertSudokuToASCII(uint8_t *sudoku){
	int i;
	for(i = 0; i <= 80; i++){
		sudoku[i] += ASCII_OFFSET;
	}
}

