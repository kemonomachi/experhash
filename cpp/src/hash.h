#ifndef EXPERHASH_HASH_H
#define EXPERHASH_HASH_H

#include <stdint.h>
#include <string>
#include <vector>

namespace ExPerHash {
  template<typename uintX_t, int X>
  std::vector<uintX_t> a_hash(const std::string &filename);

  template<typename uintX_t, int X>
  std::vector<uintX_t> d_hash(const std::string &filename);

  template<typename uintX_t, int X>
  std::vector<uintX_t> dd_hash(const std::string &filename);
}

#endif

