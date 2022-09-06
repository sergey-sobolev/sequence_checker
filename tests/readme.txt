- Add test case to suite:
    tuma TestSuiteName test_case_name
 
- Create test suite:
    tcuppa header.h SuiteName [test_case_1] [test_case_2]

- Create test suite to main:
    bcuppa main_file_name SuiteFileName 

- Add test suite to main function
    buma MainFileName SuiteFileName

embUnit Assertions

Table 2.1. embUnit Assertions
Assertion  Description
TEST_ASSERT_EQUAL_STRING(expected,actual)  Assert that strings actual and expected are equivalent
TEST_ASSERT_EQUAL_INT(expected,actual)  Assert that integers actual and expected are equivalent
TEST_ASSERT_NULL(pointer)  Assert that pointer value == NULL
TEST_ASSERT_MESSAGE(condition, message)  Assert that condition is TRUE (non-zero)
TEST_ASSERT(condition)  Assert that condition is TRUE (non-zero)
TEST_FAIL(message)  Register a failed assertion with the specified message. No logical test is performed. 

