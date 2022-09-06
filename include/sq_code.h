/** 
 * @file
 * @brief Paths manipulations types definitions and functions prototypes.
 */

#ifndef SC_CODE_HDR
#define SC_CODE_HDR

#include <memory.h>    // NULL
#include "sq_symtab.h" // SYMBOL typedef

#define ST_FINAL_POINT 1

typedef struct _sq_state  SC_STATE;
typedef struct _sq_sequence SC_SEQUENCE;
typedef struct _sq_path   SC_PATH;
typedef struct _sq_paths  SC_PATHS;

struct _sq_state      //!< state structure
{
    int      id;      //!< state identifier
    int      opt;     //!< state options: FINAL point and others
    double   timeout; //!< timeout for the state
    SC_PATH *path;    //!< downlink to the path which this state belongs to
};

struct _sq_sequence     //!< sequence list: for entry and end points
{
    SC_STATE  *state; //!< pointer to current state
    SC_SEQUENCE *next;  //!< pointer to the next item in the list
    SC_SEQUENCE *prev;  //!< pointer to the previous item
};

struct _sq_path       //!< path structure
{
    SC_SEQUENCE *entp;  //!< entry points for this path
    SC_SEQUENCE *endp;  //!< end points for this path
    SC_PATHS  *paths; //!< possible successors of the path
};

struct _sq_paths      //!< paths list: for entry and end points
{
    SC_PATH  *path;   //!< pointer to current path
    SC_PATHS *next;   //!< pointer to the next item in the list
    SC_PATHS *prev;   //!< pointer to the previous item
};


// Functions declarations

SC_PATH*   sq_link_paths( SC_PATH*, SC_PATH*);

SC_STATE*  sq_create_state( int type );

SC_PATH*   sq_create_new_path( SC_STATE* state );

int        sq_finalize_allowed_path( SC_PATH* ap );

int        sq_check_executed_path( SC_PATH* ep );

SC_PATH*   sq_set_state_timeout( SC_PATH* state, double timeout );

SC_SEQUENCE* sq_append_list_with_state( SC_SEQUENCE *list, SC_STATE *state );

SC_SEQUENCE* sq_merge_lists( SC_SEQUENCE *list1, SC_SEQUENCE *list2 );

SC_PATH*   sq_merge_paths( SC_PATH *path1, SC_PATH *path2 );

SC_PATHS*  sq_create_new_paths_item( SC_PATH *path );

SC_STATE*  sq_find_state_by_id( SC_SEQUENCE *sequence, int type, double timeout );

SC_PATH*   sq_find_path_by_id( SC_PATHS *paths, int type, double timeout );

SC_PATH*   sq_define_new_state( SYMBOL *st_name );


// Externed variables
extern SC_PATH *allowed_path;   
extern SC_PATH *executed_path;  

#endif // SC_CODE_HDR
