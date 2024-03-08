// Copyright (C) 2021 zh.luxu1986@gmail.com

#ifndef MODEL_SERVER_SRC_UTIL_OS_RESOURCE_USED_H_
#define MODEL_SERVER_SRC_UTIL_OS_RESOURCE_USED_H_

struct ResourceUsed {
  double resident_mb;
  double user_time;
  double system_time;
};

bool get_process_resource_used(struct ResourceUsed *res);
bool get_system_resource_used(struct ResourceUsed *res);

#endif  // MODEL_SERVER_SRC_UTIL_OS_RESOURCE_USED_H_
