#ifndef	C_STMT_LR_PARSE_H
#include "prod_entry.h"
#define	MAX_TAB_SIZE 300
#define	MAX_PROD_TAB_SIZE 100
#define	ACCEPT 10000
#define	ERROR  0
#ifdef __cplusplus
extern "C"
{
#endif
extern prod_entry prod_table[MAX_PROD_TAB_SIZE];
extern int action_table[MAX_TAB_SIZE][MAX_TAB_SIZE];
extern int goto_table[MAX_TAB_SIZE][MAX_TAB_SIZE];
extern char *tname[];
#ifdef __cplusplus
}
#endif


#define	C_STMT_LR_PARSE_H
#endif
