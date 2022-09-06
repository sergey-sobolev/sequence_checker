/** 
 * @file
 * @brief Main file of testing framework (tests runner implementation).
 */

#include <embUnit/embUnit.h>
#include <textui/TextOutputter.h>
#include <textui/TextUIRunner.h>

/*embunit:extern=+ */
extern TestRef sq_code_tests_tests(void);
extern TestRef sq_symtab_tests_tests(void);
extern TestRef sq_init_tests_tests(void);
extern TestRef sq_options_test_tests(void);
extern TestRef sq_parser_tests_tests(void);
/*embunit:extern=- */

#define ENGINE TextUIRunner
#define TestRunner_runTest TextUIRunner_runTest
#define TestRunner_start TextUIRunner_start
#define TestRunner_end TextUIRunner_end

int main(int argc,char *argv[])
{
    //TextUIRunner_setOutputter(XMLOutputter_outputter());
    TextUIRunner_setOutputter(TextOutputter_outputter());
    //TextUIRunner_setOutputter(CompilerOutputter_outputter());
  
    TestRunner_start();
	/*embunit:run=+ */
		TestRunner_runTest(sq_options_test_tests());
		TestRunner_runTest(sq_init_tests_tests());
		TestRunner_runTest(sq_symtab_tests_tests());
		TestRunner_runTest(sq_code_tests_tests());
		TestRunner_runTest(sq_parser_tests_tests());
	/*embunit:run=- */
	TestRunner_end();
	return 0;
}
