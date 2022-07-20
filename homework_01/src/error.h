
#ifndef ERROR_H_
#define ERROR_H_

//#define ERR_OK 0	// no error
//#define ERR_GEN 1	// generic error
//#define ERR_NED 2	// Not Enough Data

enum error{
	ERR_OK=0,
	ERR_GEN,	// unspecified error
	ERR_NED,	// not enough data
	ERR_SIZE,
	ERR_FILE,	// file error
	ERR_SOLUTION	// sudoku not solved
};

typedef enum error error_t;

#endif /* ERROR_H_ */
