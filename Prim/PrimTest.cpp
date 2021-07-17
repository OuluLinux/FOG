#include <Prim/PrimIncludeAll.h>


#define PRIM_TEST_MODULE




TYPEINFO_SINGLE(PrimTest, PrimPrint);

char PrimTest::_initialised = false; //   Set true once class initialised.
PrimTest* PrimTest::_first = 0;   //   First module in null terminated linked list of module tests.
PrimTest* PrimTest::_last = 0;   //   Last module in null terminated linked list of module tests.
int PrimTest::_starts = 0;    //   Number of tests started.
int PrimTest::_fails = 0;    //   Number of tests failed.
int PrimTest::_passes = 0;    //   Number of tests passed.
bool PrimTest::_debug_fail = false;  //   true to generate debug messages for fails.
bool PrimTest::_debug_pass = false;  //   true to generate debug messages for passes.

//
//  	Register a module test called aName.
//
PrimTest::PrimTest(const char* aName)
	:
	_next(0),
	_prev(_last),
	_name(aName) {
	if (_first == 0)
		_first = this;
		
	if (_last != 0)
		_last->_next = this;
		
	_last = this;
}

//
//  	Register a module test called aName.
//
PrimTest::PrimTest(const PrimString& aName)
	:
	_next(0),
	_prev(_last),
	_name(aName) {
	if (_first == 0)
		_first = this;
		
	if (_last != 0)
		_last->_next = this;
		
	_last = this;
}

//
//  	Deregister the module test.
//
PrimTest::~PrimTest() {
	if (_next != 0)
		_next->_prev = _prev;
		
	if (_prev != 0)
		_prev->_next = _next;
		
	if (_first == this)
		_first = _next;
		
	if (_last == this)
		_last = _prev;
}

//
//  	Perform the module tests, returning true if all successful.
//
int PrimTest::do_tests() {
	_starts = 0;
	_passes = 0;
	_fails = 0;
	
	for (PrimTest* p = _first; p != 0; p = p->_next) {
		_starts++;
		
		if (p->do_test()) {
			CONDMSG(_debug_pass, *p << " has passed.");
			_passes++;
		}
		else {
			CONDMSG(_debug_fail, *p << " has failed.");
			_fails++;
		}
	}
	
	return ((_fails == 0) && (_passes == _starts));
}

//
//  	Print a summary of the test results on s.
//
std::ostream& PrimTest::print_test_summary(std::ostream& s) {
	s << _passes << "/" << _starts << " tests passed\n";
	s << _fails << "/" << _starts << " tests failed\n";
	unsigned int unFinished = _starts - _passes - _fails;
	s << unFinished << "/" << _starts << " tests incomplete\n";
	
	if (PrimNotifier::warning_count() > 0)
		s << PrimNotifier::warning_count() << " warnings reported\n";
		
	if (PrimNotifier::error_count() > 0)
		s << PrimNotifier::error_count() << " errors reported\n";
		
	if (_passes != _starts)
		s << "\007Test failed.\n";
	else if (PrimNotifier::error_count() > 0)
		s << "\007Test passed subject to qualifications.\n";
	else if (PrimNotifier::warning_count() > 0)
		s << "\007Test passed subject to minor qualifications.\n";
	else
		s << "\nAll tests passed without error or qualification.\n";
		
	return (s);
}

//
//  	Generate a diagnostic print out of the module test.
//
std::ostream& PrimTest::print_this(std::ostream& s) const {
	return (s << _name);
}
