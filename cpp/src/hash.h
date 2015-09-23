#ifndef EXPERHASH_HASH_H
#define EXPERHASH_HASH_H

#include <stdint.h>
#include <string>
#include <vector>

namespace ExPerHash {
  std::vector<uint8_t> a_hash(const std::string &filename);
  std::vector<uint8_t> d_hash(const std::string &filename);
  std::vector<uint8_t> dd_hash(const std::string &filename);
}

#endif

