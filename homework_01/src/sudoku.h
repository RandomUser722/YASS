#ifndef SUDOKU_H_
#define SUDOKU_H_

#define CR 13
#define LF 10
#define OFFSET_CR 11
#define OFFSET_LF 12
#define SUDOKU_ROW_LENGTH 9 // Velikost strany sudoku pocitaje pouze cislice
#define SUDOKU_ROW_LENGTH_RAW (SUDOKU_ROW_LENGTH+4) // 9 digits, 2 '|', CR and LF signs
#define SUDOKU_LENGTH_RAW ( SUDOKU_ROW_LENGTH_RAW * 11 ) // 9 lines of data, 2 lines of divider

#endif /* SUDOKU_H_ */
