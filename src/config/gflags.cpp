// Copyright (C) 2023 zh.luxu1986@gmail.com

#include "model_server/src/config/gflags.h"

DEFINE_string(host, "", "Host");
DEFINE_int32(port, 8610, "Port");

DEFINE_int32(number_of_inference_workers, 16, "The number of inference workers");
DEFINE_int32(batch_size, 128, "Batch size");

DEFINE_string(local_model_dir, "", "Local model directory");
DEFINE_int32(number_of_test_cases, 1000, "Then number of test cases");
DEFINE_int32(number_of_producers, 1, "The number of producers");
DEFINE_int32(number_of_consumers, 1, "The number of consumers");

DEFINE_int32(engine_opt_level, 1, "Optimization level");
DEFINE_int32(engine_jit_level, 0, "JIT level");
DEFINE_int32(engine_inter_op_parallelism_threads, 16, "Inter op parallelism threads");
DEFINE_int32(engine_intra_op_parallelism_threads, 16, "Intra op parallelism threads");
DEFINE_bool(engin_use_global_thread_pool, true, "Use global thread pool");
DEFINE_bool(engine_ort_parrallel_execution, false, "ORT parallel execution");
