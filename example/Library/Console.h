#ifndef _Library_Console_h_
#define _Library_Console_h_

#include <stdio.h>
#include <string.h>


struct Console {
	
	static int Get(char* buf, int size);
	static void Put(const char* msg);
	
};

#endif
