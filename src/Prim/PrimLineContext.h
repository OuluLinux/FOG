
#ifndef PRIMLINECONTEXT_HXX
#define PRIMLINECONTEXT_HXX






//  
//  	A PrimLineContext maintains a source file context so that error messages can identify a sensible
//  	source context for their declarations.
//  
class PrimLineContext : public PrimPrint
{
 TYPEDECL_SINGLE(PrimLineContext, PrimPrint)
 PRIMSHARES_DECLS(PrimLineContext)
 PRIMREF_DECLS(PrimLineContext)
private:
 PrimStringHandle _file;      //   The source file.
 size_t _line;        //   The line number.
protected:
 PrimLineContext() {}
public:
 PrimLineContext(const char *fileName, size_t lineNumber);
 PrimLineContext(const PrimString& fileName, size_t lineNumber);
 const PrimString& file() const { return *_file; }
 size_t line() const { return _line; }
 virtual std::ostream& print_this(std::ostream& s) const;
 virtual std::ostream& print_viz(std::ostream& s) const;
};

inline void annul(PrimLineContext *lineContext) { if (lineContext) delete lineContext; }
#endif
 
