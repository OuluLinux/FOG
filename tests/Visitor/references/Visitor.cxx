#ifndef VISITOR_CXX
#define VISITOR_CXX

#ifndef VISITOR_HXX
#include <Visitor.hxx>
#endif

#line 10 "tests/Visitor/Visitor.fog"
bool DerivedDerivedElement::accept(Element::Visitor& aVisitor)
{
#line 11
    return aVisitor.accept_DerivedDerivedElement(*this) || DerivedElement2::accept_DerivedElement2(aVisitor);
};

#line 10
bool DerivedElement1::accept(Element::Visitor& aVisitor)
{
#line 11
    return aVisitor.accept_DerivedElement1(*this) || Element::accept_Element(aVisitor);
};

#line 10
bool DerivedElement2::accept(Element::Visitor& aVisitor)
{
#line 11
    return aVisitor.accept_DerivedElement2(*this) || Element::accept_Element(aVisitor);
};

#line 10
bool DerivedElement3::accept(Element::Visitor& aVisitor)
{
#line 11
    return aVisitor.accept_DerivedElement3(*this) || Element::accept_Element(aVisitor);
};

#line 12
bool DerivedVisitor::accept_DerivedDerivedElement(DerivedDerivedElement& anElement)
{
    return true;
};

#line 12
bool DerivedVisitor::accept_DerivedElement1(DerivedElement1& anElement)
{
#line 70
    cout << "DerivedVisitor accepts DerivedElement1" << endl;
#line 14
    return true;
};

#line 12
bool DerivedVisitor::accept_DerivedElement2(DerivedElement2& anElement)
{
#line 72
    cout << "DerivedVisitor accepts DerivedElement2" << endl;
#line 14
    return true;
};

#line 12
bool DerivedVisitor::accept_DerivedElement3(DerivedElement3& anElement)
{
    return true;
};

#line 5
bool Element::accept(Visitor& aVisitor)
{
#line 5
    return false;
};

#line 12
bool Visitor::accept_DerivedDerivedElement(DerivedDerivedElement& anElement)
{
#line 13
    return false;
};

#line 12
bool Visitor::accept_DerivedElement1(DerivedElement1& anElement)
{
#line 13
    return false;
};

#line 12
bool Visitor::accept_DerivedElement2(DerivedElement2& anElement)
{
#line 13
    return false;
};

#line 12
bool Visitor::accept_DerivedElement3(DerivedElement3& anElement)
{
#line 13
    return false;
};

#endif
