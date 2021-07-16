#include <Fog/FogIncludeAll.h>














PRIM_WITH_REF(FogLexerInput_Element)

//  
//  	A Context provides a pointer to an Elemnt that represents a token source. The context maintains
//  	the saved state of the element for use by a remark(), and a back linkage to the previous context
//  	so that print_source can identify the current position.
//  
//  	Contexts may form a tree rooted at the context of the null Element constructed for the first Mark.
//  	Each branch of the tree represents a possible sequence of input sources. Marks identify positions
//  	in the tree with their own context defining the saved state at the identified position. Branching
//  	occurs when recursive mark()s occur before the marked source gas been exhausted.
//  
//  .note
//  	The _unget member is only needed in its entirety in Mark derived classes. Element derived classes
//  	just latch a copy, and so could perhaps be more efficient by sharing the contents keeping only a
//  	distinct threshold.
//  
class FogLexerInput_Context : public PrimPrint
{
 friend class FogLexerInput_Mark;
 typedef PrimPrint Super;
 typedef FogLexerInput_Context This;
 TYPEDECL_SINGLE(This, Super)
private:
 FogLexerInput_ElementRef _element;  //   Pointer to next (less-nested invoking) context,
 size_t _index;       //   Index to next token in _element.
 size_t _post_line_index;    //   Index of first token following Line, 0 undefined.
 FogLexerUnget _unget;     //   Backed up tokens w.r.t. _index.
private:
 This& operator=(const This&);
protected:
 FogLexerInput_Context();
 FogLexerInput_Context(const This& inputContext);
public:
 virtual ~FogLexerInput_Context();
 inline void adopt(FogLexerInput_Element *inputElement) { if (inputElement) _element.adopt(*inputElement); }
 FogLexerInput_Element& element() { return *_element; }
 const FogLexerInput_Element& element() const { return *_element; }
 bool has_unget() const { return _unget.is_full(); }
 size_t index() const { return _index; }
 bool is_empty() const;
 const char *peek(FogTokenRef& tokenValue, bool skipWhite);
 size_t post_line_index() const { return _post_line_index; }
 virtual std::ostream& print_depth(std::ostream& s, int aDepth) const;
 virtual std::ostream& print_members(std::ostream& s, int aDepth) const;
 virtual std::ostream& print_this(std::ostream& s) const;
 virtual std::ostream& print_viz(std::ostream& s) const;
 virtual void push_include(const PrimId& fileStem, const FogUtility& aUtility);
};

//  
//  	An Element defines a source of parsed tokens. Derived classes implement the different sources.
//  	The inherited PrimMonitor is used to establish the input source nesting on the stack so that
//  	error messages reflect the input nesting. Each Element has a Context that defines the saved
//  	state of the next input source, input from which has been suspended by the need to process
//  	input from this source.
//  
class FogLexerInput_Element : public FogLexerInput_Context
{
 typedef FogLexerInput_Context Super;
 typedef FogLexerInput_Element This;
 TYPEDECL_SINGLE(This, Super)
 PRIMSHARES_DECLS(This)
 PRIMREF_DECLS(This)
private:
 FogLexerInput_Element(const This&);
 This& operator=(const This&);
 This& mutate() const { return *(This *)this; }
protected:
 FogLexerInput_Element() {}
 FogLexerInput_Element(FogLexerInput_Mark& topMark);
 virtual ~FogLexerInput_Element() {}
public:
 virtual void clear_done() = 0;
 virtual bool done(const FogLexerInput_Context& aContext) const = 0;
 virtual const FogMetaSlot *find(const PrimId& anId) const { return 0; }
 virtual int get(FogTokenContext& tokenContext, size_t tokenIndex, size_t postLineIndex) = 0;
 virtual const char *peek(FogTokenRef& tokenValue, size_t nextIndex, bool skipWhite) = 0;
 virtual std::ostream& print_message(std::ostream& s, const FogLexerInput_Context& aContext) const { return s; }
 virtual std::ostream& print_message_line(std::ostream& s, int aDepth) const { return s; }
 virtual void set_done(const FogLexerInput_Context& aContext) const = 0;
};

//  
//  	A Notice buffers the context of an error or warning message until such time as the message can be
//  	emitted when all marks have been unmarked.
//  
class FogLexerInput_Notice : public FogLexerInput_Context
{
 typedef FogLexerInput_Context Super;
 typedef FogLexerInput_Notice This;
 friend class FogLexerInput;
 TYPEDECL_SINGLE(This, Super)
private:
 PrimAdopted<FogLexerInput_Notice> _notice;  //   Pointer to next notice.
 const PrimStringHandle _msg;     //   Diagnostic text recorded as notice.
 const bool _is_error;       //   True for an error/false for a warning.
private:
 FogLexerInput_Notice(const This&);
 This& operator=(const This&);
private:
 void post(const FogLexerInput_Context *contextList[], size_t anIndex) const;
public:
 FogLexerInput_Notice(const FogLexerInput_Mark& aContext, const char *aMessage, bool isError);
 PrimAdopted<FogLexerInput_Notice>& next() { return _notice; }
 void post();
};

//  
//  	A Mark defines a marked position in the input token sequence. Mark's form a linear stack
//  	pushed by FogLexerInput::mark(), rewound to a marked position be FogLexerInput::remark()
//  	and popped by FogLexerInput::unmark().
//  
//  	As a result of token lookahead intrinsic to a one token lookahead parser, and peeking required
//  	to support parser concatenation, the sequencing of Mark pushes and pops has no guaranteed correlation
//  	with Element pushes and pops. Hence the need for a Mark to point into a tree of Element's rather than
//  	a list. The tree is self pruning as a result of the use of share counts on Element's.
//  
class FogLexerInput_Mark : public FogLexerInput_Context
{
 typedef FogLexerInput_Context Super;
 typedef FogLexerInput_Mark This;
 friend class FogLexerInput;
 TYPEDECL_SINGLE(This, Super)
private:
 PrimAdopted<FogLexerInput_Mark> _mark;   //   Pointer to next marked context.
 PrimAdopted<FogLexerInput_Notice> _notice;  //   Error and warning notices caught by this mark.
 size_t _token_count;       //   Tokens parsed since the mark.
 PrimAdopted<FogLexerInput_Notice> _best_notice; //   Best error and warning notices caught by this mark or remark.
 size_t _best_token_count;      //   Best count of tokens parsed since the mark or remark..
 size_t _garbage_threshold;      //   Tokens garbage collection threshold
private:
 FogLexerInput_Mark(const This&);
 This& operator=(const This&);
public:
 FogLexerInput_Mark();
 FogLexerInput_Mark(PrimAdopted<FogLexerInput_Mark> topOfStack, size_t garbageThreshold);
 const char *get(FogTokenContext& tokenContext);
 PrimAdopted<FogLexerInput_Notice>& notice_tail();
 void pop();
 virtual std::ostream& print_depth(std::ostream& s, int aDepth) const;
 virtual std::ostream& print_members(std::ostream& s, int aDepth) const;
 std::ostream& print_message_line(std::ostream& s, int aDepth) const;
 void remark(const FogLexerInput_Mark& inputContext);
 void rewind();
 bool unget(const FogTokenContext& tokenContext, bool wsNeeded) { return _unget.unget(tokenContext, wsNeeded); }
};
 
TYPEINFO_SINGLE(FogLexerInput, Super)

FogLexerInput::FogLexerInput(const FogToken& aToken)
:
 _token(aToken),
 _mark(new FogLexerInput_Mark)
{
 _garbage.set_capacity(128);   //   Re-entrant lexers are small, so don't be too enthusiastic.
}

FogLexerInput::~FogLexerInput()
{
 if (_mark && _mark->_notice)
 {
  PrimAdopted<FogLexerInput_Notice> p = _mark->_notice;
  _mark.reset();
  ERRMSG("BUG -- should not have residual notices in FogLexerInput::~FogLexerInput.");
  for ( ; p; p = p->next())
   p->post();
 }
}

const FogMetaSlot *FogLexerInput::find(const PrimId& anId) const
{
 return _mark ? _mark->element().find(anId) : 0;
}

void FogLexerInput::garbage_collect(size_t garbageThreshold)
{
 for (size_t i = _garbage.tally(); i-- > garbageThreshold; )
  _garbage.remove(i);
}

const char *FogLexerInput::get(FogTokenContext& tokenContext)
{
 FogLexerInput_Mark *p = _mark.get();
 if (!p)
 {
  ERRMSG("BUG - no mark for FogLexerInput::get.");
  return 0;
 }
 for ( ; !p->is_empty(); p->pop())
 {
  const char *stateName = p->get(tokenContext);
  if (stateName)
  {
   if (tokenContext.token().is_line() && _token.is_null())
    clear_done();
   p->_token_count++;
   return stateName;
  }
 }
 return 0;
}

void FogLexerInput::mark()
{
 if (!_mark)
  ERRMSG("BUG - no mark for FogLexerInput::mark.");
 if (Fog::debug_lex2main())
  print_token("<INPUT-MARK>", 0);
 _mark.reset(new FogLexerInput_Mark(_mark, _garbage.tally()));
}

//  
//  	Peek the next character within the context of aLexer to tokenValue, returning a non-zero diagnostic
//  	source indication if one to peek.
//  
const char *FogLexerInput::peek(FogTokenRef& tokenValue, bool skipWhite)
{
 FogLexerInput_Context *p = _mark.get();
 if (!p)
 {
  ERRMSG("BUG - no mark for FogLexerInput::peek.");
  return 0;
 }
 for ( ; !p->is_empty(); p = &p->element())
 {
  const char *stateName = p->peek(tokenValue, skipWhite);
  if (stateName)
   return stateName;
 }
 return 0;
}

bool FogLexerInput::post_error(const PrimNotice_Error& aNotice) const
{
 if (_mark && _mark->_mark)
 {
  mutate()._mark->notice_tail().reset(new FogLexerInput_Notice(*_mark, aNotice.message(), true));
  return true;
 }
 const PrimNotifier *thisNotifier = this;    //   Visual C++ 6 workaround.
 if (thisNotifier != global_notice_board())
  return global_notice_board()->post_error(aNotice);
 else
  return Super::post_error(aNotice);
}

bool FogLexerInput::post_warning(const PrimNotice_Warning& aNotice) const
{
 if (_mark && _mark->_mark)
 {
  mutate()._mark->notice_tail().reset(new FogLexerInput_Notice(*_mark, aNotice.message(), false));
  return true;
 }
 const PrimNotifier *thisNotifier = this;    //   Visual C++ 6 workaround.
 if (thisNotifier != global_notice_board())
  return global_notice_board()->post_warning(aNotice);
 else
  return Super::post_warning(aNotice);
}

std::ostream& FogLexerInput::print_depth(std::ostream& s, int aDepth) const
{
 for (const FogLexerInput_Mark *p = _mark.get(); p; p = p->_mark.get())
 {
  int i = 1;
  for (const FogLexerInput_Context *q = p; true; i++)
  {
   q->print_deep(s, aDepth + i);
   const FogLexerInput_Element& nextElement = q->element();
   if (nextElement.is_null())
    break;
   q = &nextElement;
  }
 }
 return Super::print_depth(s, aDepth);
}

std::ostream& FogLexerInput::print_members(std::ostream& s, int aDepth) const
{
 for (const FogLexerInput_Mark *p = _mark.get(); p; p = p->_mark.get())
 {
  int i = 1;
  for (const FogLexerInput_Context *q = p; true; i++)
  {
   q->print_on(s, aDepth+i);
   const FogLexerInput_Element& nextElement = q->element();
   if (nextElement.is_null())
    break;
   q = &nextElement;
  }
 }
 return Super::print_members(s, aDepth);
}

std::ostream& FogLexerInput::print_message_line(std::ostream& s, int aDepth) const
{
 if (!_token.is_null())
 {
  const FogLine *aLine = _token.is_line();
  if (aLine)
   s << indent(aDepth) << "At " << viz(*aLine) << ": " << aLine->text() << std::endl;
  else
   s << indent(aDepth) << "In " << viz(_token) << std::endl;
 }
 else if (_mark)
    {
        if (!_mark->element().is_null())
      _mark->print_message_line(s, aDepth);
        else
   s << indent(aDepth) << "At End of File" << std::endl;
    }
 return s;
}

std::ostream& FogLexerInput::print_this(std::ostream& s) const
{
 if (!_token.is_null())
  _token.print_viz(s);
 return s;
}

//  
//  	Set-up the inclusion for a using "file-name". This is named as if it were a nomal recursion, but is currently
//  	implemented at the Cpp level. Surely a bug that should be resolved by a nested lexer?
//  
void FogLexerInput::push_include(const PrimId& fileStem, const FogUtility& aUtility)
{
 if (!_mark)
  ERRMSG("BUG - no mark for FogLexerInput::push_include.");
 else
  _mark->push_include(fileStem, aUtility);
}

//  
//  	Restore the marked context as the current context.
//  
void FogLexerInput::remark()
{
 if (!_mark)
  ERRMSG("BUG - no mark for FogLexerInput::remark.");
 else if (!_mark->_mark)
  ERRMSG("BUG - no marked mark for FogLexerInput::remark.");
 else
 {
  if (Fog::debug_lex2main())
   print_token("<INPUT-REMARK>", 0);
  const size_t garbageThreshold = _mark->_garbage_threshold;
  _mark->remark(*_mark->_mark);
  garbage_collect(garbageThreshold);
 }
}

//  
//  	Mark the most recent returned token as unconsumed and therefore to be returned again by the next call
//  	to get().
//  
void FogLexerInput::unget(const FogTokenContext& tokenContext, bool wsNeeded)
{
 if (!_mark)
  ERRMSG("BUG - no mark for FogLexerInput::unget.");
 else
 {
  if (Fog::debug_lex2main())
   print_token(wsNeeded ? "<INPUT-UNGET>" : "<INPUT-UNGET> >> WS", &tokenContext.token());
  _mark->unget(tokenContext, wsNeeded);
 }
}

//  
//  	Unmarking preserves the context of the top stack entry, discarding the source conrtext of the
//  	second entry (that contained the state for restoration by remark). Any errors and warnings
//  	accrued by the two contexts are merged. If the unmark stack has emptied the suppressed
//  	errors and warnings are emitted.
//  
void FogLexerInput::unmark()
{
 if (!_mark)
  ERRMSG("BUG - no mark for FogLexerInput::unmark.");
 else if (!_mark->_mark)
  ERRMSG("BUG - no marked mark for FogLexerInput::unmark.");
 else
 {
  if (Fog::debug_lex2main())
   print_token("<INPUT-UNMARK>", 0);
  const size_t garbageThreshold = _mark->_garbage_threshold;
  if (!_mark->_notice)          //   If unmarked context is error free.
   ;              //    leave it error free.
  else if (_mark->_best_notice && (_mark->_token_count < _mark->_best_token_count)) //   If alternate parse was longer.
  {               //    restore longer parse context.
   _mark->_token_count = _mark->_best_token_count;
   _mark->_notice = _mark->_best_notice;
  }
  _mark->notice_tail() = _mark->_mark->_notice;  
  _mark->_mark = _mark->_mark->_mark;
  if (!_mark->_mark)
   for (PrimAdopted<FogLexerInput_Notice> p = _mark->_notice; p; p = p->next())
    p->post();
  garbage_collect(garbageThreshold);
 }
}
 
TYPEINFO_SINGLE(FogLexerInput_Context, Super)

//  
//  	Construct the null context.
//  
FogLexerInput_Context::FogLexerInput_Context()
:
 _index(0),
 _post_line_index(0)
{}

//  
//  	Construct a copy of a context.
//  
FogLexerInput_Context::FogLexerInput_Context(const This& inputContext)
:
 _element(inputContext._element),
 _index(inputContext._index),
 _post_line_index(inputContext._post_line_index),
 _unget(inputContext._unget)
{}

FogLexerInput_Context::~FogLexerInput_Context() {}

bool FogLexerInput_Context::is_empty() const { return _element->is_null() && !_unget.is_full(); }

const char *FogLexerInput_Context::peek(FogTokenRef& tokenValue, bool skipWhite)
{
 if (_unget.peek(tokenValue, skipWhite))
  return "<INPUT-REPEEK>";
 return _element->peek(tokenValue, _index, skipWhite);
}

std::ostream& FogLexerInput_Context::print_depth(std::ostream& s, int aDepth) const
{
//  	s << indent(aDepth) << "line_index = " << _post_line_index << ", index = " << _index
//  		<< ", unget = " << _unget << '\n';
 if (_element)
 {
  _element->print_deep(s, aDepth);
  s << indent(aDepth+1);
  _element->print_message(s, *this);
  s << '\n';
 }
 return s;
}

std::ostream& FogLexerInput_Context::print_members(std::ostream& s, int aDepth) const
{
//  	s << indent(aDepth) << "line_index = " << _post_line_index << ", index = " << _index
//  		<< ", unget = " << _unget << '\n';
 if (_element)
 {
  _element->print_on(s, aDepth);
  s << indent(aDepth+1);
  _element->print_message(s, *this);
  s << '\n';
 }
 return s;
}

std::ostream& FogLexerInput_Context::print_this(std::ostream& s) const
{
 s << _unget << ", ";
 return _element->print_message(s, *this);
}

std::ostream& FogLexerInput_Context::print_viz(std::ostream& s) const
{
 return print_instance(s);
}

void FogLexerInput_Context::push_include(const PrimId& fileStem, const FogUtility& aUtility)
{
 if (!_element)
  ERRMSG("BUG - should not FogLexerInput_Context::push_include(" << fileStem << ").");
 else
  _element->push_include(fileStem, aUtility);
}
 
TYPEINFO_SINGLE(FogLexerInput_Element, Super)
PRIMREF_NULL_CLAS(FogLexerInput_Element)
 virtual void clear_done() {}
 virtual bool done(const FogLexerInput_Context& aContext) const { return false; }
 virtual int get(FogTokenContext& tokenContext, size_t tokenIndex, size_t postLineIndex) { return -1; }
 virtual const char *peek(FogTokenRef& tokenValue, size_t nextIndex, bool skipWhite) { return 0; }
 virtual void set_done(const FogLexerInput_Context& aContext) const {}
};
PRIMREF_NULL_IMPL(FogLexerInput_Element)
PRIMSHARES_IMPL(FogLexerInput_Element)
PRIMREFS_IMPL(FogLexerInput_Element)
TMPL_HACK_FIX_DO(FogLexerInput_Element)

FogLexerInput_Element::FogLexerInput_Element(FogLexerInput_Mark& topMark)
:
 Super(topMark)
{
 topMark.rewind();
}
 
TYPEINFO_SINGLE(FogLexerInput_Mark, Super)

FogLexerInput_Mark::FogLexerInput_Mark()
:
 _token_count(0),
 _best_token_count(0),
 _garbage_threshold(0)
{}

FogLexerInput_Mark::FogLexerInput_Mark(PrimAdopted<FogLexerInput_Mark> topOfStack, size_t garbageThreshold)
:
 Super(*topOfStack),
 _mark(topOfStack),
 _token_count(0),
 _best_token_count(0),
 _garbage_threshold(garbageThreshold)
{}

const char *FogLexerInput_Mark::get(FogTokenContext& tokenContext)
{
 if (_unget.get(tokenContext))
  return "<INPUT-REGET>";
 int inputIndex = _element->get(tokenContext, _index, _post_line_index);
 if (inputIndex < 0)
  return 0;
 _index = inputIndex;
 if (tokenContext.token().is_line())
 {
  _post_line_index = _index;
  _element->clear_done();
 }
 return "<INPUT-GET>";
}

PrimAdopted<FogLexerInput_Notice>& FogLexerInput_Mark::notice_tail()
{
 PrimAdopted<FogLexerInput_Notice> *p = &_notice; 
 while (*p)
  p = &(*p)->next();
 return *p;
}

//  
//  	Eliminate the top input stack element.
//  
void FogLexerInput_Mark::pop()
{
 FogLexerInput_Context& thatInput = *_element;
 _index = thatInput._index;
 _post_line_index = thatInput._post_line_index;
 _unget = thatInput._unget;
 _element = thatInput._element;
}

std::ostream& FogLexerInput_Mark::print_depth(std::ostream& s, int aDepth) const
{
    Super::print_depth(s, aDepth);
 if (_notice)
  _notice->print_deep(s, aDepth);
 return s;
}

std::ostream& FogLexerInput_Mark::print_members(std::ostream& s, int aDepth) const
{
    Super::print_members(s, aDepth);
 if (_notice)
  _notice->print_on(s, aDepth);
 return s;
}

std::ostream& FogLexerInput_Mark::print_message_line(std::ostream& s, int aDepth) const
{
 return _element->print_message_line(s, aDepth);
}

void FogLexerInput_Mark::remark(const FogLexerInput_Mark& inputMark)
{
 _index = inputMark._index;
 _post_line_index = inputMark._post_line_index;
 _unget = inputMark._unget;
 _element = inputMark._element;
 if (!_best_notice || (_token_count > _best_token_count))
 {
  _best_token_count = _token_count;
  _best_notice = _notice;
 }
 _notice.reset();
 _token_count = 0;
}

void FogLexerInput_Mark::rewind()
{
 _element.reset();
 _index = 0;
 _post_line_index = 0;
 _unget.reset();
}
 
TYPEINFO_SINGLE(FogLexerInput_Notice, Super)

FogLexerInput_Notice::FogLexerInput_Notice(const FogLexerInput_Mark& aContext,
 const char *aMessage, bool isError)
:
 Super(aContext),
 _msg(aMessage),
 _is_error(isError)
{}

//  
//  	Post up the notice with its context, creating PrimTextMonitor's for each context level to provide
//  	consistent indentation. The list linkage is unfortunately in the wrong order, so a local list
//  	is created and then recursively scanned in reverse.
//  
void FogLexerInput_Notice::post()
{
 const FogLexerInput_Context *contextList[64];
 size_t i = 0;
 for (const FogLexerInput_Context *p = this; !p->element().is_null() && (i < 64); p = &p->element())
  contextList[i++] = p;
 post(contextList, i);
}

void FogLexerInput_Notice::post(const FogLexerInput_Context *contextList[], size_t anIndex) const
{
 if (anIndex == 0)
 {
  if (_is_error)
   ERRMSGZ(_msg.str());
  else  
   WRNMSGZ(_msg.str());
  return;
 }
 const FogLexerInput_Context& aContext = *contextList[anIndex-1];
 const FogLexerInput_Element& anElement = aContext.element();
 if (anElement.done(aContext))
 {
  PrimTextMonitor atMonitor(0);
  post(contextList, anIndex-1);
 }
 else
 {
  PrimOstrstream s;
  anElement.print_message(s, aContext);
  anElement.set_done(aContext);
  PrimTextMonitor atMonitor(s.str());
  post(contextList, anIndex-1);
 }
}
 
//  
//  	A FogLexerInput_Stream manages source tokens from the C preprocessor input stream.
//  
class FogLexerInput_Stream : public FogLexerInput_Element
{
 typedef FogLexerInput_Stream This;
 typedef FogLexerInput_Element Super;
 TYPEDECL_SINGLE(FogLexerInput_Stream, Super)
private:
 FogLexerCpp _lexer;        //   The C preprocessor input lexer.
 FogTokenListOfRef _contents;     //   The recorded token stream.
 FogLineRefToConst _start_line;     //   Starting line context, when no _post_line_index.
 size_t _start_column;       //   Starting column context, when no _post_line_index.
 mutable FogLineRefToConst _done_line;   //   Context of most recent
 mutable size_t _done_column;     //    set done
private:
 size_t column(const FogLexerInput_Context& aContext) const;
 const FogLine& line(const FogLexerInput_Context& aContext) const;
 This& mutate() const { return *(This *)this; }
public:
 FogLexerInput_Stream(FogLexerInput_Mark& topMark, FogReader& aReader,
  std::istream& inputStream, const FogSourceFile& sourceFile);
 virtual void clear_done() { _done_line.reset(); _done_column = 0; }
 virtual bool done(const FogLexerInput_Context& aContext) const
  { return (_done_line.pointer() == &line(aContext)) && (_done_column == column(aContext)); }
 virtual int get(FogTokenContext& tokenContext, size_t tokenIndex, size_t postLineIndex);
 virtual const char *peek(FogTokenRef& tokenValue, size_t nextIndex, bool skipWhite);
 virtual std::ostream& print_depth(std::ostream& s, int aDepth) const
  { Super::print_depth(s, aDepth); _contents.print_on(s, aDepth); return s; }
 virtual std::ostream& print_message(std::ostream& s, const FogLexerInput_Context& aContext) const;
 virtual std::ostream& print_message_line(std::ostream& s, int aDepth) const
  { return _lexer.print_message_line(s, aDepth); }
 virtual std::ostream& print_this(std::ostream& s) const { return _lexer.print_this(s); }
 virtual void push_include(const PrimId& fileStem, const FogUtility& aUtility) { _lexer.include(fileStem, aUtility); }
 virtual void set_done(const FogLexerInput_Context& aContext) const
  { mutate()._done_line = line(aContext); mutate()._done_column = column(aContext); }
};

void FogLexerInput::push_stream(FogReader& aReader, std::istream& inputStream, const FogSourceFile& sourceFile)
{
 _mark->adopt(new FogLexerInput_Stream(*_mark, aReader, inputStream, sourceFile));
}

TYPEINFO_SINGLE(FogLexerInput_Stream, Super)

FogLexerInput_Stream::FogLexerInput_Stream(FogLexerInput_Mark& topMark, FogReader& aReader,
  std::istream& inputStream, const FogSourceFile& sourceFile)
:
 Super(topMark),
 _lexer(aReader, inputStream, sourceFile),
//  	_contents(FogList::IS_RAW),
 _start_column(0),
 _done_column(0)
{}

size_t FogLexerInput_Stream::column(const FogLexerInput_Context& aContext) const
{
 size_t postLineIndex = aContext.post_line_index();
 size_t sourceCols = postLineIndex > 1 ? 0 : _start_column;
 const size_t maxIndex = aContext.index();
 for (size_t anIndex = postLineIndex; anIndex < maxIndex; anIndex++)
 {
  const FogToken& aToken = *_contents[anIndex];
  sourceCols += aToken.source_columns();
 }
 return sourceCols;
}

int FogLexerInput_Stream::get(FogTokenContext& tokenContext, size_t tokenIndex, size_t postLineIndex)
{
 if (tokenIndex < _contents.tally())       //   If replaying.
 {
  if (postLineIndex)
   tokenContext.assign(_contents[postLineIndex-1]->line(), 0, *_contents[tokenIndex++]);
  else
   tokenContext.assign(*_contents[tokenIndex++]);
  return tokenIndex;
 }
 if (!_lexer.get(tokenContext))
  return -1;
 if (shares() <= 1)           //   If not recording.
 {
  if (_contents.tally())
   _contents.vacate();
  _start_line = tokenContext.line();
  _start_column = tokenContext.column();
  return 0;
 }
 if (!_contents.tally() && !tokenContext.token().is_line())   //   If first recorded character and it's not a line
 {
  _contents.add(tokenContext.line());      //   Start recording with line context.
  _start_column = tokenContext.column();
 }
 _contents.add(tokenContext.token());
 return _contents.tally();
}

const FogLine& FogLexerInput_Stream::line(const FogLexerInput_Context& aContext) const
{
 size_t postLineIndex = aContext.post_line_index();
 return postLineIndex && _contents.tally() ? _contents[postLineIndex-1]->line() : *_start_line;
}

const char *FogLexerInput_Stream::peek(FogTokenRef& tokenValue, size_t nextIndex, bool skipWhite)
{
 while (nextIndex < _contents.tally())       //   If replaying.
 {
  tokenValue = _contents[nextIndex++];
  if (!tokenValue->is_skip(skipWhite))
   return "<INPUT_REPEEK>";
 }
 return _lexer.peek(tokenValue, skipWhite);
}

std::ostream& FogLexerInput_Stream::print_message(std::ostream& s, const FogLexerInput_Context& aContext) const
{
 const FogLine& aLine = line(aContext);
 size_t sourceCols = column(aContext);
 return s << "At " << viz(aLine) << '.' << sourceCols << ": " << aLine.text();
}
 
//  
//  	A FogLexerInput_Token manages a source token from an evaluated built in function.
//  
class FogLexerInput_Token : public FogLexerInput_Element
{
 typedef FogLexerInput_Token This;
 typedef FogLexerInput_Element Super;
 TYPEDECL_SINGLE(FogLexerInput_Tokens, Super)
private:
 const FogToken &_token;      //   Declaration or entity to be expanded.
 const FogTokenRef _token_value;    //   The resolved value.
 mutable bool _done;       //   True once emitted.
private:
 This& mutate() const { return *(This *)this; }
public:
 FogLexerInput_Token(FogLexerInput_Mark& topMark, const FogToken& inToken, FogToken& theToken)
  : Super(topMark), _token(inToken), _token_value(theToken), _done(false) {}
 virtual void clear_done() { _done = false; }
 virtual bool done(const FogLexerInput_Context& aContext) const { return _done; }
 virtual int get(FogTokenContext& tokenContext, size_t tokenIndex, size_t postLineIndex);
 virtual const char *peek(FogTokenRef& tokenValue, size_t nextIndex, bool skipWhite);
 virtual std::ostream& print_message(std::ostream& s, const FogLexerInput_Context& aContext) const;
 virtual std::ostream& print_message_line(std::ostream& s, int aDepth) const;
 virtual std::ostream& print_this(std::ostream& s) const { return s << _token_value; }
 virtual void set_done(const FogLexerInput_Context& aContext) const { mutate()._done = true; }
};

void FogLexerInput::push_token(const FogToken& inToken, FogToken& theToken)
{
 _mark->adopt(new FogLexerInput_Token(*_mark, inToken, theToken));
}

TYPEINFO_SINGLE(FogLexerInput_Token, Super)

int FogLexerInput_Token::get(FogTokenContext& tokenContext, size_t tokenIndex, size_t postLineIndex)
{
 switch (tokenIndex)
 {
  case 0:
   if (!_token_value->is_null())  //   null token gets pushed as an argument to AS_VOID
   {
    tokenContext.assign(*_token_value);
    return 1;
   }
   /*   drop-through */
  case 1:
   tokenContext.assign(FogTokenType::RAMMARG);
   return 2;
  default:
   return -1;
 }
}

const char *FogLexerInput_Token::peek(FogTokenRef& tokenValue, size_t tokenIndex, bool skipWhite)
{
 if (tokenIndex > 1)
  return 0;
 if (tokenIndex == 1)
  tokenValue = FogTokenType::get(FogTokenType::RAMMARG);
 else
 {
  FogToken& theToken = *_token_value;
  if (!theToken.is_skip(skipWhite) && !theToken.is_null())
   tokenValue.assign(theToken);
  else
   tokenValue = FogTokenType::get(FogTokenType::RAMMARG);
 }
 return "<INPUT-PEEK>";
}

std::ostream& FogLexerInput_Token::print_message(std::ostream& s, const FogLexerInput_Context& aContext) const
{
 s << "Expanding " << viz(_token) << " to " << _token_value;
 return s;
}

std::ostream& FogLexerInput_Token::print_message_line(std::ostream& s, int aDepth) const
{
 s << indent(aDepth) << "Expanding " << viz(_token) << " to " << _token_value << std::endl;
 return s;
}
 
//  
//  	A FogLexerInput_Tokens manages source tokens from a function body or braced variable initialiser.
//  
class FogLexerInput_Tokens : public FogLexerInput_Element
{
 typedef FogLexerInput_Tokens This;
 typedef FogLexerInput_Element Super;
 TYPEDECL_SINGLE(FogLexerInput_Tokens, Super)
private:
 const FogToken& _token;       //   Declaration or entity to be expanded.
 const FogTokenListOfRef& _tokens;    //   The function body.
 mutable FogLineRefToConst _done_line;   //   Context of most recent
 mutable FogTokenRefToConst _done_token;   //    set done
private:
 This& mutate() const { return *(This *)this; }
protected:
 const FogToken& token() const { return _token; }
public:
 FogLexerInput_Tokens(FogLexerInput_Mark& topMark, const FogToken& aToken,
  const FogTokenListOfRef& tokenValues) : Super(topMark), _token(aToken), _tokens(tokenValues) {}
 virtual void clear_done() { _done_line.reset(); _done_token.reset(); }
 virtual bool done(const FogLexerInput_Context& aContext) const;
 virtual int get(FogTokenContext& tokenContext, size_t tokenIndex, size_t postLineIndex);
 virtual const char *peek(FogTokenRef& tokenValue, size_t nextIndex, bool skipWhite);
 virtual std::ostream& print_depth(std::ostream& s, int aDepth) const
  { Super::print_depth(s, aDepth); _tokens.print_on(s, aDepth); return s; }
 virtual std::ostream& print_message_line(std::ostream& s, int aDepth) const;
 virtual std::ostream& print_message(std::ostream& s, const FogLexerInput_Context& aContext) const;
 virtual std::ostream& print_this(std::ostream& s) const { return s << _token; }
 virtual void set_done(const FogLexerInput_Context& aContext) const;
};

void FogLexerInput::push_tokens(const FogToken& aToken, const FogTokenListOfRef& tokenValues)
{
 _mark->adopt(new FogLexerInput_Tokens(*_mark, aToken, tokenValues));
}

TYPEINFO_SINGLE(FogLexerInput_Tokens, Super)

bool FogLexerInput_Tokens::done(const FogLexerInput_Context& aContext) const
{
 size_t postLineIndex = aContext.post_line_index();
 if (postLineIndex)
  return _done_line.pointer() == &_tokens[postLineIndex-1]->line();
 const FogLine *aLine = _token.is_line();
 if (aLine)
  return _done_line.pointer() == aLine;
 else
  return _done_token.pointer() == &_token;
}

int FogLexerInput_Tokens::get(FogTokenContext& tokenContext, size_t tokenIndex, size_t postLineIndex)
{
 if (tokenIndex > _tokens.tally())
  return -1;
 FogToken& theToken = (tokenIndex < _tokens.tally()) ? *_tokens[tokenIndex] : FogTokenType::get(FogTokenType::RAMMARG);
 if (postLineIndex)
  tokenContext.assign(_tokens[postLineIndex-1]->line(), 0, theToken);
 else
  tokenContext.assign(theToken);
 return tokenIndex + 1;
}

const char *FogLexerInput_Tokens::peek(FogTokenRef& tokenValue, size_t nextIndex, bool skipWhite)
{
 if (nextIndex > _tokens.tally())
  return 0;
 for (size_t i = nextIndex; i < _tokens.tally(); ++i)
 {
  FogToken& theToken = *_tokens[i];
  if (!theToken.is_skip(skipWhite))
  {
   tokenValue.assign(theToken);
   return "<INPUT-PEEK>";
  }
 }
 tokenValue = FogTokenType::get(FogTokenType::RAMMARG);
 return "<INPUT-PEEK>";
}

std::ostream& FogLexerInput_Tokens::print_message(std::ostream& s, const FogLexerInput_Context& aContext) const
{
 size_t postLineIndex = aContext.post_line_index();
 if (postLineIndex)
  s << "At " << viz(_tokens[postLineIndex-1]->line());
 else
 {
  const FogLine *aLine = _token.is_line();
  if (aLine)
   s << "At " << viz(*aLine) << ": " << aLine->text();
  else
   s << "In " << viz(_token);
 }
//  	if (postLineIndex)
//  	{
//  		size_t sourceCols = /* postLineIndex ? theTokens[postLineIndex-1].columns() : */ 0;
//  		size_t anIndex = postLineIndex;
//  		const size_t maxIndex = _tokens.tally();
//  		for ( ; anIndex < maxIndex; anIndex++)
//  		{
//  			const FogToken& tokenValue = *_tokens[anIndex];
//  			sourceCols += tokenValue.source_columns();
//  		}
//  		s << '.' << sourceCols;
//  	}
 if (postLineIndex)
 {
  s << ": ";
  size_t anIndex = postLineIndex;
  const size_t maxIndex = _tokens.tally();
  size_t endIndex = maxIndex;
  if (aContext.index() < endIndex)
   endIndex = aContext.index();
  if (aContext.has_unget() && (postLineIndex < endIndex))
   endIndex--;
  for ( ; anIndex < endIndex; anIndex++)
  {
   const FogToken& tokenValue = *_tokens[anIndex];
   tokenValue.print_source(s, 0);
  }
  s << " <*> ";
  for ( ; anIndex < maxIndex; anIndex++)
  {
   const FogToken& tokenValue = *_tokens[anIndex];
   if (tokenValue.is_line())
    break;
   tokenValue.print_source(s, 0);
  }
 }
 return s;
}

std::ostream& FogLexerInput_Tokens::print_message_line(std::ostream& s, int aDepth) const
{
 if (!element().is_null())
  return element().print_message_line(s, aDepth);     //   Calling context more interesting
 const FogLine *aLine = _token.is_line();
 if (aLine)
  s << indent(aDepth) << "At " << viz(*aLine) << ": " << aLine->text() << std::endl;
 else
  s << indent(aDepth) << "In " << viz(_token) << std::endl;
 return s;
}

void FogLexerInput_Tokens::set_done(const FogLexerInput_Context& aContext) const
{
 size_t postLineIndex = aContext.post_line_index();
 if (postLineIndex)
 {
  mutate()._done_line = _tokens[postLineIndex-1]->line();
  mutate()._done_token.reset();
  return;
 }
 const FogLine *aLine = _token.is_line();
 if (aLine)
 {
  mutate()._done_line = aLine;
  mutate()._done_token.reset();
  return;
 }
 else
 {
  mutate()._done_line.reset();
  mutate()._done_token = _token;
  return;
 }
}
