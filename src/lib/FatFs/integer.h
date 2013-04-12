/*-------------------------------------------*/
/* Integer type definitions for FatFs module */
/*-------------------------------------------*/

#ifndef _INTEGER
#define _INTEGER

#ifdef _WIN32	/* FatFs development platform */

#include <windows.h>
#include <tchar.h>

#else			/* Embedded platform */

/* jg: there's a reason this was invented */
#include <stdint.h>

/* These types must be 16-bit, 32-bit or larger integer */
typedef int_least16_t	INT;
typedef uint_least16_t	UINT;

/* These types must be 8-bit integer */
typedef int8_t			CHAR;
typedef uint8_t			UCHAR;
typedef uint8_t			BYTE;

/* These types must be 16-bit integer */
typedef int16_t			SHORT;
typedef uint16_t		USHORT;
typedef uint16_t		WORD;
typedef uint16_t		WCHAR;

/* These types must be 32-bit integer */
typedef int32_t			LONG;
typedef uint32_t		ULONG;
typedef uint32_t		DWORD;

#endif

#endif
