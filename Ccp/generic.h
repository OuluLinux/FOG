/* 
	Copyright 04 Aug 1993 Sun Microsystems, Inc. All Rights Reserved
*/
/*   generic.h -- for faking generic class declarations

    Copyright 1991 by TauMetric Corporation      ALL RIGHTS RESERVED

    @(#)generic.h	1.4  04 Aug 1993 14:08:25
*/

#ifndef _GENERIC_H_
#define _GENERIC_H_

//  token-pasting macros; ANSI requires an extra level of indirection
#define name2(z, y)  name2_x(z, y)
#define name2_x(z, y)  z##y
#define name3(z, y, x)  name3_x(z, y, x)
#define name3_x(z, y, x) z##y##x
#define name4(z, y, x, w) name4_x(z, y, x, w)
#define name4_x(z, y, x, w) z##y##x##w

//  macros for declaring and implementing classes
#define declare(z, y)  name2(z, declare)(y)
#define implement(z, y)  name2(z, implement)(y)
#define declare2(z, y, x) name2(z, declare2)(y, x)
#define implement2(z, y, x) name2(z, implement2)(y, x)

//  macros for declaring error-handling functions
extern void genericerror(int, char*);
typedef int (*GPT)(int, char*);
#define set_handler(gen, tp, z) name4(set_, tp, gen, _handler)(z)
#define errorhandler(gen, tp) name3(tp, gen, handler)
#define callerror(gen, tp, z, y) (*errorhandler(gen, tp))(z, y)

#endif /*  _GENERIC_H_ */
