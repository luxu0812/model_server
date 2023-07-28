// Copyright 2023 zh.luxu1986@gmail.com

#ifndef MODEL_SERVER_SRC_DATA_RUNTIME_CONF_H_
#define MODEL_SERVER_SRC_DATA_RUNTIME_CONF_H_

namespace model_server {

struct RuntimeConf {
    int32_t opt_level;
    int32_t jit_level;

    int32_t inter_op_parallelism_threads;
    int32_t intra_op_parallelism_threads;
};

}  // namespace model_server

#endif  // MODEL_SERVER_SRC_DATA_RUNTIME_CONF_H_
