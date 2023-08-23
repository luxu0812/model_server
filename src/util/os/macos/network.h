// Copyright 2021 zh.luxu1986@gmail.com

#ifndef MODEL_SERVER_SRC_UTIL_OS_MACOS_NETWORK_H_
#define MODEL_SERVER_SRC_UTIL_OS_MACOS_NETWORK_H_

#include <unistd.h>      // gethostname
#include <netdb.h>       // gethostbyname
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <string>

bool get_host_ip(std::string *ip) {
  if (nullptr == ip) {
    return false;
  }

  char hostname[256];
  if (0 != gethostname(hostname, sizeof(hostname))) {
    return false;
  }

  struct hostent *host_entry = gethostbyname(hostname);
  if (nullptr == host_entry) {
    return false;
  }

  *ip = inet_ntoa(*((struct in_addr*) host_entry->h_addr_list[0]));

  return true;
}

#endif  // MODEL_SERVER_SRC_UTIL_OS_MACOS_NETWORK_H_
