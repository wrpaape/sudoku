#ifndef SUDOKU_SUDOKU_H
#define SUDOKU_SUDOKU_H
/* external dependencies
 * ────────────────────────────────────────────────────────────────────────── */
#include "file/file_utils.h"		/* open, read, write, close */
#include "string/string_utils.h"	/* string_length */
#include <stdlib.h>			/* exit */


/* macro constants
 * ────────────────────────────────────────────────────────────────────────── */
/* 9 cell rows + 8 line rows each having 9 segments of width 4 */
#define BOARD_LINE_LENGTH	(9 * 4)
#define BOARD_BUFFER_LENGTH	((9 + 8) * (9 * 4))


/* typedefs, struct declarations
 * ────────────────────────────────────────────────────────────────────────── */
struct Cell {
	unsigned int *restrict row;
	unsigned int *restrict col;
	unsigned int *restrict block;
	char *restrict token;
};

struct Board {
	struct Cell *restrict rem_cells;
	const struct Cell *restrict cells_until;
	struct Cell cells[9 * 9];
	unsigned int rows[9];
	unsigned int cols[9];
	unsigned int blocks[9];
	char buffer[BOARD_BUFFER_LENGTH + 1]; /* '\0' */
};


/* helpful macros
 * ────────────────────────────────────────────────────────────────────────── */
#define IS_ASCII_DIGIT(TOKEN)	(((TOKEN) <= '9') && ((TOKEN) >= '0'))

/* 0..9 → '0'..'9' */
#define DIGIT_TO_ASCII(DIGIT)	((char) (((unsigned int) (DIGIT)) | 48u))

/* '0'..'9' → 0..9 */
#define ASCII_TO_DIGIT(TOKEN)	(((unsigned int) (TOKEN)) & 15u)

/* 1 << 0..9 → '0'..'9' */
#define MOVE_TO_ASCII(MOVE)	DIGIT_TO_ASCII(__builtin_ctz(MOVE))

/*  '0'..'9' → 1 << 0..9 */
#define ASCII_TO_MOVE(TOKEN)	(1 << ASCII_TO_DIGIT(TOKEN))



/* helper functions
 * ────────────────────────────────────────────────────────────────────────── */
static inline void
exit_on_failure(const char *const restrict failure)
__attribute__((noreturn));

static inline void
init_row(struct Cell *restrict *const restrict cell_ptr,
	 unsigned int *const restrict row,
	 unsigned int *restrict col,
	 unsigned int *restrict block,
	 unsigned int *const restrict *restrict block_col,
	 unsigned int *const restrict *const restrict block_cols_last,
	 char *restrict *const restrict buffer_ptr);

static inline void
init_board(void);

static inline void
print_board(void);

static inline void
print_failed_to_solve(void);

#endif /* ifndef SUDOKU_SUDOKU_H */

