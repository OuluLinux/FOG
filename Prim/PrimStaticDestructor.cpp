#include <Prim/PrimIncludeAll.h>







PrimStaticDestructorImp::Entry *PrimStaticDestructorImp::_contents = 0;    //   List of destructors.
size_t PrimStaticDestructorImp::_tally = 0;    //   Number of entries in _contents.
size_t PrimStaticDestructorImp::_capacity = 0;    //   Maximum number of entries in _contents.
size_t PrimStaticDestructorImp::_count_out = 0;   //   Count out till destructors called.

//  
//  	Register aDestructor for invocation once all PrimStaticDestructorImp's have destructed.
//  
PrimStaticDestructorImp::PrimStaticDestructorImp(Destructor aDestructor, void *aParameter)
{
 if (_count_out)
  ERRMSG("Constructing a PrimStaticDestructorImp after destructing another.");
 if (_capacity <= _tally)
 {
  const size_t newCapacity = std::max((size_t)64, 2 * _capacity);
  Entry *const newContents = new Entry[newCapacity];
  if (!newContents)
  {
   ERRMSG("Failed to allocate memory for PrimStaticDestructorImp array.");
   return;
  }
  const Entry *p = _contents;
  Entry *q = newContents;
  for (size_t i = _tally; i-- > 0; )
   *q++ = *p++;
  Entry *const oldContents = _contents;
  _contents = newContents;
  _capacity = newCapacity;
  if (oldContents)
   delete[] oldContents;
 }
 Entry *p = &_contents[_tally++];
 p->_destructor = aDestructor;
 p->_parameter = aParameter;
}

//  
//  	Destruction of a PrimStaticDestructorImp count out until all are destructed and then repeatedly invokes
//  	all registered destructors in reverse registration order until each has returned true once or until
//  	no registered destructors return true. A registered destructor is not invoked after it has rturned true.
//  
PrimStaticDestructorImp::~PrimStaticDestructorImp()
{
 if (++_count_out >= _tally)
 {
  bool gotOne = false;
  do
  {
   gotOne = false;
   Entry *p = _contents + _tally - 1;
   for (int i = _tally; i-- > 0; p--)
    if (!p->_destructor)
     ;
    else if ((*p->_destructor)(p->_parameter))
    {
     p->_destructor = 0;
     gotOne = true;
    }
  } while (gotOne);
  if (_contents)
  {
   delete[] _contents;
   _contents = 0;
  }
 }
}
