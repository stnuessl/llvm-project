#ifndef PLATFORM_TYPES_H
#define PLATFORM_TYPES_H

#define CPU_TYPE_8 8
#define CPU_TYPE_16 16
#define CPU_TYPE_32 32
#define CPU_TYPE_64 64
#define MSB_FIRST 0
#define LSB_FIRST 1
#define HIGH_BYTE_FIRST 0
#define LOW_BYTE_FIRST 1

#if __x86_64__ || __aarch64__ || __powerpc64__
#define CPU_TYPE CPU_TYPE_64
#elif __i386__ || __arm__ || __powerpc__
#define CPU_TYPE CPU_TYPE_32
#else
#error failed to determine architecture
#endif 

#define CPU_BIT_ORDER LSB_FIRST

#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
#define CPU_BYTE_ORDER LOW_BYTE_FIRST
#elif __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
#define CPU_BYTE_ORDER HIGH_BYTE_FIRST
#else
#error failed to determine byte order
#endif

#define FALSE 0u
#define TRUE 1u

typedef __UINT8_TYPE__ boolean;
typedef __INT8_TYPE__ sint8;
typedef __UINT8_TYPE__ uint8;
typedef __INT16_TYPE__ sint16;
typedef __UINT16_TYPE__ uint16;
typedef __INT32_TYPE__ sint32;
typedef __UINT32_TYPE__ uint32;
typedef __INT64_TYPE__ sint64;
typedef __UINT64_TYPE__ uint64;

typedef float float32;
typedef double float64;

#endif /* PLATFORM_TYPES_H */
