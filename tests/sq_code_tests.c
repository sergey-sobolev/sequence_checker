/** 
 * @file
 * @brief Paths manipulation functions tests
 */
#include <embUnit/embUnit.h>

/*embunit:include=+ */
#include "sq_debug.h"
#include "sq_code.h"
#include "sq_symtab.h"
#include "sq_init.h" // sq_init/cleanup
#include "y.tab.h" // INIT, SYNC, etc
/*embunit:include=- */

static void setUp(void)
{
	/* initialize */
    sq_init();
}

static void tearDown(void)
{
	/* terminate */
    sq_cleanup();
    //fprintf( stderr, "allocated: %d, freed: %d\n", allocated, freed );
    assert( allocated == freed );
}

/*embunit:impl=+ */

/** \test
 *
 */
static void test_sq_link_paths(void)
{
    SQ_STATE *st1, *st2, *st3, *st4;
    SQ_PATH  *path1, *path2, *tmpp;
    st1 = sq_create_state( 1 );
    st2 = sq_create_state( 2 );
    st3 = sq_create_state( 3 );
    st4 = sq_create_state( 4 );
   
    path1 = path2 = tmpp = NULL; 

    // start with INIT 
    tmpp = sq_create_new_path( st1 );
    path1 = sq_link_paths( path1, tmpp ); 
    tmpp = sq_create_new_path( st2 );
    path1 = sq_link_paths( path1, tmpp );
}

/** \test
 *
 */
static void test_sq_finalize_allowed_path(void)
{
//	TEST_FAIL("no implementation");
    int res = 0;
    SQ_STATE *st1;
    SQ_PATH  *path1;
    st1 = sq_create_state( 1 );
    path1 = NULL; 

    path1 = sq_create_new_path( st1 );
    res = sq_finalize_allowed_path( path1 ); 
    TEST_ASSERT_EQUAL_INT( 0, res );
    TEST_ASSERT( allowed_path == path1 );
}

/** \test
 *
 */
static void test_sq_check_executed_path(void)
{
//	TEST_FAIL("no implementation");
    int res = 0;
    SQ_STATE *st1, *st2, *st3, *st4;
	SQ_PATH  *pst1, *pst2, *pst3, *pst4;
    SQ_PATH  *path1, *path2, *tmpp;
    st1 = sq_create_state( 1 );
    st2 = sq_create_state( 2 );
    st3 = sq_create_state( 3 );
    st4 = sq_create_state( 4 );
	pst1 = sq_create_new_path( st1 );
	pst2 = sq_create_new_path( st2 );
	pst3 = sq_create_new_path( st3 );
	pst4 = sq_create_new_path( st4 );
   
    path1 = path2 = tmpp = NULL; 

    // start with INIT 
    path1 = sq_link_paths( pst3, pst4 );
    TEST_ASSERT( path1 == st3->path );
    // start with 3
	TEST_ASSERT( path1->entp->state == st3 );
    // 3->4
	TEST_ASSERT( path1->paths->path->entp->state == st4 );
    
    path2 = sq_link_paths( pst2, path1 );
	// check start with 2
    TEST_ASSERT( path2->entp->state == st2 );
    // check 2->3
	TEST_ASSERT( path2->paths->path->entp->state == st3 );
    // check 2->3->4
	TEST_ASSERT( path2->paths->path->entp->state->path->paths->path->entp->state == st4 );

    path1 = sq_link_paths( pst1, path2 );
    // check start with 1
    TEST_ASSERT( path1->entp->state == st1 );
    // check 1->2
	TEST_ASSERT( path1->paths->path->entp->state == st2 );
    // check 1->2->3
	TEST_ASSERT( path1->paths->path->entp->state->path->paths->path->entp->state == st3 );
    // check 1->2->3->4
	TEST_ASSERT( path1->paths->path->entp->state->path->paths->path->entp->state->path->paths->path->entp->state == st4 );

    sq_finalize_allowed_path( path1 );
    res = sq_check_executed_path( path1 );
    TEST_ASSERT_EQUAL_INT( 0, res );
}

/** \test
 *
 */
static void test_sq_set_state_timeout(void)
{
    SQ_STATE *st;
    SQ_PATH  *ph, *ph1;
    st  = sq_create_state( 1 );
    TEST_ASSERT( st != NULL );
    ph  = sq_create_new_path( st );
    TEST_ASSERT( ph->entp->state->timeout == 0.0 );
    ph1 = sq_set_state_timeout( ph, 1.0 );
    TEST_ASSERT( ph == ph1 );
    TEST_ASSERT( ph1->entp->state->timeout == 1.0 );
//	TEST_FAIL("no implementation");
}

/** \test
 *
 */
static void test_sq_create_state(void)
{
//	TEST_FAIL("no implementation");
    SQ_STATE *st;
    st = sq_create_state( 1 );
    TEST_ASSERT( st != NULL );
    TEST_ASSERT( st->id  == 1 );
    TEST_ASSERT( st->opt == 0 );
    TEST_ASSERT( st->timeout == 0.0 );
    TEST_ASSERT( st->path == NULL );
    //TEST_ASSERT( );
} // test_sq_create_state

/** \test
 *
 */
static void test_sq_create_new_path(void)
{
//	TEST_FAIL("no implementation");
    SQ_STATE *st = sq_create_state(1);
    SQ_PATH  *np = sq_create_new_path( st );
    TEST_ASSERT( np->entp->state == st );
    TEST_ASSERT( np->paths == NULL );
}

// /** \test
//  *
//  */
// static void test_sq_merge_paths(void)
// {
// 	TEST_FAIL("no implementation");
// } // test_sq_merge_paths

// /** \test
//  *
//  */
// static void test_sq_merge_lists(void)
// {
// 	TEST_FAIL("no implementation");
// }

// /** \test
//  *
//  */
// static void test_sq_append_list_with_state(void)
// {
// 	TEST_FAIL("no implementation");
// }

/** \test
 *  \todo Script for automated sequence and paths initialization with result control.
 */
//#define 
static void test_sq_construct_path(void)
{
    //int res;
    SQ_STATE *stA;
    SQ_PATH  *ptA;
    SQ_STATE *stB;
    SQ_PATH  *ptB;
    SQ_PATH  *ptM;
	//TEST_FAIL("no implementation");
    // 1. describe path A
    {
        // 1. create state object & check fields
        stA = sq_create_state( 1 );
        TEST_ASSERT_NOT_NULL( stA );
        TEST_ASSERT_EQUAL_INT( 0, (int)stA->timeout );
        TEST_ASSERT_EQUAL_INT( 1, stA->id   );
        TEST_ASSERT_EQUAL_INT( 0, stA->opt );
        TEST_ASSERT_NULL( stA->path );

        // 2. create path object & check fields
        ptA = sq_create_new_path( stA );
        TEST_ASSERT_NOT_NULL( ptA );
        TEST_ASSERT_NOT_NULL( ptA->entp );
        TEST_ASSERT_NULL( ptA->entp->next );
        TEST_ASSERT( ptA->entp->state == stA );
        TEST_ASSERT_NOT_NULL( ptA->endp );
        TEST_ASSERT_NULL( ptA->endp->next );
        TEST_ASSERT( ptA->endp->state == stA );
        TEST_ASSERT_NULL( ptA->paths );
    }

    // 2. describe path A|B
    {
        // 1. create state B object & path
        stB = sq_create_state( 2 );
        TEST_ASSERT_NOT_NULL( stB );
        TEST_ASSERT_EQUAL_INT( 0, (int)stB->timeout );
        TEST_ASSERT_EQUAL_INT( 2, stB->id   );
        TEST_ASSERT_EQUAL_INT( 0, stB->opt );
        TEST_ASSERT_NULL( stB->path );

        // 2. create path object & check fields
        ptB = sq_create_new_path( stB );
        TEST_ASSERT_NOT_NULL( ptB );
        TEST_ASSERT_NOT_NULL( ptB->entp );
        TEST_ASSERT_NULL( ptB->entp->next );
        TEST_ASSERT( ptB->entp->state == stB );
        TEST_ASSERT_NOT_NULL( ptB->endp );
        TEST_ASSERT( ptB->endp->state == stB );
        TEST_ASSERT_NULL( ptB->paths );
        TEST_ASSERT( stB->path == ptB );

        // 2. use object A from the first path of the test

        // 3. create joint path object and fill it

        ptM = sq_merge_paths( ptA, ptB );
        
        // 4. check fields
        // now the merged path should have 2 entry and two end points: A & B in both cases 
        TEST_ASSERT( ptM != ptA );
        TEST_ASSERT( ptM != ptB );
        TEST_ASSERT_NOT_NULL( ptM->endp );
        TEST_ASSERT_NOT_NULL( ptM->entp );
        TEST_ASSERT_NOT_NULL( ptM->entp->state );
        TEST_ASSERT_NOT_NULL( ptM->entp->next );
        TEST_ASSERT_NOT_NULL( ptM->entp->next->state );
        TEST_ASSERT_NULL( ptM->entp->next->next );
        TEST_ASSERT_NOT_NULL( ptM->endp->state );
        TEST_ASSERT_NOT_NULL( ptM->endp->next );
        TEST_ASSERT_NOT_NULL( ptM->endp->next->state );
        TEST_ASSERT_NULL( ptM->endp->next->next );
        TEST_ASSERT( ptM->entp->state == stA );
        TEST_ASSERT( ptM->entp->next->state == stB );
        TEST_ASSERT( ptM->endp->state == stA );
        TEST_ASSERT( ptM->endp->next->state == stB );
    }

    // 3. describe path A->B
    {
        // now we have A&B sequence and paths,
        // we also have a merged path, now link them, however, it can be not a good idea since it looks like we will need
        // unique set of objects for each incoming state
        // Result must be: a path with entry point A and end point B
        // Note: sq_link_path modifies first argument: function links with it the second path
        SQ_PATH *ptL;
        // link all possible paths from A with path B
        ptL = sq_link_paths( ptA, ptB );
        TEST_ASSERT( ptL == ptA );
        TEST_ASSERT_NOT_NULL( ptL->paths );
        TEST_ASSERT( ptL->paths->path == ptB );
        // now check that A->B is valid in this new path
        TEST_ASSERT( ptL->entp->state == stA );
        TEST_ASSERT( ptL->paths->path->entp->state == stB );

    }
    // 4. describe path A->B->C
    {
        // create the path
        SQ_STATE *st4A = sq_create_state( 1 );
        SQ_STATE *st4B = sq_create_state( 2 );
        SQ_STATE *st4C = sq_create_state( 3 );
        SQ_PATH  *pt4A = sq_create_new_path( st4A );
        SQ_PATH  *pt4B = sq_create_new_path( st4B );
        SQ_PATH  *pt4C = sq_create_new_path( st4C );
        SQ_PATH  *pt4LBC  = sq_link_paths( pt4B, pt4C );
        SQ_PATH  *pt4LABC = sq_link_paths( pt4A, pt4LBC );
        
        TEST_ASSERT( pt4LBC == pt4B );
        TEST_ASSERT( pt4LABC == pt4A );

        // check result
        TEST_ASSERT_NOT_NULL( pt4LABC->paths );
        TEST_ASSERT( pt4LABC->paths->path == pt4B );
        TEST_ASSERT( pt4LABC->paths->path->paths->path == pt4C );
        // now check that A->B is valid in this new path
        TEST_ASSERT( pt4LABC->entp->state == st4A );
        TEST_ASSERT( pt4LABC->paths->path->entp->state == st4B );
        // now check that B->C is valid in this new path
        TEST_ASSERT( pt4LABC->paths->path->paths->path->entp->state == st4C );

    }
    // 5. describe path A->B|C
    {
        // create the path
        SQ_STATE *st5A = sq_create_state( 1 );
        SQ_STATE *st5B = sq_create_state( 2 );
        SQ_STATE *st5C = sq_create_state( 3 );
        SQ_PATH  *pt5A = sq_create_new_path( st5A );
        SQ_PATH  *pt5B = sq_create_new_path( st5B );
        SQ_PATH  *pt5C = sq_create_new_path( st5C );
        SQ_PATH  *pt5M = sq_merge_paths( pt5B, pt5C );
        SQ_PATH  *pt5L = sq_link_paths( pt5A, pt5M );
        TEST_ASSERT( pt5A->entp->state == st5A );
        TEST_ASSERT_NOT_NULL( pt5M );
        TEST_ASSERT_NOT_NULL( pt5L );
        TEST_ASSERT( pt5L != pt5M );
        // now check paths
        // check that entry point is A
        TEST_ASSERT( pt5L->entp->state == st5A );
        // check that B is successor
        TEST_ASSERT( pt5L->paths->path->entp->state == st5B );
        // check that C is also successor
        TEST_ASSERT_NOT_NULL( pt5L->paths->path->entp->next );
        TEST_ASSERT( pt5L->paths->path->entp->next->state == st5C );
        // check that there is an alternative path
        //TEST_ASSERT_NOT_NULL( pt5L->paths->next ); 
        // check that in this alternative path C is presented 
        //TEST_ASSERT( pt5L->paths->next->path->entp->state == st5C );
    }
    // 6. describe path A->[B->C]|[D->E]
    {
        SQ_STATE *st6A = sq_create_state( 1 );
        SQ_STATE *st6B = sq_create_state( 2 );
        SQ_STATE *st6C = sq_create_state( 3 );
        SQ_STATE *st6D = sq_create_state( 4 );
        SQ_STATE *st6E = sq_create_state( 5 );
        SQ_PATH  *pt6A = sq_create_new_path( st6A );
        SQ_PATH  *pt6B = sq_create_new_path( st6B );
        SQ_PATH  *pt6C = sq_create_new_path( st6C );
        SQ_PATH  *pt6D = sq_create_new_path( st6D );
        SQ_PATH  *pt6E = sq_create_new_path( st6E );
        SQ_PATH  *pt6LBC = sq_link_paths( pt6B, pt6C );
        SQ_PATH  *pt6LDE = sq_link_paths( pt6D, pt6E );
        SQ_PATH  *pt6MBCDE = sq_merge_paths( pt6LBC, pt6LDE );
        SQ_PATH  *pt6R   = sq_link_paths( pt6A, pt6MBCDE );
        // check subpaths
        // check pt6LBC
        TEST_ASSERT( pt6B->entp->state->path == pt6LBC );
        TEST_ASSERT( pt6LBC->entp->state == st6B );
        TEST_ASSERT( pt6LBC->paths->path->entp->state == st6C );
        // now check the result (pt6R)
        TEST_ASSERT_NOT_NULL( pt6R );
        // check that A is entry point
        TEST_ASSERT( pt6R->entp->state == st6A );
        TEST_ASSERT_NOT_NULL( pt6R->paths );
        // check that B is successor A
        TEST_ASSERT( pt6R->paths->path->entp->state == st6B );
        TEST_ASSERT_NOT_NULL( pt6R->paths->path->entp->next );
        // check that D is also successor of A
        TEST_ASSERT( pt6R->paths->path->entp->next->state == st6D );
        // check that there are no more successors
        TEST_ASSERT_NULL( pt6R->paths->path->entp->next->next );
        // now check that C is successor of B 
        TEST_ASSERT_NOT_NULL( st6B->path );
        TEST_ASSERT( st6B->path->paths->path->entp->state == st6C );
        TEST_ASSERT_NULL( st6B->path->paths->next );
        // now check that E is successor of D
        TEST_ASSERT_NOT_NULL( st6D->path );
        TEST_ASSERT( st6D->path->paths->path->entp->state == st6E );
        TEST_ASSERT_NULL( st6D->path->paths->next );
    }
    // 7. describe path A->[B->C]|[D->E]->F
    {
        SQ_STATE *st7A = sq_create_state( 1 );
        SQ_STATE *st7B = sq_create_state( 2 );
        SQ_STATE *st7C = sq_create_state( 3 );
        SQ_STATE *st7D = sq_create_state( 4 );
        SQ_STATE *st7E = sq_create_state( 5 );
        SQ_STATE *st7F = sq_create_state( 6 );
        SQ_PATH  *pt7A = sq_create_new_path( st7A );
        SQ_PATH  *pt7B = sq_create_new_path( st7B );
        SQ_PATH  *pt7C = sq_create_new_path( st7C );
        SQ_PATH  *pt7D = sq_create_new_path( st7D );
        SQ_PATH  *pt7E = sq_create_new_path( st7E );
        SQ_PATH  *pt7F = sq_create_new_path( st7F );
        SQ_PATH  *pt7LBC = sq_link_paths( pt7B, pt7C );
        SQ_PATH  *pt7LDE = sq_link_paths( pt7D, pt7E );
        SQ_PATH  *pt7MBCDE = sq_merge_paths( pt7LBC, pt7LDE );
        SQ_PATH  *pt7LBCDEF = sq_link_paths( pt7MBCDE, pt7F );
        SQ_PATH  *pt7R   = sq_link_paths( pt7A, pt7LBCDEF );
        // copy all checks from 6: additionally check if st7F is a normal successor
        // check subpaths
        // check pt7LBC
        TEST_ASSERT( pt7B->entp->state->path == pt7LBC );
        TEST_ASSERT( pt7LBC->entp->state == st7B );
        TEST_ASSERT( pt7LBC->paths->path->entp->state == st7C );
        // now check the result (pt7R)
        TEST_ASSERT_NOT_NULL( pt7R );
        // check that A is entry point
        TEST_ASSERT( pt7R->entp->state == st7A );
        TEST_ASSERT_NOT_NULL( pt7R->paths );
        // check that B is successor A
        TEST_ASSERT( pt7R->paths->path->entp->state == st7B );
        TEST_ASSERT_NOT_NULL( pt7R->paths->path->entp->next );
        // check that D is also successor of A
        TEST_ASSERT( pt7R->paths->path->entp->next->state == st7D );
        // check that there are no more successors
        TEST_ASSERT_NULL( pt7R->paths->path->entp->next->next );
        // now check that C is successor of B 
        TEST_ASSERT_NOT_NULL( st7B->path );
        TEST_ASSERT( st7B->path->paths->path->entp->state == st7C );
        TEST_ASSERT_NULL( st7B->path->paths->next );
        // now check that E is successor of D
        TEST_ASSERT_NOT_NULL( st7D->path );
        TEST_ASSERT( st7D->path->paths->path->entp->state == st7E );
        TEST_ASSERT_NULL( st7D->path->paths->next );
        // now check that F can be reached
        // check that F is successor for C & E
        TEST_ASSERT_NOT_NULL( st7C->path->paths );
        TEST_ASSERT( st7C->path->paths->path->entp->state == st7F );
        TEST_ASSERT_NOT_NULL( st7E->path->paths );
        TEST_ASSERT( st7E->path->paths->path->entp->state == st7F );
        // write full path now
        //TEST_ASSERT( st7E == pt7R->path ); 
        //                     A                        B                               C                              F
        TEST_ASSERT( st7F == pt7R->paths->path->entp->state->path->paths->path->entp->state->path->paths->path->endp->state );
    }
    // 8. describe path
    {
    }
}

/*embunit:impl=- */
TestRef sq_code_tests_tests(void)
{
	EMB_UNIT_TESTFIXTURES(fixtures) {
	/*embunit:fixtures=+ */
		new_TestFixture("test_sq_link_paths",test_sq_link_paths),
		new_TestFixture("test_sq_finalize_allowed_path",test_sq_finalize_allowed_path),
		new_TestFixture("test_sq_check_executed_path",test_sq_check_executed_path),
		new_TestFixture("test_sq_set_state_timeout",test_sq_set_state_timeout),
		new_TestFixture("test_sq_create_state",test_sq_create_state),
		new_TestFixture("test_sq_create_new_path",test_sq_create_new_path),
		// new_TestFixture("test_sq_merge_paths",test_sq_merge_paths),
		// new_TestFixture("test_sq_merge_lists",test_sq_merge_lists),
		// new_TestFixture("test_sq_append_list_with_state",test_sq_append_list_with_state),
		new_TestFixture("test_sq_construct_path",test_sq_construct_path),
	/*embunit:fixtures=- */
	};
	EMB_UNIT_TESTCALLER(sq_code_tests,"sq_code_tests",setUp,tearDown,fixtures);
	return (TestRef)&sq_code_tests;
};
