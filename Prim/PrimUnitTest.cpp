// **************************** RACAL Confidential ****************************
//
// 	  Title:			Primitive library unit tests program
//
// 	  File Name:		PrimUnitTest.cc
//
// 	  Author:			E.D.Willink
//
// 	  SCCS:				%W% %G%
//
// 	  Latest Modification:
//  EDW	1.1		Date:	10-Apr-1996		Original
//  EDW	7.1		Date:	21-Aug-1998		use std:: streams
// END
//

#define PRIM_TEST_MODULE
#include <Prim/PrimIncludeAll.h>

PrimComParse& main_parser = PrimComParse::main_parser();
static PrimComToken_Bool zx(main_parser, PrimPrint::print_address(),
                            "zx", "display full hex address");
static PrimComToken_Bool zc(main_parser, PrimCheckLock::debug_check(),
                            "zc", "perform consistency checks");
static PrimComToken_Bool zf(main_parser, PrimCheckLock::debug_full_check(),
                            "zf", "perform full consistency checks");
static PrimComToken_Bool tf(main_parser, PrimTest::debug_fail(),
                            "tf", "debug test failures");
static PrimComToken_Bool tp(main_parser, PrimTest::debug_pass(),
                            "tp", "debug test passes");

//
// 	The main program parses the command line, activates the tests, and print a summary of the results.
//
int main(int argc, char* argv[]) {
	if (!main_parser.parse_args(argc, argv)) {
		main_parser.print_usage(std::cout);
		return EXIT_FAILURE;
	}
	
	if (PrimCheckLock::debug_full_check())
		PrimCheckLock::debug_check() = true;
		
	int testStatus = PrimTest::do_tests();
	std::cout << std::endl;
	PrimTest::print_test_summary(std::cout);
	return testStatus ? EXIT_SUCCESS : EXIT_FAILURE;
}

//
// 	Dummy routine to pull in testable code.
//
void dummy_routine() {
	// 	PrimNumberWords aBigNat;
	PrimNumber::make_zero();
}

