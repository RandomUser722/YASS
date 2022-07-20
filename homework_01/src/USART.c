
#include <stdio.h>
#include "USART.h"


uint8_t USART_PrintSudokuGrid(const char *Grid, uint8_t RowLength){
	printf("%s", Grid);
	fflush(stdout);
	return 0;
}
