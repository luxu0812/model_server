// Copyright 2021 zh.luxu1986@gmail.com

#ifndef UTIL_IO_H_
#define UTIL_IO_H_

#include <string>
#include <string_view>

namespace WORKSPACE::util::io {

bool read_line(FILE *fp, std::string *line);
bool read_file(FILE *fp, std::string *file);

bool read_line(int32_t fd, std::string *line);
bool read_file(int32_t fd, std::string *file);

bool reverse_file_line(std::string_view input, std::string_view output);

bool curl(const std::string& url, std::string *body);

}  // namespace WORKSPACE::util::io

#endif  // UTIL_IO_H_
