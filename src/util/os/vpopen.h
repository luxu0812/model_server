// Copyright (C) 2021 zh.luxu1986@gmail.com

#ifndef MODEL_SERVER_SRC_UTIL_OS_VPOPEN_H_
#define MODEL_SERVER_SRC_UTIL_OS_VPOPEN_H_

#include <stdint.h>
#include <stdio.h>

FILE *vpopen(const char *cmd_string, const char *type);
int32_t vpclose(FILE *fp);

#endif  // MODEL_SERVER_SRC_UTIL_OS_VPOPEN_H_
