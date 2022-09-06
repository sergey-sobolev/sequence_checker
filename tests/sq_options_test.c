/** 
 * @file
 * @brief Command line options parser tests.
 */

#include <embUnit/embUnit.h>

/*embunit:include=+ */
#include "sq_options.h"
/*embunit:include=- */

static void setUp(void)
{
	/* initialize */
}

static void tearDown(void)
{
	/* terminate */
}

/*embunit:impl=+ */

/** \test
 *
 */
static void test_sq_options(void)
{
    int res;
    int argc;
    {   // test help
        char *argv[] = { "name", "/?" };
        argc = 2;
        res  = 0;        res = process_options( argc, argv );
        TEST_ASSERT_EQUAL_INT( -2, res );
	}
    {   // test version
        char *argv[] = { "name", "--version" };
        argc = 2;
        res  = 0;
		res = process_options( argc, argv );
        TEST_ASSERT_EQUAL_INT( -1, res );
	}
    {   // test yydebug option
        extern int yydebug;
        char *argv[] = { "name", "-d" };
        argc = 2;
        res  = 0;
		res = process_options( argc, argv );
        TEST_ASSERT_EQUAL_INT( 0, res );
        TEST_ASSERT_EQUAL_INT( 1, yydebug );
    }

    //TEST_FAIL("no implementation");
}

/*embunit:impl=- */
TestRef sq_options_test_tests(void)
{
	EMB_UNIT_TESTFIXTURES(fixtures) {
	/*embunit:fixtures=+ */
		new_TestFixture("test_sq_options",test_sq_options),
	/*embunit:fixtures=- */
	};
	EMB_UNIT_TESTCALLER(sq_options_test,"sq_options_test",setUp,tearDown,fixtures);
	return (TestRef)&sq_options_test;
};
