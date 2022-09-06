/** 
* @file
* @brief Command line options parser here.
*/
#include <string.h>     // sprintf, strcpy
#include "sq_debug.h"   // DEBUG
#include "sq_checker.h" // SQ_VERSION
#include "sq_options.h"

char sq_file_name[256] = {0}; //!< input file name container, initialized in process_options
char sq_cfg_name[256] = {0};  //!< configuration file name container, can be used to separate definitions from paths
int  verbose_output;          //!< verbosity level, set in command line by options -v and -vv (full) 

/** This function performs command line arguments processing
 * \param argc number of arguments
 * \param argv array of arguments
 * \return if no error 0, otherwise error code
 */
int process_options( int argc, char *argv[] )
{
    int argn = 1;
    extern int yydebug;

    // set defaults
    yydebug = 0;
    verbose_output = 0;

    if ( argc == 1 )
    {
        D2( printf( "no parameters specified, using stdin\n" ) );    
        return 0; // use STDIN by default
    }
    
    while ( argn < argc )
    {
        if ( strcmp( argv[argn], "--version" ) == 0 )
        {
            printf( "States checker, version %s\n", SQ_VERSION );
            return -1; // print version and exit
        }
        
        if ( (strcmp( argv[argn], "/?" ) == 0) || (strcmp( argv[1], "--?" ) == 0) )
        {
            printf( "Specify file name to be checked\n" );
            return -2; // print help and exit
        }

        if ( (strcmp( argv[argn], "-d" ) == 0 ) )
        { // debug output enabled
            extern int yydebug;
            yydebug = 1;
            argn++;
            continue;
        }
	
		if ( (strcmp( argv[argn], "-cfg" ) == 0 ) )
		{
			if ( argn == argc-1 )
			{
				fprintf( stderr, "File name must be specified for this option" );
				return -3;
			}
			D( printf( "Configuration file name: %s\n", argv[argn+1] ) );
			strcpy( (char*)sq_cfg_name, (char*)argv[argn+1] );
			argn += 2;
			continue;
		}

        if ( (strcmp( argv[argn], "-v" ) == 0) )
        {
            verbose_output = 1;
            argn++;
            continue;
        }
        
        if ( (strcmp( argv[argn], "-vv" ) == 0) )
        {
            verbose_output = 2;
            argn++;
            continue;
        }
        D( printf( "file name: %s\n", argv[argn] ) );
        strcpy( (char*)sq_file_name, (char*)argv[argn] );
        argn++;
    } // end of while loop
    return 0;
} // process_options
