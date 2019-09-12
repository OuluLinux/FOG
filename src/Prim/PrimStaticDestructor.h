
#ifndef PRIMSTATICDESTRUCTOR_HXX
#define PRIMSTATICDESTRUCTOR_HXX


//  
//  	A PrimStaticDestructor maintains a list of static destruction routines to be invoked very late
//  	in the static destruction process, thereby insuring that they are invoked to assist in leak
//  	diagnosis, but not before potential clients have been eliminated. Very late is defined by
//  	the time of static destruction of the last static instance of a PrimStaticDestructorImp.
//  
//  	The registered destructor should be a static function with a signature of the form
//  
//  		bool registered_destructor(T *aParameter)
//  
//  	and be registered via a static instance of the form
//  
//  		static PrimStaticDestructor<T> aDestructor(&registered_destructor, aParameter);
//  
//  	The T *aParameter is propagated from the registration point to the destructor invocation.
//  
//  	The boolean return from the registered_destructor is used to signal whether destruction occurred (true)
//  	or must be deferred till another pass (false). The list of destructors is repeatedly traversed in
//  	reverse registration order until each routine has returned true, or a complete pass has occurred
//  	without any routines returning true. A registered_destructor is not reinvoked after it has returned
//  	true.
//  
class PrimStaticDestructorImp
{
public:
 typedef bool (*Destructor)(void *);
public:            //  .bugbug should be private if Sun C++ 4.1 would allow it.
 struct Entry
 {
  Destructor _destructor;
  void *_parameter;
 };
private:
 static Entry *_contents;       //   List of destructors.
 static size_t _tally;        //   Number of entries in _contents.
 static size_t _capacity;       //   Maximum number of entries in _contents.
 static size_t _count_out;       //   Activation count.
//  	static bool _destruct_on_exit;						// Set true to 
 
private:
 PrimStaticDestructorImp(const PrimStaticDestructorImp&);    //   Copy by creation is forbidden.
 PrimStaticDestructorImp& operator=(const PrimStaticDestructorImp&); //   Copy by assignment is forbidden.

public:
 PrimStaticDestructorImp(Destructor aDestructor, void *aParameter);
 ~PrimStaticDestructorImp();
};

template <class T>
class PrimStaticDestructor : private PrimStaticDestructorImp
{
 typedef bool (*Destructor)(T *);
public:
 PrimStaticDestructor(Destructor aDestructor, T *aParameter)
  : PrimStaticDestructorImp((PrimStaticDestructorImp::Destructor)aDestructor, aParameter) {} 
};
#endif
 
