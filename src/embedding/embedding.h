// Copyright (C) 2023 zh.luxu1986@gmail.com

#ifndef MODEL_SERVER_SRC_EMBEDDING_EMBEDDING_H_
#define MODEL_SERVER_SRC_EMBEDDING_EMBEDDING_H_

#include <vector>

namespace model_server {

class Embedding {
 public:
  Embedding() noexcept(false) {}
  ~Embedding() {}

  Embedding& operator=(const Embedding&) = delete;
  Embedding(const Embedding&) = delete;

  virtual void get_embedding(const std::vector<uint64_t>& ids, std::vector<float*> *embeddings) noexcept(false) = 0;
};

class LocalEmbedding : public Embedding {
 public:
  LocalEmbedding() noexcept(false) {}
  ~LocalEmbedding() {}

  LocalEmbedding& operator=(const LocalEmbedding&) = delete;
  LocalEmbedding(const LocalEmbedding&) = delete;

  void get_embedding(const std::vector<uint64_t>& ids, std::vector<float*> *embeddings) noexcept(false) override {}
};

class RemoteEmebedding : public Embedding {
 public:
  RemoteEmebedding() noexcept(false) {}
  ~RemoteEmebedding() {}

  RemoteEmebedding& operator=(const RemoteEmebedding&) = delete;
  RemoteEmebedding(const RemoteEmebedding&) = delete;

  void get_embedding(const std::vector<uint64_t>& ids, std::vector<float*> *embeddings) noexcept(false) override {}
};

}  // namespace model_server

#endif  // MODEL_SERVER_SRC_EMBEDDING_EMBEDDING_H_
