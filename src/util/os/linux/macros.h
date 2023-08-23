// Copyright 2021 zh.luxu1986@gmail.com

#ifndef UTIL_OS_LINUX_MACROS_H_
#define UTIL_OS_LINUX_MACROS_H_

#ifdef __GNUC__
#define likely(x)       __builtin_expect(!!(x), 1)
#define unlikely(x)     __builtin_expect(!!(x), 0)
#else
#define likely(x)       (x)
#define unlikely(x)     (x)
#endif

#endif  // UTIL_OS_LINUX_MACROS_H_
