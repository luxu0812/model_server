// Copyright (C) 2023 zh.luxu1986@gmail.com

#ifndef MODEL_SERVER_SRC_UTIL_SERIALIZE_H_
#define MODEL_SERVER_SRC_UTIL_SERIALIZE_H_

#include <string>
#include "google/protobuf/message.h"

namespace model_server {

std::string pb_to_json(const google::protobuf::Message& pb_msg);
void json_to_pb(const std::string& json_msg, google::protobuf::Message *pb_msg);

}

#endif  // MODEL_SERVER_SRC_UTIL_SERIALIZE_H_
