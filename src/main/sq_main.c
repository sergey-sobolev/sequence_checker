/** 
 * @file
 * @brief Main sequence checker function implementation.
 */    
#include "sq_checker.h"
#include "sq_init.h"    // sq_init, sq_cleanup
#include "sq_parser.h"
#include "sq_options.h" // process_options, file name




/** 
* @brief Main function of the sequence checker.
* 
* @param argc input arguments number
* @param argv array of input arguments
* 
* @return error code, zero if input verification passed.
*/
int main( int argc, char *argv[] )
{
    int res;
    res = process_options( argc, argv );
    if ( res < 0 )
        return 0;
    else if ( res > 0 )
        return 1;    

	sq_init(); // initializae symbol table and other stuff
    
	if ( sq_cfg_name[0] )
		res = sq_parser( sq_cfg_name );

	if ( !res )
		res = sq_parser( sq_file_name ); 

	sq_cleanup(); // release memory    
    return res;     
} // main







