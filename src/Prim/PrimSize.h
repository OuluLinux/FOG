
#ifndef PRIMSIZE_HXX
#define PRIMSIZE_HXX


//  
//  	A PrimSize defines an integer for use typically as an array dimension. This class is intended for
//  	use to prevent accidental construction of classes which would otherwise have single integer arguments.
//  
class PrimSize
{
private:
 size_t _size;        //   The size value.

public:
//  
//  	Conastruct a size value from aSize.
//  
 PrimSize(size_t aSize) : _size(aSize) {}
//  
//  	Construct a copy of aSize.
//  
 PrimSize(const PrimSize& aSize) : _size(aSize._size) {}
//  
//  	Assign aSize to this size value
//  
 PrimSize& operator=(size_t aSize) { _size = aSize; return (*this); }
//  
//  	Assign aSize to this size value
//  
 PrimSize& operator=(const PrimSize& aSize) { _size = aSize._size; return (*this); }
//  
//  	Make the size available as a built-in type.
//  
 size_t size() const { return (_size); }
 static const PrimSize& zero();
};
#endif

