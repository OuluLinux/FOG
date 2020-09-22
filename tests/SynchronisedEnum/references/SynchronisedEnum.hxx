#ifndef SYNCHRONISEDENUM_HXX
#define SYNCHRONISEDENUM_HXX

class SynchronisedEnum
{
public:
    enum Enum
    {
        LABEL = 40,
        FIRST = 1,
        THIRD = 3
    };
    
public:
#line 4 "tests/SynchronisedEnum/SynchronisedEnum.fog"
    static const char *texts[];
};

#endif
