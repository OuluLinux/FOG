#ifndef MONITORED_HXX
#define MONITORED_HXX

#ifndef MONITOR_HXX
#include <Monitor.hxx>
#endif

class Monitored
{
private:
#line 33 "tests/monitor/monitor.inc"
    DerivedMonitor _monitor;
#line 7 "tests/monitor/monitor.fog"
    int *_i;
    
public:
#line 9
    inline int *f();
    inline const int *fc() const;
    inline const volatile int *fcv() const volatile;
    inline volatile int *fv() volatile;
};

#line 9
inline int *Monitored::f()
{
#line 54 "tests/monitor/monitor.inc"
    ReadWriteLock aLock(_monitor);
#line 9 "tests/monitor/monitor.fog"
 return _i;
};

#line 10
inline const int *Monitored::fc() const
{
#line 50 "tests/monitor/monitor.inc"
    ReadOnlyLock aLock(_monitor);
#line 10 "tests/monitor/monitor.fog"
 return _i;
};

#line 11
inline const volatile int *Monitored::fcv() const volatile
{
#line 11
    return _i;
};

#line 12
inline volatile int *Monitored::fv() volatile
{
#line 12
    return _i;
};

#endif
