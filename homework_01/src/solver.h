#ifndef SOLVER_H_
#define SOLVER_H_

#include <stdint.h>
#include "error.h"

error_t solver_solveSudoku(uint8_t *Grid);

#endif /* SOLVER_H_ */
