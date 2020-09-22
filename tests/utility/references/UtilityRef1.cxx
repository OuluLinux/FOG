#ifndef UTILITYREF1_CXX
#define UTILITYREF1_CXX

#ifndef PRIMTYPEINFO_HXX
#include <PrimTypeInfo.hxx>
#endif
#ifndef UTILITYREF1_HXX
#include <UtilityRef1.hxx>
#endif

#line 14 "tests/utility/utility.inc"
const PrimTypeInfo& UtilityRef1::dynamic_type_info() const
{
#line 15
    return static_type_info();
};

#line 43
const PrimTypeInfo& UtilityRef1::static_type_info()
{
    static const PrimTypeInfo_Base baseList[2] =
    {
        {
            &PrimPrint::static_type_info(),
            (char * ) & (PrimPrint & )(UtilityRef1 & ) * (char * )0x1000 - (char * )0x1000
        },
        {
            0,
            0
        }
    };
#line 50
    static const PrimTypeInfo typeInfo("UtilityRef1", baseList);
    return typeInfo;
};

#endif
