#ifndef _COMPILER_H_
#define _COMPILER_H_

#define likely(x)	                      __builtin_expect(!!(x), 1)
#define unlikely(x)	                    __builtin_expect(!!(x), 0)

#define barrier()                       __asm__ __volatile__("": : :"memory")
#define barrier_data(ptr)               __asm__ __volatile__("": :"r"(ptr) :"memory")

#define __user		__attribute__((noderef, address_space(1)))
#define __kernel	__attribute__((address_space(0)))
#define __safe		__attribute__((safe))
#define __force	  __attribute__((force))
#define __nocast	__attribute__((nocast))
#define __iomem	  __attribute__((noderef, address_space(2)))
#define __must_hold(x)	__attribute__((context(x,1,1)))
#define __acquires(x)	  __attribute__((context(x,0,1)))
#define __releases(x)	  __attribute__((context(x,1,0)))
#define __acquire(x)	  __context__(x,1)
#define __release(x)	  __context__(x,-1)
#define __cond_lock(x,c)	((c) ? ({ __acquire(x); 1; }) : 0)
#define __percpu	__attribute__((noderef, address_space(3)))
#define __rcu		  __attribute__((noderef, address_space(4)))
#define __private	__attribute__((noderef))

#define __alias(symbol)                 __attribute__((__alias__(#symbol)))
#define __aligned(x)                    __attribute__((__aligned__(x)))
#define __aligned_largest               __attribute__((__aligned__))

#ifndef __always_inline
#define __always_inline                 inline __attribute__((__always_inline__))
#endif

#define __gnu_inline                    __attribute__((__gnu_inline__))
#define   noinline                      __attribute__((__noinline__))

#define __cold                          __attribute__((__cold__))

#ifndef __attribute_const__
#define __attribute_const__             __attribute__((__const__))
#endif

#define __packed                        __attribute__((__packed__))
#define __pure                          __attribute__((__pure__))
#define __section(S)                    __attribute__((__section__(#S)))

#define __noreturn                      __attribute__((__noreturn__))
#define __always_unused                 __attribute__((__unused__))
#define __maybe_unused                  __attribute__((__unused__))
#define __used                          __attribute__((__used__))
#define __weak                          __attribute__((__weak__))

#ifndef __u8
#define __u8 unsigned char
#endif

#ifndef __u16
#define __u16 unsigned short int
#endif

#ifndef __u32
#define __u32 unsigned int
#endif

#ifndef __u64
#define __u64 unsigned long long
#endif

#ifndef	u8
#define u8 unsigned char
#endif

#ifndef u16
#define u16 unsigned short int
#endif

#ifndef u32
#define u32 unsigned int
#endif

#ifndef u64
#define u64 unsigned long long
#endif

#define __swab16(x) (__u16)__builtin_bswap16((__u16)(x))
#define __swab32(x) (__u32)__builtin_bswap32((__u32)(x))
#define __swab64(x) (__u64)__builtin_bswap64((__u64)(x))

static __always_inline void __read_once_size(const volatile void *p, void *res, int size)
{
	switch (size) {
	case 1: *(__u8 *)res = *(volatile __u8 *)p; break;
	case 2: *(__u16 *)res = *(volatile __u16 *)p; break;
	case 4: *(__u32 *)res = *(volatile __u32 *)p; break;
	case 8: *(__u64 *)res = *(volatile __u64 *)p; break;
	default:
		barrier();
		__builtin_memcpy((void *)res, (const void *)p, size);
		barrier();
	}
}

#define READ_ONCE(x)                              \
({									                              \
	union { typeof(x) __val; char __c[1]; } __u;		\
	__read_once_size(&(x), __u.__c, sizeof(x));		  \
	__u.__val;							                        \
})

static __always_inline void __write_once_size(volatile void *p, void *res, int size)
{
	switch (size) {
	case 1: *(volatile __u8 *)p = *(__u8 *)res; break;
	case 2: *(volatile __u16 *)p = *(__u16 *)res; break;
	case 4: *(volatile __u32 *)p = *(__u32 *)res; break;
	case 8: *(volatile __u64 *)p = *(__u64 *)res; break;
	default:
		barrier();
		__builtin_memcpy((void *)p, (const void *)res, size);
		barrier();
	}
}

#define WRITE_ONCE(x, val)                      \
({							                                \
	union { typeof(x) __val; char __c[1]; } __u =	\
		{ .__val = (__force typeof(x)) (val) };     \
	__write_once_size(&(x), __u.__c, sizeof(x));	\
	__u.__val;					                          \
})

#ifndef ARRAY_SIZE
#define ARRAY_SIZE(ptr) (sizeof(ptr) / sizeof((ptr)[0]))
#endif

#endif // _UTILS_H_
