unit_test() {
  if [[ "${UNIT_TEST}" = true ]]; then
    bazelisk test //src:test_tf_engine   \
      --compilation_mode opt             \
      --jobs=10                          \
      --cxxopt='-std=c++17'              \
      --define "malloc=jemalloc"         \
      --test_output=all                  \
      --verbose_failures                 \
      --sandbox_debug                    \
      --test_verbose_timeout_warnings    \
      --dynamic_mode=off                 \
      --spawn_strategy=standalone        \
      --strategy=Genrule=standalone
    
    bazelisk test //src:test_onnx_engine \
      --compilation_mode opt             \
      --jobs=10                          \
      --cxxopt='-std=c++17'              \
      --define "malloc=jemalloc"         \
      --test_output=all                  \
      --verbose_failures                 \
      --sandbox_debug                    \
      --test_verbose_timeout_warnings    \
      --dynamic_mode=off                 \
      --spawn_strategy=standalone        \
      --strategy=Genrule=standalone
  fi
}

benchmark_test() {
  if [[ "${BENCHMARK_TEST}" = true ]]; then
    bazelisk test //src:bm_flat_hash_map \
      --compilation_mode opt             \
      --jobs=10                          \
      --cxxopt='-std=c++17'              \
      --define "malloc=jemalloc"         \
      --test_output=all                  \
      --verbose_failures                 \
      --sandbox_debug                    \
      --test_verbose_timeout_warnings    \
      --dynamic_mode=off                 \
      --spawn_strategy=standalone        \
      --strategy=Genrule=standalone
  fi
}