/** 
 * @file
 * @brief States parser constants and functions prototypes.
 */

#ifndef SQ_PARSER_HDR
#define SQ_PARSER_HDR

#define MAX_FN_LEN 256

int  sq_parser( char *input_fn );
void yyerror( char *str );
int	 yylex( void );

#endif // SQ_PARSER_HDR
