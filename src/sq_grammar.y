/** @cond */
%{
/** @endcond */    

/** 
 * @file sq_grammar.y
 * @brief Language grammar description here. This file is processed by YACC/Bison and results to the language parser.
 * \note Grammar specification format is not supported by Doxygen, so sections delimiters and the grammar itsel are protected by conditional processing (Doxygen cond/endcond commands).
 */    
/** 
 * @file sq_parser.c
 * @brief Parser source code generated by YACC/Bison from the grammar file sq_grammar.y.
 */    

// States checker parser

#include <stdio.h>     // FILE, stdin
#include <string.h>    // strcpy
#include <ctype.h>     // islower
#include <memory.h>	   // malloc
#include "sq_debug.h"
#include "sq_checker.h"
#include "sq_init.h"
#include "sq_symtab.h"    
#include "sq_code.h"
#include "sq_parser.h"

#define YYDEBUG 1

/** \cond */
%}

%union
{
    SQ_PATH *path;
    SYMBOL  *sym;
    double   dnum;
}

%token <dnum> NUMBER
%token <sym>  DEFINE STRING
%token <path> FINAL VARSTATE
%token <path> ALLOWED EXECUTED
%type  <path> allowed_path state executed_path path definition
%right '='
%right TRANSITION
%right OR

%start list

%%

list : /* nothing */
    | list '\n'
    | list definition ';'
    | list allowed_path ';'  { sq_finalize_allowed_path( $2 ); return( 1 ); }
    | list executed_path ';' { sq_check_executed_path( $2 ); return ( 1 ); }
    ;

allowed_path : ALLOWED '=' path { $$ = $3; }
         ;

executed_path : EXECUTED '=' path { $$ = $3; }
         ;

path :     state
         | '[' path ']'  { $$ = $2; }
         | state '(' NUMBER ')' { $$ = sq_set_state_timeout( $1, $3 ); }
         | path TRANSITION path { $$ = sq_link_paths( $1, $3 ); }
         | path OR path { $$ = sq_merge_paths( $1, $3 ); }
         ;
   
state :    VARSTATE 
         | FINAL
         ;
  
definition: DEFINE STRING { $$ = sq_define_new_state( $2 ); }
         ;         

%%
/* END OF GRAMMAR */         
/** \endcond */

FILE *input_file;                   //!< input file handler 
char  input_file_name[MAX_FN_LEN];  //!< input file name, can be NULL for stdin

int   lineno    = 1;                //!< current line number, used for error and warning messages
int   tokenno   = 0;                //!< token number, used for error and warning messages
int   sq_errors = 0;                //!< global error detection flag, used by parser to report about detected errors in the input data

/** 
* @brief Prints warning to stderr
* 
* @param s warning message
* @param t comment for the message (can be empty)
*/
void warning( char *s, char *t )
{
    fprintf( stderr, "%s", s );
    if ( t )
        fprintf( stderr, "%s", t );
    fprintf( stderr, ", file %s, near line %d, token num: %d\n", input_file_name, lineno, tokenno );   
} // warning








/** 
* @brief Prints error message, called when further parsing is not possible
* 
* @param s message text
*/
void yyerror( char *s )
{
    warning( s, (char*) 0 );
    if ( input_file )
        fclose( input_file );
    sq_cleanup();    
    exit(2); // 2 - as for errno 2 - file not found - incorrect input
} // yyerror








/** 
* @brief Checks next symbol if it is as expected or not.
* Returns ifyes value if next symbol is expected and ifno otherwise.
* @param expect expected symbol
* @param ifyes  what to return if next symbol is as expected
* @param ifno   what to return if next symbol is not as expected
* 
* @return either ifyes or ifno
*/
int follow( int expect, int ifyes, int ifno )
{
    FILE *f = input_file;
    register int   c;

    c = getc( f );

    if( c == expect ) 
        return( ifyes );
    ungetc( c, f );
    return( ifno );
} // follow








/** 
* @brief Checks if input symbol makes a known escape-code
* Converts text "\n" to symbol \n
* @param c symbol which follows backslash in the source code
* 
* @return decoded escape code
*/
int backslash( int c )
{
    FILE *f = input_file;
    static   char  tabl[] = "b\bf\fn\nr\rt\t";
    char *p = (char *)0;

    if( c != '\\' ) 
        return( c );

    c = getc( f );
    p = (char *)strchr( tabl, c ); /* or strchr() */

    if( islower( c ) && p != (char *)0 )
        return( p[1] );
    
    return( c );
} // backslash








/** 
* @brief Lexems scanner function
* Parses input file and returns token codes.
*
* @return Token code
*/
int yylex( void )
{
    FILE *f = input_file;
    register int   c;

    // let \n pass through the filter: it is used in rules
    while( (c = getc( f )) == ' ' || c == '\t' || c == '\r' );

    if( c == EOF )
    {
         return( 0 );
    }

    if( c == '.' || isdigit( c ) )
    {
        double d = 0.0;
        int n = getc( f );

        /* it mustn't be number "." for scanf */
        if( c == '.' && !isdigit( n ) )
        {
            ungetc( n, f );
            yylval.dnum = d;
            return( NUMBER );
        }
        ungetc( n, f );

        /* number */
        ungetc( c, f );
        n = fscanf( f, "%lf", &d );
        yylval.dnum = d;

        return( NUMBER );
    }
    if( (c == '_') || isalpha( c ) || c == '#' )
    {
        SYMBOL   *sp;
        SQ_STATE *st;
        char sbuf[256], *p = sbuf;

        do
        {
            *p = c;
            p++;
        } while( (c=getc( f )) != EOF && (isalnum( c ) || (c == '_')) );

        ungetc( c, f );
        *p = '\0';

        if( (sp = sq_sym_lookup( sbuf ) ) == (SYMBOL *)0 )
        {
            // didn't find the symbol: return the string
            D2( printf( "Got string: %s\n", sbuf ) );
            yylval.sym = (SYMBOL *)emalloc( (unsigned) strlen( sbuf ) + 1 );             
            strcpy( (char *)yylval.sym, sbuf );
            return( STRING );
        }
        else
        {
            D2( printf( "Got symbol %d(%s)\n", sp->id, sbuf ) );
            yylval.sym = sp;
            //if ( (sp->type == VARSTATE) || (sp->type == FINAL) )
            {
                st = sq_create_state( sp->id );
                if ( sp->type == FINAL )
					st->opt = ST_FINAL_POINT;
                yylval.path = sq_create_new_path( st );
            }
            return( sp->type );
        }

    }

    if( c == '\n' ) { lineno++; return( c ); }


    switch( c )
    {
        case '|': 
            return OR;
        case '#':
            return FINAL;    
        case '/': 
            if( (c = follow( '/', 'c', c )) == 'c' ) // 'c' - means comment
            {
                while( (c=getc( f )) != EOF && c != '\n' );
                if( c == '\n' ) { lineno++; }
                return '\n';
            }
            return (c);
        case '-':
            if ( (c = follow( '>', TRANSITION, c)) == TRANSITION )
                return TRANSITION;
            return c;        
        default : 
            return( c );
    }

} // yylex 







/** 
* @brief Main parser function.
* It parses input file (can be stdin) and checks data for correctness (if required)
* @param fname input file name, stdin if empty
* 
* @return Non-zero if errors are detected, zero if okay.
*/
int sq_parser( char *fname )
{
    #if YYDEBUG > 0
    // yydebug = 0; // can be set in options (-d)
    #endif

    if ( ! fname | ! fname[0] )
    {   // use stdin then
        D2( printf( "using stding as input\n" ) );
        strcpy( input_file_name, "stdin" );
        input_file = stdin;
    }
    else
    {    
        if ( strlen( fname ) > MAX_FN_LEN )
        {
            fprintf( stderr, "Too long file name, sorry. Maximum %d\n", MAX_FN_LEN );
            return 3;
        }

        strcpy( input_file_name, fname );
        if ( (input_file = fopen( input_file_name, "rb" )) == NULL )
        {
            fprintf( stderr, "Cannot open file %s", input_file_name );
            return 1;
        }
    }

    //sq_init(); // initialization and deinitialization is called by the main function

    while ( yyparse() > 0 ); // call until not finished

    D( printf( "File parsed ok, leaving parser\n" ) );
    fclose( input_file );
    input_file = NULL;
    
    //sq_cleanup();
    return sq_errors;
} // sq_parser
