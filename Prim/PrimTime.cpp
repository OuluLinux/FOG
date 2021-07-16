#include <Prim/PrimIncludeAll.h>



//  %import PrimManip.hxx

#ifdef HAS_CTIME_R
extern "C"
{
 extern char *ctime_r(const time_t *, char *, int);  //   Would be in time.h if POSIX or REENTRANT
};
#endif

PrimTime::PrimTime()
:
 _timer(::time(0)),
    _time(*localtime(&_timer))
{}

const PrimString& PrimTime::date() const
{
    if (!_date_string)
    {
        char dateBuf[20] = { 0 };
        const char *ascTime = string().str();
        strncpy(dateBuf, ascTime + 4, 7);
        strncpy(dateBuf + 7, ascTime + 20, 4);
        mutate()._date_string = PrimStringHandle(dateBuf, 11);
    }
    return *_date_string;
}

const PrimString& PrimTime::string() const
{
    if (!_string)
    {
#ifdef HAS_CTIME_R
     char buf[80];
     mutate()._string = ctime_r(&_timer, buf, sizeof(buf));
#else
     mutate()._string = ctime(&_timer);
#endif
    }
    return *_string;
}

const PrimString& PrimTime::time() const
{
    if (!_time_string)
        mutate()._time_string = PrimStringHandle(string().str() + 11, 8);
    return *_time_string;
}

std::ostream& operator<<(std::ostream& s, const PrimTime& aTime)
{
 s << aTime.string();
 return s;
}
