//
// C++ Interface: config
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2004
//
// Copyright: See COPYING file that comes with this distribution
//
//

#ifndef CONFIG_H
#define CONFIG_H

#include <stddef.h>


#define VERSION_MAJOR 1
#define VERSION_MINOR 2
#define VERSION_REV ""


#define ZERO_STRUCT( m_struct ) { for (long __i=0;__i<sizeof(m_struct);__i++) ((char*)(&m_struct))[__i]=0; }

#ifdef PALM_OS_ENABLED

#define MAX_VIRTUAL_VOICES 64
#define MIXDOWN_BUFFER_SIZE 4096
#define CPU_16_BITS


#else

#define MAX_VIRTUAL_VOICES 128
#define CPU_32_BITS
#define MIXDOWN_BUFFER_SIZE 2048
	
#endif

#define MAX_CHORUS_READBACK_MS 50

#define CHORUS_RINGBUFF_SIZE 8192
#define CHORUS_RINGBUFF_MASK (8192-1)

#define DECLICKER_BUFFER_BITS 6
#define DECLICKER_BUFFER_SIZE (1<<DECLICKER_BUFFER_BITS)
#define DECLICKER_BUFFER_MASK (DECLICKER_BUFFER_SIZE-1)
#define DECLICKER_FADE_BITS 5
#define DECLICKER_FADE_SIZE (1<<DECLICKER_FADE_BITS)





#ifdef CPU_16_BITS

typedef signed long long Sint64;
typedef unsigned long long Uint64;

typedef signed long Sint32;
typedef unsigned long Uint32;

typedef signed int Sint16;
typedef unsigned int Uint16;

#else

typedef signed int Sint32;
typedef unsigned int Uint32;

typedef signed short Sint16;
typedef unsigned short Uint16;

#if defined ( __BORLANDC__ ) || defined ( _MSC_VER )

typedef __int64 Sint64;
typedef __int64 Uint64;

#else
typedef signed long long Sint64;
typedef unsigned long long Uint64;

#endif

#endif

typedef signed char Sint8;
typedef unsigned char Uint8;

#ifndef MIN
#define MIN(m_left,m_right) ( (m_left)<(m_right)?(m_left):(m_right) )
#endif

#ifndef MAX
#define MAX(m_left,m_right) ( (m_left)>(m_right)?(m_left):(m_right) )
#endif


#define RANDOM_MAX 2147483647



static inline Sint32 random_generate(Uint32 *seed) {
	Sint32 k;
	Sint32 s = (Sint32)(*seed);
	if (s == 0)
		s = 0x12345987;
	k = s / 127773;
	s = 16807 * (s - k * 127773) - 2836 * k;
	if (s < 0)
		s += 2147483647;
	(*seed) = (Uint32)s;
	return (Sint32)(s & RANDOM_MAX);
}

static inline Sint32 intabs(const Sint32& p_v) {
	
	if (p_v<0)
		return -p_v;
	else
		return p_v;	
}

static inline void memzero(void *ptr,Uint32 p_len) {
	
	char*c=(char*)ptr;
	for (int i=0;i<(int)p_len;i++)
		c[i]=0;
	
}

#endif  /* Config_h */
