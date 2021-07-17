
#ifndef PRIMDIRECTORYITERATOR_HXX
#define PRIMDIRECTORYITERATOR_HXX

#ifdef HAS_DIRENT_H





//
//  	A PrimDirectoryIterator performs an iteration through a directory as.
//
//  	for (PrimDirectoryIterator p(aDirectoryName); p; p++)
//  		...
//
//  	while p is non-null p.path() and p.file() give the path and file name components, while
//  	p.error() gives the most recent error number.
//
class PrimDirectoryIterator : public PrimPrint {
	TYPEDECL_SINGLE(PrimDirectoryIterator, PrimPrint)
private:
	class Locked {};
	
private:
	const PrimStringHandle _path;  //   The directory name.
	DIR* _dir;       //   The directory stream.
	PrimError _error;     //   Most recent error.
	PrimStringHandle _file;    //   The current directory entry.
	
private:
	PrimDirectoryIterator(const PrimDirectoryIterator& aDirectory);    //   Not implemented.
	PrimDirectoryIterator& operator=(const PrimDirectoryIterator& aDirectory); //   Not implemented.
	
public:
	PrimDirectoryIterator(const char* aFileName);
	virtual ~PrimDirectoryIterator();
	//
	//  	Report whether the directory scan is in a fit state an incomplete.
	//
	operator const Locked* () const { return (const Locked*)_dir; }
	//
	//  	Report whether the directory scan is in an unfit state or complete.
	//
	bool operator!() const { return !operator const Locked * (); }
	void operator++();
	void operator++(int) { operator++(); }
	const PrimError& error() const { return _error; }
	const PrimString& file() const { return *_file; }
	const PrimString& path() const { return *_path; }
	virtual std::ostream& print_this(std::ostream& s) const;
};
#endif
#endif

