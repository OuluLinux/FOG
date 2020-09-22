#ifndef ITERATOR_CXX
#define ITERATOR_CXX

#ifndef META_HXX
#include <Meta.hxx>
#endif
#ifndef ITERATOR_HXX
#include <Iterator.hxx>
#endif

#line 50 "tests/iterator/iterator.inc"
const MetaTypeInfo *B::_bases[] =
{
    &Iterator::static_class_info(),
    0
};
#line 53
const MetaFunctionInfo B::_functions[] =
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
const MetaVariableInfo B::_member_variables[] =
{
    {
        0
    }
};
#line 57
const MetaVariableInfo B::_static_variables[] =
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
const MetaTypeInfo B::_type_info =
{
    "B",
    B::_bases,
    B::_functions,
    B::_static_variables, 
    B::_member_variables
};

#line 30
const MetaTypeInfo& B::dynamic_class_info() const
{
#line 30
    return _type_info;
};

#line 51
const MetaTypeInfo *C::_bases[] =
{
    0
};
#line 53
const MetaFunctionInfo C::_functions[] =
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
const MetaVariableInfo C::_member_variables[] =
{
    {
        0
    }
};
#line 57
const MetaVariableInfo C::_static_variables[] =
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
const MetaTypeInfo C::_type_info =
{
    "C",
    C::_bases,
    C::_functions,
    C::_static_variables,
    C::_member_variables
};

#line 30
const MetaTypeInfo& C::dynamic_class_info() const
{
#line 30
    return _type_info;
};

#line 50
const MetaTypeInfo *D::_bases[] =
{
    &B::static_class_info(),
    &C::static_class_info(),
    0
};
#line 53
const MetaFunctionInfo D::_functions[] =
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
const MetaVariableInfo D::_member_variables[] =
{
    {
        0
    }
};
#line 57
const MetaVariableInfo D::_static_variables[] =
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
const MetaTypeInfo D::_type_info =
{
    "D",
    D::_bases,
    D::_functions,
    D::_static_variables,
    D::_member_variables
};

#line 30
const MetaTypeInfo& D::dynamic_class_info() const
{
#line 30
    return _type_info;
};

#line 51
const MetaTypeInfo *Iterator::_bases[] =
{
    0
};
#line 53
const MetaFunctionInfo Iterator::_functions[] =
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
#line 59
const MetaVariableInfo Iterator::_member_variables[] =
{
    {
        "_i"
    },
    {
        0
    }
};
#line 57
const MetaVariableInfo Iterator::_static_variables[] =
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
const MetaTypeInfo Iterator::_type_info =
{
    "Iterator",
    Iterator::_bases,
    Iterator::_functions,
    Iterator::_static_variables,
    Iterator::_member_variables
};

#line 30
const MetaTypeInfo& Iterator::dynamic_class_info() const
{
#line 30
    return _type_info;
};

#endif
