#pragma once

typedef char    BOOL;
typedef unsigned char UCHAR;
typedef char    CHAR;
typedef unsigned short USHORT;
typedef short   SHORT;
typedef unsigned long ULONG;
typedef long    LONG;

#ifndef TRUE
#define TRUE            1
#endif

#ifndef FALSE
#define FALSE           0
#endif

#define assert(p)                   while (!(p)){}
#define INLINE                      inline
#define PR_BEGIN_EXTERN_C           extern "C" {
#define	PR_END_EXTERN_C             }

void ENTER_CRITICAL_SECTION(void);
void EXIT_CRITICAL_SECTION(void);