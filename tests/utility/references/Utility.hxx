#ifndef UTILITY_HXX
#define UTILITY_HXX

#ifndef PRIMPRINT_HXX
#include <PrimPrint.hxx>
#endif

class Utility : public PrimPrint
{
private:
#line 5 "tests/utility/utility.fog"
    int _x;

public:
#line 14 "tests/utility/utility.inc"
    virtual const PrimTypeInfo& dynamic_type_info() const;

public:
#line 43
    static const PrimTypeInfo& static_type_info();
};

#endif
