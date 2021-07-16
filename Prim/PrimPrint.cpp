#include <Prim/PrimIncludeAll.h>











TYPEINFO_ROOT(PrimPrint);

//  
//  	Create and return a type identification for this class.
//  
/*   The steps are done explicitly to avoid ambiguity via PrimTypeInfo::operator const PrimTypeId() */
/*   The method is implemented as a virtual because cfront 2.1 doesn't implement a conversion merge for
				multiple inheritance */
//  PrimPrint::operator const PrimTypeId() const
//  {
//  	  const PrimTypeInfo& aTypeInfo = reportTypeInfo();
//  	  const PrimTypeId aTypeId(aTypeInfo);
//  	  return aTypeId;
//  }

#ifndef NO_PRIM_CHECKS
//  
//  	Perform consistency check on this object, returning true if ok, false after generating an error
//  	message if something is amiss. Derived classes should reimplement to perform a local invariant
//  	check if fullCheck is false, and a hierarchical check if fullCheck is true.
//  
//  	The base implementation here returns false (without an error message) if checking is disabled, in
//  	order to truncate the checking of all derived classes. The base implementation always returns
//  	true when checking is enabled.
//  
bool PrimPrint::check(bool fullCheck) const
{
 UNUSED(fullCheck);
 return PrimCheckLock::checking_enabled();
}
#endif

#ifndef NO_PRIM_TYPEINFO
//  
//  	Report the name of this class, using the run-time type information facilities.
//  
const char *PrimPrint::class_name() const { return (dynamic_type_info().name()); }
#endif

#ifndef NO_PRIM_STREAMS
//  
//  	Report the debug memory location to control hex address in diagnostics.
//  
bool& PrimPrint::print_address()
{
 static PrimGetEnv<bool> printAddress("PrimPrint::print_address", false);
 return printAddress;
}

//  
//  	Append the in-line diagnostic address of anObject to the output stream. If print_address
//  	has not been set, nothing is generated. If print_address has been set an @ followed by
//  	the hex address of anObject is appended to the output stream.
//  
//  	Returns the updated stream context.
//  
std::ostream& PrimPrint::print_address_of(std::ostream& s, const volatile void *anObject)
{
 if (print_address())
  s << "@" << (void *)anObject;
 return s;
}

//  
//  	Append the name of this class to the specified output stream. The default implementation
//  	here just appends class_name(). Derived template classes should reimplement to
//  	identify their template parameters.
//  
//  	Returns the updated stream context.
//  
std::ostream& PrimPrint::print_class_name(std::ostream& s) const { return s << class_name(); }

//  
//  	Provide a suffix description for the class name. The default implementation here output
//  	the hex instance address, but only when the print_address debug variable is set.
//  
//  	Returns the updated stream context.
//  
std::ostream& PrimPrint::print_class_suffix(std::ostream& s) const
{
 print_address_of(s, this);
 return s;
}

//  
//  	Provide a full description of this object and its children indented to anIndentLevel.
//  	The default implementation here indents and prints "this" resulting in the generation of
//  	the class name followed by print_this(), all indented by anIndentLevel. print_deep()
//  	is then invoked indented by anIndentLevel+1. Multiply inheriting derived classes
//  	may reimplement this method to invoke print_instance() and print_depth() for each base
//  	class. Singly inheriting classes should reimplement print_deep() in preference to
//  	print_on().
//  
//  	Returns the updated stream context.
//  
std::ostream& PrimPrint::print_deep(std::ostream& s, int anIndentLevel) const
{
 s << indent(anIndentLevel) << *this << "\n";
 return print_depth(s, anIndentLevel+1);
}

//  
//  	Provide a full description of the members of this object. The default implementation
//  	here does nothing, since a PrimPrint has no member variables. Derived classes should
//  	describe as much information as possible, excluding that provided by print_this().
//  	Child objects should be instructed to print_all, provided that this does not result
//  	in an infinite recursion.
//  
//  	Returns the updated stream context.
//  
std::ostream& PrimPrint::print_depth(std::ostream& s, int anIndentLevel) const
{
 UNUSED(anIndentLevel);
 return s;
}

//  
//  	Generate an in-line diagnostic of this object. The default implementation here
//  	prints the class name optionally followed by the instance address and then followed
//  	by any additional contribution from print_this(). The default output is of the form
//  	ClassName@0x3456(). The @0x3456 is suppressed when the print_address static member
//  	variable is false (the default). Any contribution from print_this() appears between
//  	the parentheses. Derived classes should not normally reimplement this method.
//  
//  	Returns the updated stream context.
//  
std::ostream& PrimPrint::print_instance(std::ostream& s) const
{
 print_class_name(s);
 print_class_suffix(s);
 s << "(";
 print_this(s);
 return s << ")";
}

//  
//  	Provide a full description of the members of this object. The default implementation
//  	here does nothing, since a PrimPrint has no member variables. Derived classes should
//  	describe as much information as possible, excluding that provided by print_this().
//  
//  	Returns the updated stream context.
//  
std::ostream& PrimPrint::print_members(std::ostream& s, int anIndentLevel) const
{
 UNUSED(anIndentLevel);
 return s;
}

//  
//  	Provide a full description of this object indented to anIndentLevel.
//  	The default implementation here indents and prints "this" resulting in the generation of
//  	the class name followed by print_this(), all indented by anIndentLevel. print_members()
//  	is then invoked indented by anIndentLevel+1. Multiply inheriting derived classes
//  	may reimplement this method to invoke print_instance() and print_members() for each base
//  	class. Singly inheriting classes should reimplement print_members() in preference to
//  	print_on().
//  
//  	Returns the updated stream context.
//  
std::ostream& PrimPrint::print_on(std::ostream& s, int anIndentLevel) const
{
 s << indent(anIndentLevel) << *this << "\n";
 return print_members(s, anIndentLevel+1);
}

//  
//  	Provide additional in-line diagnostic description of this object.
//  	The default implementation here does nothing, derived classes may add identification
//  	of their name or equivalent.
//  
//  	Returns the updated stream context.
//  
std::ostream& PrimPrint::print_this(std::ostream& s) const
{
 return s;
}
#endif

//  
//  	Return a pointer to this object. Multiply derived classes should reimplement to return a pointer
//  	to the first base class. (This is reimplementation is performed automatically by the
//  	TYPEDECL_DOUBLE etc. macros.)
//  
//  const PrimPrint *PrimPrint::report_this() const { return this; }

//  
//  	Return a pointer to this object. Multiply derived classes should reimplement to return a pointer
//  	to the first base class. (This reimplementation is performed automatically by the
//  	TYPEDECL_DOUBLE etc. macros.)
//  
PrimPrint *PrimPrint::report_this() { return this; }

#ifndef NO_PRIM_STREAMS
//  
//  	The debug implementation of an unwanted new() generates an error message and returns 0.
//  
void *prim_operator_new(size_t aSize)
{
 ERRMSG("prim_operator_new(" << aSize << ") invoked.");
 return 0;
}

//  
//  	The non-debug implementation of an unwanted delete() generates an error message.
//  
void prim_operator_delete(void *p)
{
 ERRMSG("prim_operator_delete(" << p << ") invoked.");
}

//  
//  	Generate an in-line print-out of this class by invoking print_instance().
//  
std::ostream& operator<<(std::ostream& s, const PrimPrint& aPrint)
{
 return aPrint.print_instance(s);
}
#endif

#ifndef NO_DBX_SUPPORT

//  #include <sys/signal.h>


int dbx_print_signalled_flag = false;   //   Cleared if an access failure occurs during a print-out.

//  
//  	This signal handler is highly specific to Sparcs. It is used to catch bad address
//  	accesses from within dbx_print_validate. A failure is signalled by clearing dbx_print_signalled_flag.
//  
//  static void dbx_print_handler(int, int, struct sigcontext *scp)
//  {
//  	  dbx_print_signalled_flag = false;			// Flag caller that an address access failed
//  	  scp->sc_pc = scp->sc_npc;					// Step the PC over the failed instruction
//  }

#ifndef _MSC_VER
//  
//  	Return true or false according to whether aPrint is a plausible PrimPrint derived class.
//  
//  	This routine catches 0 and silly addresses, but not non-PrimPrint objects.
//  
//  	0 is explicitly checked.
//  	Silly addresses cause BUS or SEGV violations when accessing the vtbl. These are caught.
//  	Objects without virtual tables tend not to have zeroes indirected off their pointer.
//  
//  	Objects with virtual tables pass ! An attempt to put a unique signature as the first entry
//  	in the virtual table failed because in a shared library implementation the entries are
//  	liable to be vectored, and the signature comparison fails.
//  
static bool dbx_print_validate(const void *aPrint)
{
 struct mptr {short d; short i; int (*f)(); };  //   The layout of a vtbl entry
 if (!aPrint)
 {
  std::cerr << "Attempted to dbx_print_??? a 0" << std::endl;
  return false;
 }
 const char *errTxt = 0;
 dbx_print_signalled_flag = true;
//  	  SIG_PF oldSigBus = signal(SIGBUS, (SIG_PF)&dbx_print_handler);
//  	  SIG_PF oldSigSegv = signal(SIGSEGV, (SIG_PF)&dbx_print_handler);
 PrimPrint refPrint;
 mptr *vtbl = *(mptr **)aPrint;
 mptr *refVtbl = *(mptr **)&refPrint;
 if ((vtbl[0].d != refVtbl[0].d) || (vtbl[0].i != refVtbl[0].i) || (vtbl[0].f != refVtbl[0].f))
  errTxt = "dbx_print requires an object with a virtual function table";
 if (!dbx_print_signalled_flag)
  errTxt = "dbx_print requires a valid address";
 if (errTxt)
  std::cerr << errTxt << std::endl;
//  	  signal(SIGBUS, oldSigBus);
//  	  signal(SIGSEGV, oldSigSegv);
 return !errTxt;
}
#else
inline static bool dbx_print_validate(const void *aPrint) { return true; }
#undef UNUSED
//#include <afx.h>
#undef max
#undef min
#endif

//  
//  	Debug routine for use with dbxtool. An operator<< is performed to a strstream buffer to
//  	identify the argument. The buffer is returned to be printed by dbxtool.
//  
const char *dbx_print_shallow(const PrimPrint *aPrint)
{
 if (!dbx_print_validate(aPrint))
  return "?";
 static char buf[1024];
 std::ostrstream s(buf, sizeof(buf));
 s << *aPrint << std::ends;
 return buf;
}

//  
//  	Debug routine for use with dbxtool. A print_on is performed to cerr to identify the argument.
//  	The resulting output may be massive. It cannot be returned to dbxtool since dbxtool has a
//  	maximum string length display constraint - specifically to prevent mega-string displays.
//  
const char *dbx_print_on(const PrimPrint *aPrint)
{
 if (dbx_print_validate(aPrint))
 {
#ifndef _MSC_VER
  aPrint->print_on(std::cerr);
#else
  std::ostrstream s;
  aPrint->print_on(s);
  s << std::ends;
  OutputDebugString(s.str());
  delete[] s.str();
#endif
 }
 return "";
}

//  
//  	Debug routine for use with dbxtool. A print_deep is performed to cerr to identify the argument.
//  	The resulting output may be massive. It cannot be returned to dbxtool since dbxtool has a
//  	maximum string length display constraint - specifically to prevent mega-string displays.
//  
const char *dbx_print_deep(const PrimPrint *aPrint)
{
 if (dbx_print_validate(aPrint))
 {
#ifndef _MSC_VER
  aPrint->print_deep(std::cerr);
#else
  std::ostrstream s;
  aPrint->print_deep(s);
  s << std::ends;
  OutputDebugString(s.str());
  delete[] s.str();
#endif
 }
 return "";
}

#endif
