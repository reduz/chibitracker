//
// C++ Interface: movemem
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//

#ifndef MOVEMEM_H
#define MOVEMEM_H

#include "config.h"

#ifdef ANSIC_LIBS_ENABLED

#include <string.h> //includes memmove

#else
//must be redefined
 void *memmove(void *dest, const void *src, Uint32 n);


#endif


#endif
