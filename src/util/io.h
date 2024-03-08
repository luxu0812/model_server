// Copyright (C) 2021 zh.luxu1986@gmail.com

#ifndef MODEL_SERVER_SRC_UTIL_IO_H_
#define MODEL_SERVER_SRC_UTIL_IO_H_

#include <string>
#include <string_view>

namespace model_server {

bool read_line(FILE *fp, std::string *line);
bool read_file(FILE *fp, std::string *file);

bool read_line(int32_t fd, std::string *line);
bool read_file(int32_t fd, std::string *file);

bool reverse_file_line(std::string_view input, std::string_view output);

bool curl(const std::string& url, std::string *body);

}  // namespace model_server

#endif  // MODEL_SERVER_SRC_UTIL_IO_H_
