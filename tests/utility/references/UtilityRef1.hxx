#ifndef UTILITYREF1_HXX
#define UTILITYREF1_HXX

#ifndef PRIMPRINT_HXX
#include <PrimPrint.hxx>
#endif

class UtilityRef1 : public PrimPrint
{
private:
#line 12 "tests/utility/utility.fog"
    int _extend;

public:
#line 14 "tests/utility/utility.inc"
    virtual const PrimTypeInfo& dynamic_type_info() const;

public:
#line 43
    static const PrimTypeInfo& static_type_info();
};

#endif
