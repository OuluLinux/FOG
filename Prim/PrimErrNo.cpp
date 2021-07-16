#include <Prim/PrimIncludeAll.h>






//  
//  		Generate a message for aMessageCode. The implementation here outputs the message to stderr.
//  		Real applications are expected to reimplement this method to take appropriate action
//  		such as displaying the number on a LED display or entering the number into a log.
//  
void PrimErrNo::generate_message(int aMessageCode)
{
 int aSeverity = aMessageCode & 3;
 int aMessage = (aMessageCode  >> 2) & 0x3FF;
 int aLibrary = aMessageCode >> 12;
 switch (aSeverity)
 {
  case DIAGNOSTIC:
   fprintf(stderr, "Diagnostic message %d from library %d\n", aMessage, aLibrary);
   break;
  case WARNING:
   fprintf(stderr, "Warning message %d from library %d\n", aMessage, aLibrary);
#ifndef NO_PRIM_UTILITIES
   prim_increment_warning_count();
#endif
   break;
  case ERROR:
   fprintf(stderr, "Error message %d from library %d\n", aMessage, aLibrary);
#ifndef NO_PRIM_UTILITIES
   prim_increment_error_count();
#endif
   break;
  default:
   fprintf(stderr, "Uncategorised message %d\n", aMessageCode);
#ifndef NO_PRIM_UTILITIES
   prim_increment_error_count();
#endif
   break;
 }
}
