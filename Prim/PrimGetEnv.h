
#ifndef PRIMGETENV_H
#define PRIMGETENV_H


//  
//  	A PrimGetEnv<T> defines a variable of type T, with a constructor that organises
//  	initialisation by issuing a getenv(). Such variables are intended to support
//  	statically initialised objects used to parameterise program behaviour. The objects
//  	should normally be controlled by command line parameters, however in order
//  	to permit control during static initialisation a more direct metrhod of control is
//  	required - using environment variables.
//  
template <class T>
class PrimGetEnv
{
private:
 T _value;     //   The value.
private:
//  
//  	Construction of a copy is not implemented.
//  
 PrimGetEnv(const PrimGetEnv<T>& aGetEnv);
//  
//  	Assignement is not implemented.
//  
 PrimGetEnv& operator=(const PrimGetEnv<T>& aGetEnv);
public:
 PrimGetEnv(const char *envName, const T& defaultValue);
//  
//  	Return a reference to the value.
//  
 operator T& () { return _value; }
 operator const T& () const { return _value; }
//  
//  	Return the value.
//  
 operator T() const { return _value; }
};
#endif
 
