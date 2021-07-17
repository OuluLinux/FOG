#ifndef STRINGS_CXX
#define STRINGS_CXX

#ifndef STRINGS_HXX
#include <Strings.hxx>
#endif

#line 3 "tests/strings/strings.fog"
const char *Strings::p = "";
const char *Strings::p0 = "p0";
const char *Strings::p1 = "p1";
#line 7
const char *Strings::p2 = "p2";
#line 9
const char *Strings::p3 = "p\\'\"@=$";
const char *Strings::p4 = "...";
#line 12
const char *Strings::p5 = "...\t\t";
#line 16
const char *Strings::p6 = " abc  def  ghi ";
#line 19
const char *Strings::p7 = "a|\n0|\nh|\0000|$8|-7|\2667|Uh|Uh|\336012|\27433";

#endif
