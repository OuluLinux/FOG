#include <Prim/PrimIncludeAll.h>





//  
//  		Compare two strings as for strcmp() but ignoring case inequalities.
//  
int strcasecmp(const char *p, const char *q) 
#if defined flagCLANG && !defined flagFREEBSD && !defined flagLINUX
throw()
#endif
{
 int cp, cq;
 while (cp = *p++, cq = *q++, (cp != EOS) && (cq != EOS))
 {
  if (cp == cq)
   continue;
  if (islower(cp) && isupper(cq))
   cq = tolower(cq);
  if (islower(cq) && isupper(cp))
   cp = tolower(cp);
  if (cp != cq)
   break;
 }
 return cp - cq;
}
