#ifndef MONITOR_HXX
#define MONITOR_HXX

class Monitor
{
    friend class ReadOnlyLock;
    friend class ReadWriteLock;
    
private:
#line 24 "tests/monitor/monitor.inc"
    inline void enter_exclusive();
    inline void enter_shared();
    inline void leave();
    
public:
    class ReadOnlyLock
    {
    private:
#line 8
        Monitor& _monitor;
        
    public:
#line 10
        inline ReadOnlyLock(Monitor& aMonitor);
        inline ~ReadOnlyLock();
    };
    
public:
    class ReadWriteLock
    {
    private:
#line 17
        Monitor& _monitor;
        
    public:
#line 19
        inline ReadWriteLock(Monitor& aMonitor);
        inline ~ReadWriteLock();
    };
};

class DerivedMonitor : public Monitor
{
};

#line 24
inline void Monitor::enter_exclusive() {};

#line 25
inline void Monitor::enter_shared() {};

#line 26
inline void Monitor::leave() {};

#line 10
inline Monitor::ReadOnlyLock::ReadOnlyLock(Monitor& aMonitor)
:
    _monitor(aMonitor)
{
#line 10
    _monitor.enter_shared();
};

#line 11
inline Monitor::ReadOnlyLock::~ReadOnlyLock()
{
#line 11
    _monitor.leave();
};

#line 19
inline Monitor::ReadWriteLock::ReadWriteLock(Monitor& aMonitor)
:
    _monitor(aMonitor)
{
#line 19
    _monitor.enter_exclusive();
};

#line 20
inline Monitor::ReadWriteLock::~ReadWriteLock()
{
#line 20
    _monitor.leave();
};

#endif
