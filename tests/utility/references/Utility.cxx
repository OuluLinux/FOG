#ifndef UTILITY_CXX
#define UTILITY_CXX

#ifndef PRIMTYPEINFO_HXX
#include <PrimTypeInfo.hxx>
#endif
#ifndef UTILITY_HXX
#include <Utility.hxx>
#endif

#line 14 "tests/utility/utility.inc"
const PrimTypeInfo& Utility::dynamic_type_info() const
{
#line 15
    return static_type_info();
};

#line 43
const PrimTypeInfo& Utility::static_type_info()
{
    static const PrimTypeInfo_Base baseList[2] =
    {
        {
            &PrimPrint::static_type_info(),
            (char * ) & (PrimPrint & )(Utility & ) * (char * )0x1000 - (char * )0x1000
        },
        {
            0,
            0
        }
    };
#line 50
    static const PrimTypeInfo typeInfo("Utility", baseList);
    return typeInfo;
};

#endif
