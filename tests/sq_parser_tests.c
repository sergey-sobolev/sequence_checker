/** 
 * @file
 * @brief States parser tests.
 */
#include <embUnit/embUnit.h>

/*embunit:include=+ */
#include "sq_parser.h"
#include "sq_options.h"
#include "sq_init.h" // sq_init/cleanup
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
}

/*embunit:impl=+ */

/** \test
 *
 */
static void test_sq_parser(void)
{
    #define TEST_FILE "sample.txt"

    int res;
    extern char sq_file_name[];
    int argc = 2;
    char *argv[] = { "test", TEST_FILE };
    res = process_options( argc, argv );
    TEST_ASSERT_EQUAL_INT( 0, res );
    TEST_ASSERT_EQUAL_STRING( TEST_FILE, sq_file_name );
    res = sq_parser( sq_file_name ); 
    TEST_ASSERT_EQUAL_INT( 0, res );
} // test_sq_parser

/*embunit:impl=- */
TestRef sq_parser_tests_tests(void)
{
	EMB_UNIT_TESTFIXTURES(fixtures) {
	/*embunit:fixtures=+ */
		new_TestFixture("test_sq_parser",test_sq_parser),
	/*embunit:fixtures=- */
	};
	EMB_UNIT_TESTCALLER(sq_parser_tests,"sq_parser_tests",setUp,tearDown,fixtures);
	return (TestRef)&sq_parser_tests;
};
