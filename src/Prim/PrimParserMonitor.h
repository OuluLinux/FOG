
#ifndef PRIMPARSERMONITOR_HXX
#define PRIMPARSERMONITOR_HXX



//  
//  	A PrimParserMonitor refines the inherited PrimMonitor behvaiour to support maintaining a current
//  	line context..
//  
class PrimParserMonitor : public PrimMonitor
{
 typedef PrimParserMonitor This;
 typedef PrimMonitor Super;
 TYPEDECL_SINGLE(This, Super)
private:
 static const PrimLineContext *_current_line_context; //   Prevailing line context.
protected:
 PrimParserMonitor();
 PrimParserMonitor(Install doInstall);
 virtual ~PrimParserMonitor();
public:
 static void adopt_current_line_context(const PrimLineContext *lineContext);
 static const PrimLineContext *current_line_context() { return _current_line_context; }
 static void set_current_line_context(const PrimLineContext *lineContext);
};
#endif
 
