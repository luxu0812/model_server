cc_library(
  name = "base",
  hdrs = glob([
    "include/absl/base/**",
  ]),
  srcs = [
    "lib/libabsl_malloc_internal.a",
    "lib/libabsl_spinlock_wait.a",
    "lib/libabsl_raw_logging_internal.a",
    "lib/libabsl_log_severity.a",
    "lib/libabsl_base.a",
  ],
  includes = ["include"],
  visibility = ["//visibility:public"],
)

cc_library(
  name = "strings_internal",
  hdrs = glob([
    "include/absl/strings/internal/*.h",
  ]),
  srcs = [
    "lib/libabsl_strings_internal.a"
  ],
  includes = ["include"],
)

cc_library(
  name = "strings",
  hdrs = glob([
    "include/absl/strings/*.h",
  ]),
  srcs = [
    "lib/libabsl_strings.a"
  ],
  includes = ["include"],
  deps = [
    ":strings_internal",
  ],
  visibility = ["//visibility:public"],
)

cc_library(
  name = "debugging_internal",
  hdrs = glob([
    "include/absl/debugging/internal/*.h",
  ]),
  srcs = [
    "lib/libabsl_demangle_internal.a",
    "lib/libabsl_examine_stack.a",
    "lib/libabsl_stacktrace.a",
    "lib/libabsl_debugging_internal.a",
  ],
  deps = [
    ":base",
  ],
  includes = ["include"],
)

cc_library(
  name = "debugging_symbolize",
  hdrs = [
    "include/absl/debugging/symbolize.h",
  ],
  srcs = [
    "lib/libabsl_symbolize.a"
  ],
  includes = ["include"],
  deps = [
    ":debugging_internal",
    ":strings",
  ],
  visibility = ["//visibility:public"],
)

cc_library(
  name = "debugging_failure_signal_handler",
  hdrs = [
    "include/absl/debugging/failure_signal_handler.h",
  ],
  srcs = [
    "lib/libabsl_failure_signal_handler.a"
  ],
  includes = ["include"],
  deps = [
    ":debugging_internal",
    ":debugging_symbolize",
  ],
  visibility = ["//visibility:public"],
)
