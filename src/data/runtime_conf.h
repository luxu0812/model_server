// Copyright 2023 zh.luxu1986@gmail.com

#ifndef INFER_ENGINE_SRC_DATA_RUNTIME_CONF_H_
#define INFER_ENGINE_SRC_DATA_RUNTIME_CONF_H_

namespace infer_engine {

struct RuntimeConf {
    int32_t opt_level;
    int32_t jit_level;

    int32_t inter_op_parallelism_threads;
    int32_t intra_op_parallelism_threads;
};

}  // namespace infer_engine

#endif  // INFER_ENGINE_SRC_DATA_RUNTIME_CONF_H_
