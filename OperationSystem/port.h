#pragma once

#ifdef __cplusplus
extern "C"
{
#endif

#include <stddef.h>
#include <stdint.h>

#define pdCHAR char
#define pdFLOAT float
#define pdDOUBLE double
#define pdLONG long
#define pdSHORT short
#define pdSTACK_TYPE uint32_t
#define pdBASE_TYPE long
#define pdFALSE ((BaseType_t)0)
#define pdTRUE ((BaseType_t)1)

typedef uint32_t StackType_t;
typedef long BaseType_t;
typedef unsigned long UBaseType_t;

#ifdef __cplusplus
}
#endif