/*
 * types.h
 *
 *  Created on: Nov 5, 2015
 *      Author: enrico
 */

#ifndef SRC_TYPES_H_
#define SRC_TYPES_H_

#include <stdint.h>

#define WINAPI

typedef uint8_t   		BYTE;
typedef uint8_t   		UCHAR;
typedef uint8_t   		TCHAR;
typedef int16_t  		SHORT;
typedef uint16_t  		USHORT;
typedef uint32_t  		DWORD;
typedef uint32_t  		ULONG;
typedef uint16_t  		WORD;
typedef int16_t *  		LPWORD;
typedef uint8_t *  		LPBYTE;
typedef uint32_t *		LPDWORD;
typedef bool			BOOL;
typedef int32_t			__int32;
typedef int32_t			LONG;			// may be int64_t
typedef void * 			PVOID;
typedef char * 			LPCTSTR;
typedef void * 			HANDLE;
typedef void * 			HWND;
typedef int64_t			__int64;
typedef void * 			HHOOK;

union			LARGE_INTEGER {
	struct {
		DWORD LowPart;
		LONG HighPart;
	} u;
	int64_t QuadPart;
};

#endif /* SRC_TYPES_H_ */
