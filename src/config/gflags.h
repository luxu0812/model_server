// Copyright (C) 2023 zh.luxu1986@gmail.com

#ifndef MODEL_SERVER_SRC_CONFIG_GFLAGS_H_
#define MODEL_SERVER_SRC_CONFIG_GFLAGS_H_

#include "gflags/gflags.h"

DECLARE_string(host);
DECLARE_int32(port);

DECLARE_int32(number_of_inference_workers);
DECLARE_int32(batch_size);

DECLARE_string(local_model_dir);
DECLARE_int32(number_of_test_cases);
DECLARE_int32(number_of_producers);
DECLARE_int32(number_of_consumers);

DECLARE_int32(engine_opt_level);
DECLARE_int32(engine_jit_level);
DECLARE_int32(engine_inter_op_parallelism_threads);
DECLARE_int32(engine_intra_op_parallelism_threads);
DECLARE_bool(engin_use_global_thread_pool);
DECLARE_bool(engine_ort_parrallel_execution);

#endif  // MODEL_SERVER_SRC_CONFIG_GFLAGS_H_
