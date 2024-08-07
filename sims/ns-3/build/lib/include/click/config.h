/* include/click/config.h.  Generated from config.h.in by configure.  */
/* Process this file with configure to produce config.h. -*- mode: c -*- */
#ifndef CLICK_CONFIG_H
#define CLICK_CONFIG_H

/* Define to 1 if type `char' is unsigned and you are not using gcc.  */
#ifndef __CHAR_UNSIGNED__
/* # undef __CHAR_UNSIGNED__ */
#endif

#define CLICK_BIG_ENDIAN	4321
#define CLICK_LITTLE_ENDIAN	1234
#define CLICK_NO_ENDIAN		0

/* Define to byte order of target machine. */
#define CLICK_BYTE_ORDER CLICK_LITTLE_ENDIAN

/* Define to enable debugging support for Click scheduling. */
/* #undef CLICK_DEBUG_SCHEDULING */

/* Define for Click memory allocation debugging. */
/* #undef CLICK_DMALLOC */

/* Define to generate smaller object files. */
/* #undef CLICK_OPTIMIZE_SIZE */

/* Version number of package */
#define CLICK_VERSION "2.1"

/* Version number of package, in CLICK_MAKE_VERSION_CODE format */
#define CLICK_VERSION_CODE CLICK_MAKE_VERSION_CODE(2,1,0)

/* Define to desired statistics level. */
#define CLICK_STATS 0

/* Define if PollDevice should run fast to get good benchmark numbers */
/* #undef CLICK_WARP9 */

/* Define if you have the __builtin_clz function. */
#define HAVE___BUILTIN_CLZ 1

/* Define if you have the __builtin_clzl function. */
#define HAVE___BUILTIN_CLZL 1

/* Define if you have the __builtin_clzll function. */
#define HAVE___BUILTIN_CLZLL 1

/* Define if you have the __builtin_ffs function. */
#define HAVE___BUILTIN_FFS 1

/* Define if you have the __builtin_ffsl function. */
#define HAVE___BUILTIN_FFSL 1

/* Define if you have the __builtin_ffsll function. */
#define HAVE___BUILTIN_FFSLL 1

/* Define if you have the __has_trivial_copy compiler intrinsic. */
#define HAVE___HAS_TRIVIAL_COPY 1

/* Define if you have the __sync_synchronize function. */
#define HAVE___SYNC_SYNCHRONIZE 1

/* Define if the __sync_synchronize function supports arguments. */
/* #undef HAVE___SYNC_SYNCHRONIZE_ARGUMENTS */

/* Define if the va_list type is addressable. */
/* #undef HAVE_ADDRESSABLE_VA_LIST */

/* Define if right shift of signed integers acts by sign extension. */
#define HAVE_ARITHMETIC_RIGHT_SHIFT 1

/* Define if Port::push/Port::pull should use bound function pointers. */
/* #undef HAVE_BOUND_PORT_TRANSFER */

/* Define if the C++ compiler understands constexpr. */
#define HAVE_CXX_CONSTEXPR 1

/* Define if the C++ compiler understands #pragma interface. */
#define HAVE_CXX_PRAGMA_INTERFACE 1

/* Define if the C++ compiler understands rvalue references. */
#define HAVE_CXX_RVALUE_REFERENCES 1

/* Define if the C++ compiler understands static_assert. */
#define HAVE_CXX_STATIC_ASSERT 1

/* Define if the C++ compiler understands template alias. */
#define HAVE_CXX_TEMPLATE_ALIAS 1

/* Define if the machine is indifferent to alignment. */
#define HAVE_INDIFFERENT_ALIGNMENT 1

/* Define if you want to use Intel-specific instructions. */
/* #undef HAVE_INTEL_CPU */

/* Define if 64-bit integer types are enabled. */
#define HAVE_INT64_TYPES 1

/* Define if IPv6 support is enabled. */
/* #undef HAVE_IP6 */

/* Define if IPsec support is enabled. */
/* #undef HAVE_IPSEC */

/* Define to 1 if the system has the type `long long'. */
#define HAVE_LONG_LONG 1

/* Define if nanosecond-granularity timestamps are enabled. */
#define HAVE_NANOTIMESTAMP_ENABLED 1

/* Define if you want to use the stride scheduler. */
#define HAVE_STRIDE_SCHED 1

/* Define to 1 since we have Strings. */
#define HAVE_STRING 1

/* Define to 1 if the system has the type `struct timespec'. */
#define HAVE_STRUCT_TIMESPEC 1

#ifdef HAVE_STRIDE_SCHED
/* Define if you want task scheduling to use a heap, not a linked list. */
/* #undef HAVE_TASK_HEAP */
#endif

/* The size of a `int', as computed by sizeof. */
#define SIZEOF_INT 4

/* The size of a `long', as computed by sizeof. */
#define SIZEOF_LONG 8

/* The size of a `long long', as computed by sizeof. */
#define SIZEOF_LONG_LONG 8

/* The size of a `size_t', as computed by sizeof. */
#define SIZEOF_SIZE_T 8

/* The size of a `struct timespec', as computed by sizeof. */
#define SIZEOF_STRUCT_TIMESPEC 16

/* The size of a `struct timeval', as computed by sizeof. */
#define SIZEOF_STRUCT_TIMEVAL 16

/* The size of a `ptrdiff_t', as computed by sizeof. */
#define SIZEOF_PTRDIFF_T 8

/* The size of a `void *', as computed by sizeof. */
#define SIZEOF_VOID_P 8

/* Define if you want to run multithreaded Click. */
/* #undef __MTCLICK__ */

/* Define inline, if necessary. C only. */
#ifndef __cplusplus
/* #undef inline */
#endif

/* Define constexpr to const under C or old C++. */
#if !defined(__cplusplus) || !HAVE_CXX_CONSTEXPR
# define constexpr const
#endif

/* Define CLICK_DEBUG_SCHEDULING to 0 if disabled. */
#ifndef CLICK_DEBUG_SCHEDULING
# define CLICK_DEBUG_SCHEDULING 0
#endif

/* Define macro for creating Click version codes (a la Linux version codes). */
#define CLICK_MAKE_VERSION_CODE(major, minor, patch) \
		(((major) << 16) | ((minor) << 8) | (patch))

/* Define macro for aligning variables. */
#if __GNUC__
# define CLICK_ALIGNED(x) __attribute__((aligned(x)))
#else
# define CLICK_ALIGNED(x) /* nothing */
#endif

/* Define macro for size of a cache line. */
#define CLICK_CACHE_LINE_SIZE 64

/* Define macro for the difference between 'x' and the next higher multiple
   of CLICK_CACHE_LINE_SIZE (between 0 and CLICK_CACHE_LINE_SIZE - 1). */
#define CLICK_CACHE_LINE_PAD_BYTES(x) \
	((((x) + CLICK_CACHE_LINE_SIZE - 1) / CLICK_CACHE_LINE_SIZE) * CLICK_CACHE_LINE_SIZE - (x))

/* Define macro for deprecated functions. */
#if __GNUC__ < 3 || (__GNUC__ == 3 && __GNUC_MINOR__ == 0)
# define CLICK_DEPRECATED /* nothing */
#else
# define CLICK_DEPRECATED __attribute__((deprecated))
#endif

/* Define macro for deprecated enumerations. */
#if __GNUC__ < 3 || (__GNUC__ == 3 && __GNUC_MINOR__ < 4)
# define CLICK_DEPRECATED_ENUM /* nothing */
#else
# define CLICK_DEPRECATED_ENUM __attribute__((deprecated))
#endif

/* Define macros for marking types as may-alias. */
#if __GNUC__ < 3 || (__GNUC__ == 3 && __GNUC_MINOR__ < 3)
# define CLICK_MAY_ALIAS /* nothing */
#else
# define CLICK_MAY_ALIAS __attribute__((__may_alias__))
#endif

/* Define macro for marking functions noninlinable. */
#ifdef CLICK_LINUXMODULE
# define CLICK_NOINLINE noinline
#elif __GNUC__
# define CLICK_NOINLINE __attribute__((noinline))
#else
# define CLICK_NOINLINE /* nothing */
#endif

/* Define macro for funtions that should be inline-able even if compiling without optimization. */
#if __GNUC__
# define CLICK_ALWAYS_INLINE __attribute__((always_inline))
#else
# define CLICK_ALWAYS_INLINE /* nothing */
#endif

/* Define macros for declaring packed structures. */
#ifdef __GNUC__
# define CLICK_PACKED_STRUCTURE(open, close) open close __attribute__((packed))
# define CLICK_SIZE_PACKED_STRUCTURE(open, close) open close __attribute__((packed)) /* deprecated */
# define CLICK_SIZE_PACKED_ATTRIBUTE __attribute__((packed))
#else
# define CLICK_PACKED_STRUCTURE(open, close) _Cannot_pack_structure__Use_GCC
# define CLICK_SIZE_PACKED_STRUCTURE(open, close) open close /* deprecated */
# define CLICK_SIZE_PACKED_ATTRIBUTE
#endif

/* Define macro for functions whose results should not be ignored. */
#if __GNUC__ < 3 || (__GNUC__ == 3 && __GNUC_MINOR__ < 4)
# define CLICK_WARN_UNUSED_RESULT /* nothing */
#else
# define CLICK_WARN_UNUSED_RESULT __attribute__((warn_unused_result))
#endif

/* Define macro for cold (rarely used) functions. */
#if __GNUC__ < 4 || (__GNUC__ == 4 && __GNUC_MINOR__ < 3)
# define CLICK_COLD /* nothing */
#else
# define CLICK_COLD __attribute__((cold))
#endif

/* Define ARCH_IS_BIG_ENDIAN based on CLICK_BYTE_ORDER. */
#if CLICK_BYTE_ORDER == CLICK_BIG_ENDIAN
# define ARCH_IS_BIG_ENDIAN	1
#elif CLICK_BYTE_ORDER == CLICK_LITTLE_ENDIAN
# define ARCH_IS_BIG_ENDIAN	0
#endif

/* Define macro for htons() on constants (allows htons() in switch cases). */
#if CLICK_BYTE_ORDER == CLICK_BIG_ENDIAN
# define click_constant_htons(x)	(x)
#elif CLICK_BYTE_ORDER == CLICK_LITTLE_ENDIAN
# define click_constant_htons(x)	((((x) >> 8) & 255) | (((x) & 255) << 8))
#endif

/* EXPORT_ELEMENT, ELEMENT_REQUIRES, ELEMENT_PROVIDES, ELEMENT_HEADER,
   ELEMENT_LIBS, and ELEMENT_MT_SAFE are noops. */
#define EXPORT_ELEMENT(x)
#define ELEMENT_REQUIRES(x)
#define ELEMENT_PROVIDES(x)
#define ELEMENT_HEADER(x)
#define ELEMENT_LIBS(x)
#define ELEMENT_MT_SAFE(x)

/* Assume CLICK_USERLEVEL unless otherwise defined. */
#if !defined(CLICK_USERLEVEL) && !defined(CLICK_TOOL) && !defined(CLICK_LINUXMODULE) && !defined(CLICK_BSDMODULE) && !defined(CLICK_MINIOS)
# define CLICK_USERLEVEL 1
#endif

/* Define stuff under a Linux module. */
#ifdef CLICK_LINUXMODULE
# include <click/config-linuxmodule.h>
#endif

/* Define stuff under a FreeBSD module. */
#ifdef CLICK_BSDMODULE
# include <click/config-bsdmodule.h>
#endif

/* Define stuff under nsclick. */
#ifdef CLICK_NS
# include <click/config-ns.h>
#endif

/* Define stuff under tools or a user-level driver. */
#if defined(CLICK_USERLEVEL) || defined(CLICK_TOOL)
# include <click/config-userlevel.h>
#endif

/* Define stuff under a MiniOS application. */
#ifdef CLICK_MINIOS
# include <click/config-minios.h>
#endif

/* Ensure declaration of DefaultArg template. */
#ifdef __cplusplus
CLICK_DECLS
template <typename T> struct DefaultArg;

/** @class uninitialized_type
    @brief Type tag indicating an object should not be initialized. */
struct uninitialized_type {
};
CLICK_ENDDECLS
#endif

/* Define aliasing versions of integer and pointer types. */
typedef uint16_t click_aliasable_uint16_t CLICK_MAY_ALIAS;
typedef int16_t click_aliasable_int16_t CLICK_MAY_ALIAS;
typedef uint32_t click_aliasable_uint32_t CLICK_MAY_ALIAS;
typedef int32_t click_aliasable_int32_t CLICK_MAY_ALIAS;
#if HAVE_INT64_TYPES
typedef uint64_t click_aliasable_uint64_t CLICK_MAY_ALIAS;
typedef int64_t click_aliasable_int64_t CLICK_MAY_ALIAS;
#endif
typedef void *click_aliasable_void_pointer_t CLICK_MAY_ALIAS;

#endif /* CLICK_CONFIG_H */
