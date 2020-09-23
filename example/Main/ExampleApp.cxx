/*!$@FOG@$!
 *	Generated at Wed Sep 23 05:53:40 2020
 *
 *	by fog 0.1.a of 04:38:49 Sep 23 2020
 *
 *	from
 *		../Library/SomeLibFile.fog
 *		../Main/ExampleApp.fog
 *		/home/sblo/FOG/example/Main/Main.mfog
 */

#ifndef EXAMPLEAPP_CXX
#define EXAMPLEAPP_CXX

#ifndef EXAMPLEAPP_HXX
#include <ExampleApp.hxx>
#endif

#line 14 "../Main/ExampleApp.fog"
int ExampleApp::Main()
{
    Console::Put("Hello world!\nAlso, write something: ");
#line 18
    char buf[1024];
    if (Console::Get(buf, 1024))
    {
#line 20
        Console::Put("You said: ");
        Console::Put(buf);
        Console::Put("\n");
    }
#line 25
    return 0;
};

#endif
