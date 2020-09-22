#ifndef MYOBSERVER_CXX
#define MYOBSERVER_CXX

#ifndef MYOBSERVER_HXX
#include <MyObserver.hxx>
#endif

#line 10 "tests/Observer/Observer.fog"
void MyMessage::dispatch_to(MyObserver& anObserver, MySubject& aSubject) const
{
#line 11
    anObserver.observe(aSubject, *this);
};

#line 36
void MyMessage1::dispatch_to(MyObserver& anObserver, MySubject& aSubject) const
{
#line 36
    anObserver.observe(aSubject, *this);
};

#line 29
void MyObserver::observe(MySubject& aSubject, const MyMessage& aMessage) {};

#line 38
void MyObserver::observe(MySubject& aSubject, const MyMessage1& aMessage) {};

#line 83
void MyObserver1::observe(MySubject& aSubject, const MyMessage1& aMessage)
{
    cout << aSubject << " changed count to " << aMessage.count() << endl;
};

#line 20
void MySubject::notify(const MyMessage& aMessage)
{
    for (MyObserver * p = _observers.begin(); p != _observers.end(); ++ p)
        aMessage.dispatch_to(*p, *this);
};

#endif
