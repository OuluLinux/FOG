
#ifndef PRIMTIME_HXX
#define PRIMTIME_HXX




//
//  	A PrimTime provides encpasulated access to the system time. On construction the current time is captured
//  	for subsequent interrogation.
//
class PrimTime {
private:
	const time_t _timer;      //   Time of construction.
	const struct tm _time;
	mutable PrimStringHandle _string;           //   asctime format
	mutable PrimStringHandle _date_string;      //   ANSI __DATE__
	mutable PrimStringHandle _time_string;      //   ANSI __TIME__
private:
	PrimTime& mutate() const { return *(PrimTime*)this; }
public:
	PrimTime();
	//  	virtual ~PrimTime();
	const PrimString& date() const;
	const struct tm& fields() const;
	const PrimString& string() const;
	const PrimString& time() const;
	time_t value() const;
	friend std::ostream& operator<<(std::ostream& s, const PrimTime& anObject);
};
#endif

