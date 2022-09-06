/** 
 * @file
 * @brief Initialization functions tests.
 */
#include <embUnit/embUnit.h>

/*embunit:include=+ */
#include "sq_init.h"
#include "sq_code.h"
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
static void test_sq_init(void)
{
	//TEST_FAIL("no implementation");
    allowed_path = (SC_PATH*)-1;
    executed_path = (SC_PATH*)0xAA;
    sq_init();
    TEST_ASSERT_NULL( allowed_path );
    TEST_ASSERT_NULL( executed_path );
}

/** \test
 *
 */
static void test_sq_cleanup(void)
{
	//TEST_FAIL("no implementation");
    sq_cleanup();
    TEST_ASSERT_NULL( allowed_path );
    TEST_ASSERT_NULL( executed_path );
}

/*embunit:impl=- */
TestRef sq_init_tests_tests(void)
{
	EMB_UNIT_TESTFIXTURES(fixtures) {
	/*embunit:fixtures=+ */
		new_TestFixture("test_sq_init",test_sq_init),
		new_TestFixture("test_sq_cleanup",test_sq_cleanup),
	/*embunit:fixtures=- */
	};
	EMB_UNIT_TESTCALLER(sq_init_tests,"sq_init_tests",setUp,tearDown,fixtures);
	return (TestRef)&sq_init_tests;
};
