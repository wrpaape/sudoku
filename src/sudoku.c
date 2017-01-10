#include "sudoku.h"

static struct Board board;

/* helper functions
 * ────────────────────────────────────────────────────────────────────────── */
static inline void
exit_on_failure(const char *const restrict failure)
{
	file_write_muffle(STDERR_FILENO,
			  failure,
			  string_length(failure));
	exit(1);
	__builtin_unreachable();
}

static inline void
read_board(void)
{
	int file_descriptor;
	const char *restrict failure;

	if (LIKELY(file_open_report(&file_descriptor,
				    BOARD_PATH,
				    O_RDONLY,
				    &failure))) {

		if (LIKELY(file_read_report(file_descriptor,
					    &board.buffer[0],
					    BOARD_BUFFER_LENGTH,
					    &failure))) {

			if (LIKELY(file_close_report(file_descriptor,
						     &failure)))
				return;

		} else {
			file_close_muffle(file_descriptor);
		}
	}

	exit_on_failure(failure);
	__builtin_unreachable();
}

static inline void
init_row(struct Cell *restrict *const restrict cell_ptr,
	 unsigned int *const restrict row,
	 unsigned int *restrict col,
	 unsigned int *restrict block,
	 unsigned int *const restrict *restrict block_col,
	 unsigned int *const restrict *const restrict block_cols_until,
	 char *restrict *const restrict buffer_ptr)
{
	unsigned int move;
	char token;
	char *restrict buffer;
	struct Cell *restrict cell;

	cell   = *cell_ptr;
	buffer = *buffer_ptr;

	while (1) {
		token = *buffer;

		if (!IS_ASCII_DIGIT(token))
			exit_on_failure("improper board format\n");

		if (token == '0') {
			cell->row   = row;
			cell->col   = col;
			cell->block = block;
			cell->token = buffer;
			++cell;

		} else {
			move = ASCII_TO_MOVE(token);

			*row   |= move;
			*col   |= move;
			*block |= move;
		}

		++col;
		if (col == *block_col) {
			++block_col;
			if (block_col == block_cols_until)
				break;

			++block;
		}

		buffer += 4;
	}

	*cell_ptr   = cell;
	*buffer_ptr = buffer;
}

static inline void
init_board(void)
{
	struct Cell *restrict cell;
	unsigned int *restrict row;
	unsigned int *restrict col;
	unsigned int *restrict block;
	char *restrict buffer;
	unsigned int *const restrict *restrict block_row;

	static unsigned int *const block_cols[3] = {
		&board.cols[3],
		&board.cols[6],
		&board.cols[9]
	};

	static unsigned int *const restrict *const restrict block_cols_until
	= &block_cols[3];

	static unsigned int *const block_rows[3] = {
		&board.rows[3],
		&board.rows[6],
		&board.rows[9]
	};

	static unsigned int *const restrict *const restrict block_rows_until
	= &block_rows[3];


	read_board();

	cell = &board.cells[0];
	board.rem_cells = cell;

	row    = &board.rows[0];
	col    = &board.cols[0];
	block  = &board.blocks[0];
	buffer = &board.buffer[1];

	block_row = &block_rows[0];

	while (1) {
		init_row(&cell,
			 row,
			 col,
			 block,
			 &block_cols[0],
			 block_cols_until,
			 &buffer);

		++row;
		if (row == *block_row) {
			++block_row;
			if (block_row == block_rows_until)
				break;

			block += 3;
		}

		buffer += (BOARD_LINE_LENGTH + 4);
	}

	board.cells_until = cell;
}

static inline void
print_board(void)
{
	const char *restrict failure;

	if (LIKELY(file_write_report(STDOUT_FILENO,
				     &board.buffer[0],
				     BOARD_BUFFER_LENGTH,
				     &failure)))
		return;

	exit_on_failure(failure);
	__builtin_unreachable();
}

static inline void
print_failed_to_solve(void)
{
	const char *restrict failure;

	if (LIKELY(file_write_report(STDOUT_FILENO,
				     "failed to solve\n",
				     sizeof("failed to solve\n"),
				     &failure)))
		return;

	exit_on_failure(failure);
	__builtin_unreachable();
}


bool
solve_board(void)
{
	unsigned int move;

	/* if no more cells, must have solved */
	if (board.rem_cells == board.cells_until)
		return true;

	/* fetch next cell */
	struct Cell *const restrict cell = board.rem_cells;
	++board.rem_cells;

	/* fetch set of available numbers */
	const unsigned int rem_moves = ~(  *(cell->row)
					 | *(cell->col)
					 | *(cell->block));

	/* for moves 1 ... 9 */
	move = 1 << 1;

	do {
		/* check if move is legal */
		if (rem_moves & move) {
			/* remove 'move' from member row, column, and block */
			*(cell->row)   |= move;
			*(cell->col)   |= move;
			*(cell->block) |= move;

			if (solve_board()) {
				/* board solved, set token in display */
				*(cell->token) = MOVE_TO_ASCII(move);
				return true;
			}

			/* 'move' produced a failed branch, undo */
			*(cell->row)   ^= move;
			*(cell->col)   ^= move;
			*(cell->block) ^= move;
		}

		move <<= 1;
	} while (move <= (1 << 9));

	/* failed to solve in this branch */
	--board.rem_cells;
	return false;
}


/* program entry
 * ────────────────────────────────────────────────────────────────────────── */
int
main(void)
{
	init_board();

	print_board();

	if (solve_board())
		print_board();
	else
		print_failed_to_solve();

	return 0;
}
