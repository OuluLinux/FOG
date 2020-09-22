#ifndef VISITOR_HXX
#define VISITOR_HXX

class Visitor;

class Element
{
public:
#line 4 "tests/Visitor/Visitor.fog"
    typedef Element Element;
#line 3
    typedef Visitor Visitor;
    
public:
#line 5
    virtual bool accept(Visitor& aVisitor);
};

class DerivedElement1 : public Element
{
public:
#line 10
    virtual bool accept(Element::Visitor& aVisitor);
};

class DerivedElement2 : public Element
{
public:
#line 10
    virtual bool accept(Element::Visitor& aVisitor);
};

class DerivedDerivedElement : public DerivedElement2
{
public:
#line 10
    virtual bool accept(Element::Visitor& aVisitor);
};

class DerivedElement3 : public Element
{
public:
#line 10
    virtual bool accept(Element::Visitor& aVisitor);
};

class Visitor
{
public:
#line 12
    virtual bool accept_DerivedDerivedElement(DerivedDerivedElement& anElement);
#line 12
    virtual bool accept_DerivedElement1(DerivedElement1& anElement);
#line 12
    virtual bool accept_DerivedElement2(DerivedElement2& anElement);
#line 12
    virtual bool accept_DerivedElement3(DerivedElement3& anElement);
};

class DerivedVisitor : public Visitor
{
public:
#line 12
    virtual bool accept_DerivedDerivedElement(DerivedDerivedElement& anElement);
#line 12
    virtual bool accept_DerivedElement1(DerivedElement1& anElement);
#line 12
    virtual bool accept_DerivedElement2(DerivedElement2& anElement);
#line 12
    virtual bool accept_DerivedElement3(DerivedElement3& anElement);
};

#endif
