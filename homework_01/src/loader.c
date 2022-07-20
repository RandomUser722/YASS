#include <stdio.h>
#include <stdint.h>
#include "sudoku.h"
#include "loader.h"

error_t loader_loadRawSudoku(char *sudoku_raw, uint8_t len){
	int count;

	if(len < SUDOKU_LENGTH_RAW){	//@todo
		return ERR_SIZE;
	}

	for(count = 0; count <= 10; count++){
		while(fgets( sudoku_raw + 13*count, 20, stdin) == NULL);
		sudoku_raw[SUDOKU_ROW_LENGTH_RAW*count +OFFSET_CR ] = CR;
		sudoku_raw[SUDOKU_ROW_LENGTH_RAW*count +OFFSET_LF ] = LF;
	}

	return 0;
}
