// Copyright (C) 2023 zh.luxu1986@gmail.com

#include "model_server/src/util/serialize.h"

#include "glog/logging.h"
#include "google/protobuf/message.h"
#include "google/protobuf/util/json_util.h"

namespace model_server {

bool pb_to_json(const google::protobuf::Message& pb_msg, std::string *json_msg) {
  if (nullptr == json_msg) {
    return false;
  }

  google::protobuf::util::JsonPrintOptions options;
  options.add_whitespace = true;
  options.always_print_primitive_fields = true;
  options.preserve_proto_field_names = true;

  return google::protobuf::util::MessageToJsonString(pb_msg, json_msg, options).ok();
}

bool json_to_pb(const std::string& json_msg, google::protobuf::Message *pb_msg) {
  if (nullptr == pb_msg) {
    return false;
  }

  google::protobuf::util::JsonParseOptions options;
  options.ignore_unknown_fields = true;

  auto ret = google::protobuf::util::JsonStringToMessage(json_msg, pb_msg, options);
  if (!ret.ok()) {
    LOG(ERROR) << "json to pb failed, json: " << json_msg << ", error: " << ret.message();
  }

  return ret.ok();
}

}  // namespace model_server
