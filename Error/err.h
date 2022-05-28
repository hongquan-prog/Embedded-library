#ifndef ERR_H
#define ERR_H

#include "err_def.h"

#define LOG(errno, info)                                              \
  {                                                                   \
    const char *str = error_to_str(errno);                            \
    printf("[%s:%d] Errno: %s: %s\n", __FILE__, __LINE__, str, info); \
  }

#define ENABLE_GLOBAL_DEBUG 0
#if (defined(ENABLE_GLOBAL_DEBUG) && (ENABLE_GLOBAL_DEBUG == 1))
#define NULL_ASSET(p)                     \
  {                                       \
    if (!(p))                             \
    {                                     \
      LOG(ERR_CONSTRUCT(NullPointer), e); \
      while (1)                           \
        ;                                 \
    }                                     \
  }
#else
#define NULL_ASSET(p) \
  {                   \
    while (!(p))      \
      ;               \
  }
#endif

typedef enum
{
  EXCEPTION_MODULE,
} module_enum_t;

// error code begin

typedef enum
{
  Arithmetic = ERR_CODE_BEGIN(EXCEPTION_MODULE),
  NullPointer,
  IndexOutofBounds,
  NoEnoughMemory,
  InvalidParameter,
  InvalidOperation
} exception_enum_t;

// error code end

const char *error_to_str(err_t errno);

#endif
