#ifndef __KEYWORDS_H__
#define __KEYWORDS_H__

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#include <stddef.h>
#include <stdint.h>

#define PUBLIC
#define PROTECTED
#define PRIVATE     static

#define STATIC
#define OVERRIDE

#define BASE_TO_SUB_CAST(instance, subType, baseName) \
    ((subType *)((uint8_t *)(instance) - offsetof(subType, baseName)))

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __KEYWORDS_H__

