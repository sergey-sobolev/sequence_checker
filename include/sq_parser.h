/** 
 * @file
 * @brief States parser constants and functions prototypes.
 */

#ifndef SC_PARSER_HDR
#define SC_PARSER_HDR

#define MAX_FN_LEN 256

int  sq_parser( char *input_fn );
void yyerror( char *str );
int	 yylex( void );

#endif // SC_PARSER_HDR
