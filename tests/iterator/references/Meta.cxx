#ifndef META_CXX
#define META_CXX

#ifndef META_HXX
#include <Meta.hxx>
#endif

#line 51 "tests/iterator/iterator.inc"
const MetaTypeInfo *Meta::_bases[] =
{
    0
};
#line 53
const MetaFunctionInfo Meta::_functions[] =
{
    {
        "dynamic_class_info"
    },
    {
        "static_class_info"
    },
    {
        0
    }
};
#line 61
const MetaVariableInfo Meta::_member_variables[] =
{
    {
        0
    }
};
#line 57
const MetaVariableInfo Meta::_static_variables[] =
{
    {
        "_type_info"
    },
    {
        "_bases"
    },
    {
        "_functions"
    },
    {
        "_static_variables"
    },
    {
        "_member_variables"
    },
    {
        0
    }
};
#line 40
const MetaTypeInfo Meta::_type_info =
{
    "Meta",
    Meta::_bases,
    Meta::_functions,
    Meta::_static_variables,
    Meta::_member_variables
};

#line 30
const MetaTypeInfo& Meta::dynamic_class_info() const
{
#line 30
    return _type_info;
};

#endif
