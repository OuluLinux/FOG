#ifndef META_HXX
#define META_HXX

struct MetaFunctionInfo;
struct MetaTypeInfo;
struct MetaVariableInfo;

class Meta
{
public:
#line 30 "tests/iterator/iterator.inc"
    virtual const MetaTypeInfo& dynamic_class_info() const;
    
private:
#line 51
    static const MetaTypeInfo *_bases[];
#line 53
    static const MetaFunctionInfo _functions[];
#line 61
    static const MetaVariableInfo _member_variables[];
#line 57
    static const MetaVariableInfo _static_variables[];
#line 40
    static const MetaTypeInfo _type_info;
    
public:
#line 29
    inline static const MetaTypeInfo& static_class_info();
};

struct MetaFunctionInfo
{
public:
#line 8
    const char *_name;
};

struct MetaTypeInfo
{
public:
#line 20
    const char *_name;
    const MetaTypeInfo *const *_bases;
    const MetaFunctionInfo *_functions;
    const MetaVariableInfo *_static_variables;
    const MetaVariableInfo *_member_variables;
};

struct MetaVariableInfo
{
public:
#line 14
    const char *_name;
};

#line 29
inline const MetaTypeInfo& Meta::static_class_info()
{
#line 29
    return _type_info;
};

#endif
