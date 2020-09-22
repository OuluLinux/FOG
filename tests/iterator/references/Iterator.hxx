#ifndef ITERATOR_HXX
#define ITERATOR_HXX

struct MetaFunctionInfo;
struct MetaTypeInfo;
struct MetaVariableInfo;

class C
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

class Iterator
{
private:
#line 3 "tests/iterator/iterator.fog"
    int _i;
    
public:
#line 30 "tests/iterator/iterator.inc"
    virtual const MetaTypeInfo& dynamic_class_info() const;
    
private:
#line 51
    static const MetaTypeInfo *_bases[];
#line 53
    static const MetaFunctionInfo _functions[];
#line 59
    static const MetaVariableInfo _member_variables[];
#line 57
    static const MetaVariableInfo _static_variables[];
#line 40
    static const MetaTypeInfo _type_info;
    
public:
#line 29
    inline static const MetaTypeInfo& static_class_info();
};

class B : public Iterator
{
public:
#line 30
    virtual const MetaTypeInfo& dynamic_class_info() const;
    
private:
#line 50
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

class D : public B, public C
{
public:
#line 30
    virtual const MetaTypeInfo& dynamic_class_info() const;
    
private:
#line 50
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

#line 29
inline const MetaTypeInfo& B::static_class_info()
{
#line 29
    return _type_info;
};

#line 29
inline const MetaTypeInfo& C::static_class_info()
{
#line 29
    return _type_info;
};

#line 29
inline const MetaTypeInfo& D::static_class_info()
{
#line 29
    return _type_info;
};

#line 29
inline const MetaTypeInfo& Iterator::static_class_info()
{
#line 29
    return _type_info;
};

#endif
