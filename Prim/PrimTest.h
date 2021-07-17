
#ifndef PRIMTEST_HXX
#define PRIMTEST_HXX


#ifdef PRIM_TEST_MODULE



//
//  		PrimTest organises module tests. A module test is requested by instantiation of a derived
//  		class. The set of module tests is maintained by this class.
//
class PrimTest : public PrimPrint {
	TYPEDECL_SINGLE(PrimTest, PrimPrint)
private:
	PrimTest* _next;       //   Next module in linked list of module tests, 0 at end.
	PrimTest* _prev;       //   Previous module in linked list of module tests, 0 at end.
	const PrimStringHandle _name;    //   Name of module test.
	static PrimTest* _first;     //   First module in null terminated linked list of module tests.
	static PrimTest* _last;      //   Last module in null terminated linked list of module tests.
	static char _initialised;     //   Set true once initialised.
	static int _starts;       //   Number of tests started.
	static int _fails;       //   Number of tests failed.
	static int _passes;       //   Number of tests passed.
	static bool _debug_fail;     //   true to generate debug messages for fails.
	static bool _debug_pass;     //   true to generate debug messages for passes.
	
private:
	PrimTest(const PrimTest& aModule);    //   Creation by copy is forbidden.
	PrimTest& operator=(const PrimTest& aModule); //   Creation by assignment is forbidden.
	
public:
	PrimTest(const char* aName);
	PrimTest(const PrimString& aName);
	virtual ~PrimTest();
	//  	  virtual int check(int fullCheck = false) const;
	//
	//  	Report the diagnose fails control parameter.
	//
	static bool& debug_fail() { return (_debug_fail); }
	//
	//  	Report the diagnose fails control parameter.
	//
	static bool& debug_pass() { return (_debug_pass); }
	static int do_tests();
	//
	//  		All derived modules must implement their test.
	//
	virtual int do_test() = 0;
	static std::ostream& print_test_summary(std::ostream& s);
	virtual std::ostream& print_this(std::ostream& s) const;
};

#define PRIMTEST_MODULE_DECL(CLASS_NAME, TEST_NAME)   \
	class CLASS_NAME : public PrimTest       \
	{               \
		TYPEDECL_SINGLE(CLASS_NAME, PrimTest)     \
	public:              \
		static CLASS_NAME _module_test;       \
	public:              \
		CLASS_NAME() : PrimTest(TEST_NAME) {}     \
		virtual int do_test();         \
	};               \
	TYPEINFO_SINGLE(CLASS_NAME, PrimTest)      \
	CLASS_NAME CLASS_NAME::_module_test;

#endif
#endif

