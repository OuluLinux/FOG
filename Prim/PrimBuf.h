
#ifndef PRIMBUF_HXX
#define PRIMBUF_HXX

#ifndef NO_PRIM_STREAMS
	
#endif


//   A PrimBuf defines the handle for a PrimBufRep in which the representation
//   of a shared buffer is stored.
//
//  .note cscrelations
//   PrimBuf is modelled after the Prim libraries PrimString) class.
//  .note csusum
//   Methods are provided to construct, copy, assign, compare, interrogate and print buffers.
//
class PrimBuf {
private:
	const PrimBufRep* _rep;        //   Pointer to representation, never 0.
	
private:
	//
	//  	Identify the representation bytes.
	//
	const char* bytes() const { return (_rep->bytes()); }
	
protected:
	PrimBuf(const PrimBufRep& aPrimBufRep);
	
public:
	//
	//  	Construct a default buffer, returning a size of 0.
	//
	PrimBuf() : _rep(&PrimBufRep::_null_rep) {}
	//
	//  	Construct a buffer from a null terminated string. If this fails the null buffer is created.
	//
	PrimBuf(const char* aBuf) : _rep(&PrimBufRep::construct(aBuf)) {}
	//
	//  	Construct from a non-null terminated string. If this fails the null buffer is created.
	//
	PrimBuf(const char* aBuf, size_t aBufLength) : _rep(&PrimBufRep::construct(aBuf, aBufLength)) {}
	PrimBuf(const PrimBuf& aPrimBuf);
	//
	//  	Cleanup by releasing the buffer representation on termination.
	//
	virtual ~PrimBuf() { delete _rep; }
	PrimBuf& operator=(const PrimBuf& aPrimBuf);
	int operator==(const PrimBuf& aPrimBuf) const;
	int operator==(const char* aBuf) const;
	//
	//   Report whether this buffer differs from aPrimBuf.
	//
	int operator!=(const PrimBuf& aPrimBuf) const { return (!(*this == aPrimBuf)); }
	//
	//   Report whether this buffer differs from a null terminated string.
	//
	int operator!=(const char* aBuf) const { return (!(*this == aBuf)); }
	//
	//  	Return the size of the buffer.
	//
	const size_t size() const { return (_rep->_size); }
#ifndef NO_PRIM_TYPEINFO
	//
	//  	 Report the class name for debug purposes.
	//
	const char* class_name() const { return "PrimBuf"; }
#endif
	//
	//  	Compare this PrimBuf with aPrimBuf.
	//
	int compare(const PrimBuf& aPrimBuf) const;
	unsigned int hash() const;
	//
	//  	Report whether this string has a 0 representation.
	//
	int is_null() const { return (_rep == &PrimBufRep::_null_rep); }
	//
	//  	Report whether this string is valid. That is whether it has a non-0 representation.
	//  	This method is used to distinguish invalid keys, when a PrimBuf is a dictionary key.
	//
	int is_valid() const { return (!is_null()); }
#ifndef NO_PRIM_STREAMS
	virtual std::ostream& print_class_name(std::ostream& s) const;
	virtual std::ostream& print_instance(std::ostream& s) const;
	friend std::ostream& operator<<(std::ostream& s, const PrimBuf& aBuf);
	friend std::istream& operator>>(std::istream& s, PrimBuf& aBuf);
#endif
};
#endif

