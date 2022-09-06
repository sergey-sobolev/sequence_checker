/** 
 * @file
 * @brief Functions for manipulating with symbol table.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sq_init.h"
#include "sq_symtab.h"
#include "sq_code.h"
#include "sq_parser.h"

static SYMBOL   *symlist = NULL; //!< pointer to the symbol table
static MEM_LIST  mem_list;       //!< allocated objects list, used for memory release
static MEM_LIST *pm_cur;         //!< pointer to the current memory list item

static unsigned  varid;          //!< unique identifier of a symbol (variable)

// memory allocation/freeing statistics
unsigned int allocated;          //!< number of memory allocations, initialized in sq_init
unsigned int freed;              //!< number of memory freeings, initialized in sq_init






/** Allocates given number of bytes from heap. 
 *  Also it fills in memory tracker list new entry: all memory allocated by this function will be
 *  automatically freed by free_mem_list function.
 * \param n number of bytes to allocate
 * \return Pointer to the first address of allocated memory.
 */ 
char * emalloc( unsigned n )
{
    MEM_LIST *pm = NULL;
    char *p = NULL;

    if( mem_list.ptr != NULL )
        pm = malloc( sizeof( MEM_LIST ) );
    else
        pm = &mem_list;

    if( pm == NULL ) 
        goto mem_alloc_error;

    p = (char *)malloc( n );

    if( p == 0 ) 
        goto mem_alloc_error;

    allocated++;

    pm->ptr  = p;
    if( pm != &mem_list )
        pm->prev = pm_cur;
    pm_cur   = pm;
    return( p );

mem_alloc_error:
    fprintf( stderr, "ERROR: out of memory\n" );
    exit( 1 );
	return 0;
} // emalloc







/** Looks for symbol with the given integer type and returns its name
  * \param id symbol id
  * \return symbol name
  */
char * sq_id_to_str( int id )
{
    SYMBOL *sp;

    for ( sp = symlist; sp != NULL; sp = sp->next )
    {
        if ( sp->id == id )
        {
            return sp->name;
        }
    }
    return "UNKNOWN";
} // sq_id_to_str






/** Looks up for a symbol with the given name.
 * \param s symbol identifier (name)
 * \return Pointer to the symbol if found, 0 otherwise
 */
SYMBOL *sq_sym_lookup( char *s )
{
   SYMBOL *sp;

   for( sp = symlist; sp != (SYMBOL *)0; sp = sp->next )
   {
      if( strcmp( sp->name, s ) == 0 ) 
          return( sp );
   }
   return( 0 );  /* item not found */
} // sq_sym_lookup







/** Installs new symbol into symbol table
 * \param s symbol identifier
 * \param t symbol type
 * \param d symbol numeric value
 * \return Pointer to the newly installed (created) symbol.
 */
SYMBOL *sq_sym_install( char *s, int t, double d )
{
    SYMBOL *sp;

    sp = (SYMBOL *)emalloc( sizeof( SYMBOL ) );
    sp->name  = emalloc( (unsigned)strlen( s ) + 1 );

    strcpy( sp->name, s );
    sp->id    = varid;    // fill in variable id
    sp->type  = t;
    sp->u.val = d;
    sp->next  = symlist;  /* alloc in begin of list */
    symlist   = sp;

    varid++;              // all variables must have *unique* identifier
    return( sp );
} // sq_sym_install







/** Initializes memory tracker.
 *
 */
void init_mem_list(void)
{
    varid         = 0;
    allocated     = 0;
    freed         = 0; 
    mem_list.ptr  = NULL;
    mem_list.prev = NULL;
    pm_cur        = &mem_list;
    symlist       = NULL;
} // init_mem_list







/** Frees memory allocated by emalloc.
 *
 */
void free_mem_list()
{
    MEM_LIST *pm = pm_cur;

    if( pm_cur != &mem_list )
    {
        while( pm->prev )
        {
            free( pm->ptr );
            pm_cur = pm_cur->prev;
            free( pm );
            pm = pm_cur;

            freed++;
        };
    };

    if( mem_list.ptr != NULL )
    {
        free( mem_list.ptr );
        freed++;
    }

    mem_list.ptr = NULL;
} // free_mem_list 

