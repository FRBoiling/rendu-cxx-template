/*
* Created by boil on 2022/9/25.
*/

#ifndef RENDU_RANDOM_DEFINE_H_
#define RENDU_RANDOM_DEFINE_H_

#if defined(__aarch64__)
#if defined(__clang__)

#include <mm_malloc.h>

#elif defined(__GNUC__)
static __inline__ void *__attribute__((__always_inline__, __nodebug__, __malloc__))
_mm_malloc(size_t __size, size_t __align)
{
    if (__align == 1)
    {
        return malloc(__size);
    }

    if (!(__align & (__align - 1)) && __align < sizeof(void *))
        __align = sizeof(void *);

    void *__mallocedMemory;

    if (posix_memalign(&__mallocedMemory, __align, __size))
        return NULL;

    return __mallocedMemory;
}

static __inline__ void __attribute__((__always_inline__, __nodebug__))
_mm_free(void *__p)
{
    free(__p);
}
#else
#error aarch64 only on clang and gcc
#endif
#else
#include <emmintrin.h>
#endif

#endif //RENDU_RANDOM_DEFINE_H_
