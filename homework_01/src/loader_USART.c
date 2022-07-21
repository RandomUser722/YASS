#include <stdint.h>
#include "loader.h"
#include "sudoku.h"

#define BUFFER_LENGHT 500

volatile uint8_t buffer[BUFFER_LENGHT];
volatile uint16_t readIndex = 0;
volatile uint16_t writeIndex = 0;

void copySudoku(uint8_t *sudoku);
uint8_t sudokuReceived(void)

error_t loader_loadRawSudoku(char *sudoku_raw, uint8_t len){

	if(len < SUDOKU_LENGTH_RAW){
		return ERR_SIZE;
	}

	if(sudokuReceived()){
		copySudoku(sudoku_raw);
	}
	return ERR_OK;
}

// Funkce pro prijem ze seriove linky.
void USART2_IRQHandler(void)
{
	if(USART2->ISR & USART_ISR_RXNE)
	{
		if(writeIndex == readIndex){
			// @todo handle overflow
			return;
		}
		buffer[writeIndex] = USART2->RDR;
		++writeIndex %= BUFFER_LENGHT;

	}
}

/*
 * checks if count of received characters is equal or bigger than size of one sudoku
 */
uint8_t sudokuReceived(void){
	uint16_t receivedLen;

	if(writeIndex >= readIndex){
		receivedLen = writeIndex - readIndex;
	}else{
		receivedLen = BUFFER_LENGHT - readIndex + writeIndex;
	}

	if(receivedLen >= SUDOKU_LENGTH_RAW){
		return ERR_OK;
	}

	return ERR_NED;
}

// Move one sudoku from USART circular buffer to sudoku array
void copySudoku(uint8_t *sudoku){
	int i;
	for(i = 0; i < SUDOKU_LENGTH_RAW; i++){
		sudoku[i] = buffer[readIndex];
		++readIndex;
		readIndex %= BUFFER_LENGHT;
	}
}

