#include <stdint.h>
#include "sudoku.h"
#include "formatChecker.h"
#include "error.h"


uint8_t checkDataLine(const char *data);
uint8_t checkDividerLine(const char *data);
uint8_t isDigit(char c);

// checks proper formatting of sudoku
error_t formatChecker_checkSudoku(const char *sudoku){
	uint8_t check = ERR_OK;
	uint8_t i = 0;

	check += checkDataLine(&sudoku[SUDOKU_ROW_LENGTH_RAW * i++]);
	check += checkDataLine(&sudoku[SUDOKU_ROW_LENGTH_RAW * i++]);
	check += checkDataLine(&sudoku[SUDOKU_ROW_LENGTH_RAW * i++]);
	check += checkDividerLine(&sudoku[SUDOKU_ROW_LENGTH_RAW * i++]);
	check += checkDataLine(&sudoku[SUDOKU_ROW_LENGTH_RAW * i++]);
	check += checkDataLine(&sudoku[SUDOKU_ROW_LENGTH_RAW * i++]);
	check += checkDataLine(&sudoku[SUDOKU_ROW_LENGTH_RAW * i++]);
	check += checkDividerLine(&sudoku[SUDOKU_ROW_LENGTH_RAW * i++]);
	check += checkDataLine(&sudoku[SUDOKU_ROW_LENGTH_RAW * i++]);
	check += checkDataLine(&sudoku[SUDOKU_ROW_LENGTH_RAW * i++]);
	check += checkDataLine(&sudoku[SUDOKU_ROW_LENGTH_RAW * i++]);

	return check;
}


// check for sudoku divider ("-----------"CRLF)
// possibly bool enum
uint8_t checkDividerLine(const char *data){
	int i;
	for(i = 0 ; i <= 10; i++){
		if(data[i] != '-'){
			return ERR_GEN;
		}
	}

	if(data[OFFSET_CR] != CR){
		return ERR_GEN;
	}

	if(data[OFFSET_LF] != LF){
		return ERR_GEN;
	}

	return ERR_OK;
}

// check for sudoku data (3 digits, '|', 3 digits, '|', 3 digits, CR, LF)
// possibly bool enum
uint8_t checkDataLine(const char *data){

	// check digits
	uint8_t digitCheck = 1;
	digitCheck &= isDigit(data[0]);
	digitCheck &= isDigit(data[1]);
	digitCheck &= isDigit(data[2]);
	digitCheck &= isDigit(data[4]);
	digitCheck &= isDigit(data[5]);
	digitCheck &= isDigit(data[6]);
	digitCheck &= isDigit(data[8]);
	digitCheck &= isDigit(data[9]);
	digitCheck &= isDigit(data[10]);
	if(digitCheck == 0){
		return ERR_GEN;
	}

	// check formatting
	if(data[3] != '|' || data[7] != '|'){
		return ERR_GEN;
	}

	// check formatting
	if(data[11] != CR || data[12] != LF){
		return ERR_GEN;
	}

	return ERR_OK;

}

/*
 * checks if character is digit
 */
uint8_t isDigit(char c){
	if(c >= 48 && c <= 57){
		return 1;
	}
	return 0;
}
