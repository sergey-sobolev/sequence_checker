/** 
 * @file
 * @brief Functions for manipulations with paths.
 */
#include "sq_debug.h"
#include "sq_checker.h" // sq_errors
#include "sq_code.h"
#include "sq_symtab.h"
#include "y.tab.h"     // VARSTATE


SQ_PATH *allowed_path;  //!< pointer to the allowed path description, set in sq_finalize_allowed_path
SQ_PATH *executed_path; //!< pointer to the executed path description, set in sq_check_executed_path


/** Adds state to the sequence list
 * \param list pointer to the sequence list. If list is NULL a new one will be created.
 * \param state pointer to the state
 * \return modified list
 * \note list is modified!
 */
SQ_SEQUENCE* sq_append_list_with_state( SQ_SEQUENCE *list, SQ_STATE *state )
{
    SQ_SEQUENCE *new_elem;
    SQ_SEQUENCE *s;

    if ( !state )
        return list;

    new_elem = (SQ_SEQUENCE*)emalloc( sizeof(SQ_SEQUENCE) );
    new_elem->state = state;
    new_elem->next = new_elem->prev = NULL;

    if ( !list )
    {
        return new_elem;
    }

    s = list;
    while ( s->next ) 
        s = s->next;
    s->next = new_elem;
    new_elem->prev = s;
             
    return list;
} // sq_append_list_with_state








/** Merges two sequence lists
 * \param list1 first list
 * \param list2 second list
 * \return merged list
 * \note list1 is modified!
 */
SQ_SEQUENCE* sq_merge_lists( SQ_SEQUENCE *list1, SQ_SEQUENCE *list2 )
{
    SQ_SEQUENCE *el;

    if ( !list1 )
        return list2;
    
    if ( !list2 )
        return list1;
    
    el = list1;
    
    while ( el->next )
        el = el->next;
    
    el->next = list2;
    list2->prev = el;         
    
    return list1;
} // sq_merge_lists








/** Merges two paths: joins entry and end points. This function performs A|B tuning.
 * \param path1 first path
 * \param path2 second path
 * \return merged path (a new one)
 */
SQ_PATH *sq_merge_paths( SQ_PATH *path1, SQ_PATH *path2 )
{
    SQ_PATH *merged_path = sq_create_new_path( NULL );
    SQ_PATHS *pso; //< pointer to paths element of original path 
    SQ_PATHS *psm; //< pointer to paths element of the merged path 
    merged_path->entp = sq_merge_lists( path1->entp, path2->entp );
    merged_path->endp = sq_merge_lists( path1->endp, path2->endp );
    D2( printf( "Merging two paths\n" ) );
    // now merge source paths
    // copy paths from path1
    pso = path1->paths;
    psm = NULL;
    while ( pso )
    {
        SQ_PATHS *psel = sq_create_new_paths_item( pso->path );
        if ( psm )
            psm->next = psel;
        psm = psel;    
        pso = pso->next;
        D2( printf( "Added another path from p2\n" ) );
    } // end of path1 paths copy loop
    // copy paths from path2
    pso = path2->paths;
    while ( pso )
    {
        SQ_PATHS *psel = sq_create_new_paths_item( pso->path );
        if ( psm )
            psm->next = psel;
        psm = psel;    
        pso = pso->next;
        D2( printf( "Added another path from p2\n" ) );
    } // end of path2 paths copy loop
    return merged_path;
} // sq_merge_paths








/** Links two paths and returns pointer to concatenation (this). This functions performs A->B tuning.
 * \param this predecessor
 * \param next successor
 * \return Pointer to the result path.
*/
SQ_PATH* sq_link_paths( SQ_PATH *this, SQ_PATH *next )
{
    SQ_PATH *pn;
    //D( printf( "%s dummy: ok\n", __FUNCTION__ ) );
    if ( ! this )
        return next;
    D2( printf( "Link state %s with state %s\n", sq_id_to_str(this->entp->state->id), sq_id_to_str(next->entp->state->id) ) ); 
    // add the 'next' path to each end point of the path 'this', unless it is FINAL point
    //this->next = next;

    // traverse all end points of the left path and the path 'next' as a successor. 
    assert( this->endp );
    {
        SQ_SEQUENCE *st = this->endp;
        while ( st )
        {
            pn = st->state->path;
            assert( pn ); // each state must belong to a path
            if ( !pn->paths )
            {
				if ( !(st->state->opt & ST_FINAL_POINT) )
				{   // we do not add successors to a final point
					pn->paths = sq_create_new_paths_item( next ); 
				}
            }
            else
            {
                assert(0); // \todo TODO implement logic in this case
            }
            st = st->next;
        } // end of this path end points processing

    }
    
    this->endp = next->endp; // init result end points of the new path as end points of the path 'next'
                             // \note currently pointer to end points is overwritten here, will not be able to release memory 
                             // rather than by using sq_cleanup!  
    
    return this;
} // sq_link_paths





/** Allowed path description is finished, final check and copy to global variable
 * \param ap pointer to allowed path
 * \return Path finalization result code, 0 if no error.
 */ 
int sq_finalize_allowed_path( SQ_PATH *ap )
{
    //D( printf( "%s dummy: ok\n", __FUNCTION__ ) );
    allowed_path = ap;
    return 0;
} // sq_finalize_path








/** Tries to find in paths list the one which contains a state with the required id
 * \param paths list of possible paths
 * \param type required type of the state
 * \param timeout state time value, is used for timeout control
 * \return Pointer to the found path or NULL if nothing found
 */
SQ_PATH *sq_find_path_by_id( SQ_PATHS *paths, int id, double timeout )
{
    SQ_PATH *path = NULL;

    while ( paths )
    {
        path = paths->path;
        assert( path ); // must be non-NULL path for each valid paths item, double check otherwise
        if ( sq_find_state_by_id( path->entp, id, timeout ) )
            break; // state is found, path valid, stop the search now with success
        paths = paths->next;
    }

	return paths ? path : (SQ_PATH*)0;
} // sq_find_path_by_id








/** Tries to find in the sequence list one with the required type
 * \param sequence list of possible sequence
 * \param type requested state type
 * \param timeout state time value, is used for timeout control
 * \return Pointer to the found state, NULL if not found
 * \note Timeout value is also checked if in a state it is not zero
 */
SQ_STATE *sq_find_state_by_id( SQ_SEQUENCE *sequence, int type, double timeout )
{
    SQ_STATE *res = NULL;
    SQ_SEQUENCE *st = sequence;
    
    if ( ! sequence )
        return NULL;
    
    while ( st )
    {
        if (     ( st->state->id   == type )
           )   
        {
            if ( (st->state->timeout == 0.0) || (st->state->timeout > timeout) )
            {    
                res = st->state;
                break;
            }
            else
            {
                D2( printf( "timeout detected\n" ) );   
            }
        }
        st = st->next;
    }
    return res;
} // sq_find_state_by_id








/** Checks if second path is possible within the first path
 * \param host path which might describe possible transitions
 * \param targ path which might describe observed transitions
 * \return If targ path is possible in host, then 0, otherwise 1;
 */
int sq_check_transitions( SQ_PATH *host, SQ_PATH *targ )
{
    int res = 0;

    SQ_PATH  *hp;
    SQ_PATH  *tp;
    SQ_STATE *st;

    if ( ! host  || ! targ )
        return -1; // arguments can't be empty

    hp = host;
    tp = targ;
    while ( tp )
    {
		D( printf( "Checking node %s..", sq_id_to_str( tp->entp->state->id ) ) );
        if ( (st = sq_find_state_by_id( hp->entp, tp->entp->state->id  , tp->entp->state->timeout )) == NULL )
        {
			D( printf( "[check failed]\n" ) );
            return 1;
        }
		D( printf( "[check passed]\n" ) );
		if ( !tp->paths )
			break;  // no more successors <=> target path finished <=> valid, quit the process
        tp = tp->paths->path;
        //if ( !tp )
        //    break;   //  
        hp = sq_find_path_by_id( st->path->paths, tp->entp->state->id  , tp->entp->state->timeout );
        if ( !hp )
        {
            res = 2; // host path finished, can't continue
			D( printf( "Error: Can't check more nodes: allowed path finished\n" ) );
			break;
        }
        //tp = tp->paths->path;
    }
    return res;
} // sq_check_transitions








/** Executed path description is finished, copy to executed path global variable and check it against allowed path.
 * \param ep pointer to executed path
 * \return Check result, 0 if no error
 */ 
int sq_check_executed_path( SQ_PATH *ep )
{
    int error;
    error = sq_check_transitions( allowed_path, ep );
	if ( error )
		sq_errors++; // increment global errors counter if an error occured
    return error;
} // sq_check_executed_path






/** Sets timeout for the given state, it will be checked if timing is available.
 * \param state pointer to path structure which describes current state
 * \param timeout timeout value (double), in milliseconds usually but can be scaled by user (all times respectively)
 * \return pointer to updated path(state)
 */
SQ_PATH* sq_set_state_timeout( SQ_PATH* state, double timeout )
{
    if ( !state )
        return state;
    assert( state->entp );
    assert( state->endp );
    assert( state->entp->state == state->endp->state );
    state->entp->state->timeout = timeout;    
	D2( printf( "%s: ok\n", __FUNCTION__ ) );
    return state;
} // sq_set_state_timeout







/** Creates new paths list item.
 * \param path pointer to the path descriptor
 * \return Pointer to the created element 
 */
SQ_PATHS *sq_create_new_paths_item( SQ_PATH *path )
{
    SQ_PATHS *np = (SQ_PATHS*)emalloc( sizeof(SQ_PATHS) );
    assert( np );
    memset( np, 0, sizeof(SQ_PATHS) );
    np->path = path;
    return np;
} // sq_create_new_paths_item








/** Creates new path and initializes its state with the given pointer.
 *
 */
SQ_PATH* sq_create_new_path( SQ_STATE* state )
{
    SQ_PATH*   np    = (SQ_PATH*)emalloc( sizeof(SQ_PATH) );
    SQ_SEQUENCE* nentp = (SQ_SEQUENCE*)emalloc( sizeof(SQ_SEQUENCE) );
    SQ_SEQUENCE* nendp = (SQ_SEQUENCE*)emalloc( sizeof(SQ_SEQUENCE) );

    assert( np );
    
    memset( nentp, 0, sizeof( SQ_SEQUENCE ) ); // clear content before using it
    memset( nendp, 0, sizeof( SQ_SEQUENCE ) ); // clear content before using it

    np->entp = nentp;
    np->entp->state = state;
    np->endp = nendp;
    np->endp->state = state;
    np->paths  = NULL;

    // if the state is not null, assosiate it with this path: the path will be used for further navigation 
    if ( state )
    {
        state->path = np;
    }
    return np;
} // sq_create_new_path







/** Creates new state and initializes its type with the given value
 * \param id state unique identifier
 * \return pointer to the created state
*/
SQ_STATE* sq_create_state( int id )
{
    SQ_STATE *state;
    state = (SQ_STATE*)emalloc( sizeof( SQ_STATE ) );
    state->id   = id;
    state->opt  = 0;
    state->timeout = 0;
    state->path = NULL;  
    D2( printf( "Created state of type %d\n", id ) ); 
    return state;
} // sq_create_state






/** Adds entry point to the path
 * \param path a path to which the entry point is being added
 * \param entp new entry point 
 * \return pointer to modified path
 */
SQ_PATH *sq_add_entry_point( SQ_PATH *path, SQ_PATH* entp )
{
    return path;
} // sq_add_entry_point







/** 
* @brief Defines new state with the given identification string
* Main task of the function is to create a new symbol with unique id for future use.
* @param st_name new state name
* 
* @return Null (result shoul not be used anywhere, type declared for compatibility with grammar).
*/
SQ_PATH *sq_define_new_state( SYMBOL *st_name )
{
    SYMBOL  *sym; //!< pointer to the new symbol
    //SQ_PATH *np;  //!< pointer to new path
    if ( (sym = sq_sym_lookup( (char*)st_name )) )
    {
        // symbol already exist, return pointer to its path
    }
    else
    {
        sym = sq_sym_install( (char*)st_name, VARSTATE, 0.0 );
        D2( printf( "Created new state id, name %s, id %d\n", (char*)st_name, sym->id ) );
        assert( sym );
    }
	return (SQ_PATH*) 0; // for syntax compatibility with the parser, result should not be used anywere
} // sq_define_new_state
