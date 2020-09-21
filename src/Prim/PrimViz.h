#ifndef PRIMVIZ_H
#define PRIMVIZ_H



#ifndef NO_PRIM_STREAMS




//
//  	A PrimViz<T> is an ostream manipulator class used to enable T::print_viz to be invoked
//  	within a stream output sequence.
//  .note tutorial
//  	A PrimViz<T> may be used in the following way:
//
//  	s << "Unable to create " << viz(aName) << " for output.";
//
template <class T>

class PrimViz {

private:
	const T& _client;       //   Client to be print_viz'd.
	
public:
//
//  	Construct a print_viz'er for aClient.
//
	//friend inline PrimViz<T> viz(const T& aClient);
	
//
//  	Construct a print_viz'er for aClient.
//
	PrimViz(const T& aClient) : _client(aClient) {}
	
public:
	const T& client() const {
		return _client;
	}
	
//
//  	Invoke print_viz(s) for aPrimViz._client.
//
	friend inline std::ostream& operator<<(std::ostream& s, const PrimViz<T>& aPrimViz) {
		return aPrimViz._client.print_viz(s);
	}
};

//#ifndef __GNUC__
template <class T>
inline PrimViz<T> viz(const T& aClient) {
	return PrimViz<T>(aClient);
}

//#endif

//
//  	Invoke print_viz(s) for aPrimViz._client.
//
//  template <class T>
//  inline std::ostream& operator<<(std::ostream& s, const PrimViz<T>& aPrimViz)
//  	{ return aPrimViz._client.print_viz(s); }

//
//  	A PrimFullViz<T> is an ostream manipulator class used to enable T::print_full_viz to be invoked
//  	within a stream output sequence.
//  .note tutorial
//  	A PrimFullViz<T> may be used in the following way:
//
//  	s << "Unable to create " << full_viz(aName) << " for output.";
//
template <class T>

class PrimFullViz {

private:
	const T& _client;       //   Client to be print_viz'd.
	
private:
//
//  	Construct a print_viz'er for aClient.
//
	PrimFullViz(const T& aClient) : _client(aClient) {}
	
public:
	const T& client() const {
		return _client;
	}
	
//
//  	Construct a print_viz'er for aClient.
//
	friend inline PrimFullViz<T> full_viz(const T& aClient) {
		return PrimFullViz<T>(aClient);
	}
	
//
//  	Invoke print_viz(s) for aPrimFullViz._client.
//
	friend inline std::ostream& operator<<(std::ostream& s, const PrimFullViz<T>& aPrimFullViz) {
		return aPrimFullViz._client.print_full_viz(s);
	}
};

//
//  	Construct a print_viz'er for aClient.
//
//  template <class T>
//  inline PrimFullViz<T> full_viz(const T& aClient) { return PrimFullViz<T>(aClient); }

//
//  	Invoke print_viz(s) for aPrimFullViz._client.
//
//  template <class T>
//  inline std::ostream& operator<<(std::ostream& s, const PrimFullViz<T>& aPrimFullViz)
//  	{ return aPrimFullViz._client.print_full_viz(s); }

class PrimVizBufBase {

private:
	size_t _max_chars;       //   Maximum characters to output, 0 for no limit.
	char *_p;         //   Pointer to actuial buffer.
	std::ostrstream _s;       //   Steam for constrained text.
	const char *_dot_dot_dot;     //   Replacement tail characters if max_chars exceeded.
	char _buf[256];        //   Buffer for use for modest output.
	
protected:
	PrimVizBufBase(size_t maxCharacters, const char *dotDotDot);
	~PrimVizBufBase();
	std::ostream& s() {
		return _s;
	}
	
public:
	friend std::ostream& operator<<(std::ostream& s, PrimVizBufBase& aPrimVizBufBase);
};

//
//  	A PrimVizBuf<T> is an ostream manipulator class used to enable T::print_viz to be invoked
//  	within a stream output sequence.
//  .note tutorial
//  	A PrimVizBuf<T> may be used in the following way:
//
//  	s << "Unable to create " << viz(aName, 40, "+++") << " for output.";
//
//  	in which the text for aName is limited to 40 characters, characters 37 to 39 inclusive being replaced
//  	by "+++" if truncation occurs.
//
template <class T>

class PrimVizBuf : public PrimVizBufBase {

private:
//
//  	Construct a print_viz'er for aClient.
//
	PrimVizBuf(const T& aClient, size_t maxCharacters, const char *dotDotDot)
			: PrimVizBufBase(maxCharacters, dotDotDot) {
		aClient.print_viz(s());
	}
	
public:
//
//  	Construct a print_viz'er for aClient with output constrained to maxCharacters with dotDotDot run-out.
//
	friend inline PrimVizBuf<T> viz(const T& aClient, size_t maxCharacters, const char *dotDotDot = "...") {
		return PrimVizBuf<T>(aClient, maxCharacters, dotDotDot);
	}
};

//
//  	Construct a print_viz'er for aClient with output constrained to maxCharacters with dotDotDot run-out.
//
//  template <class T>
//  inline PrimVizBuf<T> viz(const T& aClient, size_t maxCharacters, const char *dotDotDot)
//  { return PrimVizBuf<T>(aClient, maxCharacters, dotDotDot); }

#endif
#endif

