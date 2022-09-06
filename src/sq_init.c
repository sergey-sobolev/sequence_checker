/** 
 * @file
 * @brief Initialization functions here.
 */
#include "sq_checker.h" // sq_errors  
#include "sq_init.h"
#include "sq_debug.h"   // assert
#include "sq_code.h"    // SQ_PATH
#include "sq_symtab.h"  // mem list funcs
#include "y.tab.h"      // INIT, SYNC, etc

/// suported keywords array 
static struct
{
   char *name;  //!< keyword name
   int   kval;  //!< keyword code (ID), used by the parser to process input
} keywords[] =
{
// main keywords types
   {"allowed", ALLOWED}, 
   {"executed", EXECUTED}, 
   {"#", FINAL}, 
   {"def", DEFINE},
   {(char*)0, 0}
};




/** Initializes internal structures. Fills in keywords and configures memory tracker (mem_list). 
 * \return Initialization result: 0 if no error.
 */
int sq_init( void )
{
    int i;
    SYMBOL *sym;
    
    allowed_path  = (SQ_PATH*)0;
    executed_path = (SQ_PATH*)0;
	sq_errors     = 0;
    init_mem_list();
    
    for( i = 0; keywords[i].name; i++ )
    {
        sym = sq_sym_install( keywords[i].name, keywords[i].kval, 0.0 );
        assert( sym ); // fatal error, cannot install symbol: abort execution
        //if ( ! sym )
        //{ // could not install symbol: cannot continue, it's better to exit now
        //    return 1;
        //}
    }
 
    return 0;
} // sq_init








/** Frees everything and finalizes everything.
 *
 */
void sq_cleanup( void )
{
    free_mem_list();
    allowed_path  = (SQ_PATH*)0;
    executed_path = (SQ_PATH*)0;
} // sq_cleanup
