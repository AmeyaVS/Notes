#ifndef	C_STMT_HDR_H

extern int done;
extern char input_str[];
int dbg_printf (const char *fmt, ...);

#ifdef	GENERATED_PARSER
	#ifdef	CHAP3_EX1 
		/* Example 1 */
		#include "c-stmt-gram.h"
	#else
		/* Example 6 */
		#include "c-stmt-bot-up-gram.h"
	#endif
#else
#define	TOK_BASE	256
#define SEMI_COLON      256
#define CONSTANT        257
#define EQ_TO_OP        258
#define IDENTIFIER      259
#define OPERATOR        260
#endif

/* All the Non Terminals - ordered lexigographically */
#define C_EXPR_FACTOR   1
#define C_EXPR_REST 	2
#define C_EXPR 	        3
#define	C_STMT	        4

#define C_STMT_HDR_H
#endif
