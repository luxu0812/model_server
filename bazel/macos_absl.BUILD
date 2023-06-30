cc_library(
  name = "base",
  hdrs = glob([
    "include/absl/base/**",
  ]),
  srcs = [
    "lib/libabsl_malloc_internal.dylib",
    "lib/libabsl_spinlock_wait.dylib",
    "lib/libabsl_raw_logging_internal.dylib",
    "lib/libabsl_log_severity.dylib",
    "lib/libabsl_base.dylib",
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
    "lib/libabsl_strings_internal.dylib"
  ],
  includes = ["include"],
)

cc_library(
  name = "strings",
  hdrs = glob([
    "include/absl/strings/*.h",
  ]),
  srcs = [
    "lib/libabsl_strings.dylib"
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
    "lib/libabsl_demangle_internal.dylib",
    "lib/libabsl_examine_stack.dylib",
    "lib/libabsl_stacktrace.dylib",
    "lib/libabsl_debugging_internal.dylib",
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
    "lib/libabsl_symbolize.dylib"
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
    "lib/libabsl_failure_signal_handler.dylib"
  ],
  includes = ["include"],
  deps = [
    ":debugging_internal",
    ":debugging_symbolize",
  ],
  visibility = ["//visibility:public"],
)
