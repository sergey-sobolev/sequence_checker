/** 
 * @file
 * @brief Paths manipulations types definitions and functions prototypes.
 */

#ifndef SQ_CODE_HDR
#define SQ_CODE_HDR

#include <memory.h>    // NULL
#include "sq_symtab.h" // SYMBOL typedef

#define ST_FINAL_POINT 1

typedef struct _sq_state  SQ_STATE;
typedef struct _sq_sequence SQ_SEQUENCE;
typedef struct _sq_path   SQ_PATH;
typedef struct _sq_paths  SQ_PATHS;

struct _sq_state      //!< state structure
{
    int      id;      //!< state identifier
    int      opt;     //!< state options: FINAL point and others
    double   timeout; //!< timeout for the state
    SQ_PATH *path;    //!< downlink to the path which this state belongs to
};

struct _sq_sequence     //!< sequence list: for entry and end points
{
    SQ_STATE  *state; //!< pointer to current state
    SQ_SEQUENCE *next;  //!< pointer to the next item in the list
    SQ_SEQUENCE *prev;  //!< pointer to the previous item
};

struct _sq_path       //!< path structure
{
    SQ_SEQUENCE *entp;  //!< entry points for this path
    SQ_SEQUENCE *endp;  //!< end points for this path
    SQ_PATHS  *paths; //!< possible successors of the path
};

struct _sq_paths      //!< paths list: for entry and end points
{
    SQ_PATH  *path;   //!< pointer to current path
    SQ_PATHS *next;   //!< pointer to the next item in the list
    SQ_PATHS *prev;   //!< pointer to the previous item
};


// Functions declarations

SQ_PATH*   sq_link_paths( SQ_PATH*, SQ_PATH*);

SQ_STATE*  sq_create_state( int type );

SQ_PATH*   sq_create_new_path( SQ_STATE* state );

int        sq_finalize_allowed_path( SQ_PATH* ap );

int        sq_check_executed_path( SQ_PATH* ep );

SQ_PATH*   sq_set_state_timeout( SQ_PATH* state, double timeout );

SQ_SEQUENCE* sq_append_list_with_state( SQ_SEQUENCE *list, SQ_STATE *state );

SQ_SEQUENCE* sq_merge_lists( SQ_SEQUENCE *list1, SQ_SEQUENCE *list2 );

SQ_PATH*   sq_merge_paths( SQ_PATH *path1, SQ_PATH *path2 );

SQ_PATHS*  sq_create_new_paths_item( SQ_PATH *path );

SQ_STATE*  sq_find_state_by_id( SQ_SEQUENCE *sequence, int type, double timeout );

SQ_PATH*   sq_find_path_by_id( SQ_PATHS *paths, int type, double timeout );

SQ_PATH*   sq_define_new_state( SYMBOL *st_name );


// Externed variables
extern SQ_PATH *allowed_path;   
extern SQ_PATH *executed_path;  

#endif // SQ_CODE_HDR
