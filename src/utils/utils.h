#ifndef SUDOKU_UTILS_UTILS_H_
#define SUDOKU_UTILS_UTILS_H_

/* external dependencies
 * ────────────────────────────────────────────────────────────────────────── */
#include <stdbool.h>	/* bool */

/* helper macros
 * ────────────────────────────────────────────────────────────────────────── */
#define LIKELY(BOOL)   __builtin_expect(BOOL, true)
#define UNLIKELY(BOOL) __builtin_expect(BOOL, false)

#endif /* ifndef SUDOKU_UTILS_UTILS_H_ */
