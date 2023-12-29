// Copyright (C) 2021 zh.luxu1986@gmail.com

#ifndef MODEL_SERVER_SRC_UTIL_OS_MACROS_H_
#define MODEL_SERVER_SRC_UTIL_OS_MACROS_H_

#ifdef __APPLE__
#define likely(x)       (x)
#define unlikely(x)     (x)
#elif defined(__linux__)
#ifdef __GNUC__
#define likely(x)       __builtin_expect(!!(x), 1)
#define unlikely(x)     __builtin_expect(!!(x), 0)
#else
#define likely(x)       (x)
#define unlikely(x)     (x)
#endif  // __GNUC__
#endif  // __APPLE__

#endif  // MODEL_SERVER_SRC_UTIL_OS_MACROS_H_
