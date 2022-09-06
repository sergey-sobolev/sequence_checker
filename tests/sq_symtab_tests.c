/** 
 * @file
 * @brief Symbol table manipulation functions tests.
 */
#include <embUnit/embUnit.h>

/*embunit:include=+ */
#include "sq_symtab.h"
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
static void test_sq_install(void)
{
	//TEST_FAIL("no implementation");
    SYMBOL *sym;
    sym = sq_sym_install( (char*)__FUNCTION__, 1, 0.5 ); 
    TEST_ASSERT_NOT_NULL( sym );
    TEST_ASSERT_EQUAL_INT( 1, sym->type );
    TEST_ASSERT( 0.5 == sym->u.val );
    TEST_ASSERT_EQUAL_STRING( (char*)__FUNCTION__, sym->name );
} // test_sq_install

/** \test
 *
 */
static void test_sq_lookup(void)
{
	//TEST_FAIL("no implementation");
    SYMBOL *sym_inst, *sym_lookup;
    sym_inst = sq_sym_install( (char*)__FUNCTION__, 1, 0.5 ); 
    TEST_ASSERT_NOT_NULL( sym_inst );
    TEST_ASSERT_EQUAL_INT( 1, sym_inst->type );
    TEST_ASSERT( 0.5 == sym_inst->u.val );
    TEST_ASSERT_EQUAL_STRING( (char*)__FUNCTION__, sym_inst->name );
    
    sym_lookup = sq_sym_lookup( (char*)__FUNCTION__ );
    TEST_ASSERT( sym_lookup == sym_inst );
    TEST_ASSERT_NOT_NULL( sym_lookup );
    TEST_ASSERT_EQUAL_INT( 1, sym_lookup->type );
    TEST_ASSERT( 0.5 == sym_lookup->u.val );
    TEST_ASSERT_EQUAL_STRING( __FUNCTION__, sym_lookup->name );

} // test_sq_lookup

/*embunit:impl=- */
TestRef sq_symtab_tests_tests(void)
{
	EMB_UNIT_TESTFIXTURES(fixtures) {
	/*embunit:fixtures=+ */
		new_TestFixture("test_sq_install",test_sq_install),
		new_TestFixture("test_sq_lookup",test_sq_lookup),
	/*embunit:fixtures=- */
	};
	EMB_UNIT_TESTCALLER(sq_symtab_tests,"sq_symtab_tests",setUp,tearDown,fixtures);
	return (TestRef)&sq_symtab_tests;
};
