
#ifndef PRIMTEXT_HXX
#define PRIMTEXT_HXX




//  
//  	A PrimText contains an array of immutable text strings.
//  
class PrimText : public PrimPrint
{
 TYPEDECL_SINGLE(PrimText, PrimPrint)
 friend class PrimTextIterator;
protected:
 PrimStringHandle *_lines;   //   Pointer to array of text strings.
 size_t _tally;      //   Actual number of lines of text available.
 size_t _capacity;     //   Array dimension of _lines.

private:
 bool set_capacity(size_t numLines);

public:
 PrimText(size_t numLines = 0);
 PrimText(const PrimText& someText);
 PrimText(const char *const someTexts[], size_t numLines);
 virtual ~PrimText();
 PrimText& operator=(const char *aString);
 PrimText& operator=(const PrimString& aString);
 PrimText& operator=(const PrimText& someText);
//  	const PrimString *operator[](size_t i) const { return (i < _tally ? &_lines[i] : 0); }
//  	PrimString& operator[](size_t i) { return *_lines[i]; }
 const PrimString& operator[](size_t i) const { return *_lines[i]; }
//  
//  	Add an additional line of text following all previous lines.
//  
 void append(const char *someText) { append(*PrimStringHandle(someText)); }
 void append(const PrimString& aString);
 int compare(const PrimText& someText) const;
 bool contains(const PrimString& aString) const;
 void erase();
 void insert_above(const PrimString& aString, unsigned int anIndex = 0);
#ifndef NO_PRIM_STREAMS
 virtual std::ostream& print_depth(std::ostream& s, int aDepth = 0) const;
 virtual std::ostream& print_members(std::ostream& s, int aDepth = 0) const;
 virtual std::ostream& print_this(std::ostream& s) const;
#endif
 void sort(int (*compareRoutine)(const PrimStringHandle *v1, const PrimStringHandle *v2) = &PrimString::sort_compare);
//  
//  	Report the number of lines in the text array.
//  
 size_t tally() const { return (_tally); }
 bool write_to_file(const char *aFileName) const;
#ifndef NO_PRIM_STREAMS
 friend std::ostream& operator<<(std::ostream& s, const PrimText& someText);
#ifndef NO_FSTREAM_SUPPORT
 friend std::ofstream& operator<<(std::ofstream& s, const PrimText& someText);
 friend std::ifstream& operator>>(std::ifstream& s, PrimText& someText);
#endif
#endif
};

//  
//  	Define an iterator PrimTextIterator over a PrimText. The iterator hides
//  	the indirection implicit in the list implementation, such that *p, and p-> access the list content.
//  	Test of the iterator tests the iteration bounds. The iterator always points at an existent
//  	object, so code may safely inadvertently test out of bounds values. Use as:
//  
//  	for (PrimTextIterator p(aList); p; p++)
//  		p->do_something_const();
//  
class PrimTextIterator
{
private:
 const PrimStringHandle *_current;
 size_t _to_go;
private:
 inline const PrimStringHandle *null() { return &PrimStringHandle::null(); }
public:
 PrimTextIterator() : _current(null()), _to_go(0) {}
 PrimTextIterator(const PrimText& aList)
  : _current(aList.tally() ? aList._lines : null()), _to_go(aList.tally()) {}
 PrimTextIterator& operator=(const PrimText& aList)
  { _to_go = aList.tally(); _current = _to_go ? aList._lines : null(); return *this; }
 const PrimStringHandle& operator *() const { return *_current; }
 const PrimStringHandle *operator->() const { return _current; }
 void operator++() { if (_to_go > 0) _current = (--_to_go > 0) ? _current+1 : null(); }
 void operator++(int) { if (_to_go > 0) _current = (--_to_go > 0) ? _current+1 : null(); }
 operator const void *() const { return _to_go ? this : 0; }
 bool operator!() const { return _to_go ? false : true; }
 size_t to_go() const { return _to_go; }
};

#endif
 
