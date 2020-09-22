#ifndef MYOBSERVER_HXX
#define MYOBSERVER_HXX

#ifndef VECTOR_H
#include <vector.H>
#endif

class MyObserver;
class MySubject;

class MyMessage
{
public:
#line 10 "tests/Observer/Observer.fog"
    virtual void dispatch_to(MyObserver& anObserver, MySubject& aSubject) const;
};

class MyMessage1 : public MyMessage
{
private:
#line 70
    int _count;
    int _count1;
    
public:
#line 73
    inline MyMessage1(int aCount);
    inline int count() const;
#line 36
    virtual void dispatch_to(MyObserver& anObserver, MySubject& aSubject) const;
};

class MyObserver
{
public:
#line 30
    typedef void observe_MyMessage(MySubject& aSubject, const MyMessage& aMessage);
#line 37
    typedef void observe_MyMessage1(MySubject& aSubject, const MyMessage1& aMessage);
    
public:
#line 29
    virtual void observe(MySubject& aSubject, const MyMessage& aMessage);
#line 38
    virtual void observe(MySubject& aSubject, const MyMessage1& aMessage);
};

class MyObserver1 : public MyObserver
{
public:
#line 83
    virtual void observe(MySubject& aSubject, const MyMessage1& aMessage);
};

class MySubject
{
private:
#line 16
    vector < MyObserver * > _observers;
    
public:
#line 18
    inline void attach(MyObserver *anObserver);
    inline void detach(MyObserver *anObserver);
    void notify(const MyMessage& aMessage);
};

#line 73
inline MyMessage1::MyMessage1(int aCount)
:
    _count(aCount)
{};

#line 74
inline int MyMessage1::count() const
{
#line 74
    return _count;
};

#line 18
inline void MySubject::attach(MyObserver *anObserver)
{
#line 18
    _observers.push_back(anObserver);
};

#line 19
inline void MySubject::detach(MyObserver *anObserver)
{
#line 19
    _observers.remove(anObserver);
};

#endif
