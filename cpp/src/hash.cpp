/*
  aHash algorithm by dr. Neal Krawitz, see http://www.hackerfactor.com/blog/index.php?/archives/432-Looks-Like-It.html

  dHash algorithm by dr Neal Krawitz and David Oftedal, see http://www.hackerfactor.com/blog/index.php?/archives/529-Kind-of-Like-That.html
  and http://01101001.net/programming.php.

  ddHash is a double dHash, one row-wise and one column-wise.
*/

#include <cstring>
#include <numeric>
#include <stdint.h>
#include <string>
#include <vector>

#include <Magick++.h>

#include "hash.h"

namespace ExPerHash {
  const int A_HASH_SIZE = 8;
  const int D_HASH_SIZE = 8;
  const int DD_HASH_SIZE = 2*D_HASH_SIZE;

  std::vector<uint8_t> read_pixels(const std::string &filename, const size_t width, const size_t height) {
    Magick::Geometry geo(width, height);
    geo.aspect(true);

    Magick::Image img(filename);
    img.resize(geo);
    img.type(Magick::GrayscaleType);

    std::vector<uint8_t> pixels(width * height);

    img.write(0, 0, width, height, "R", Magick::CharPixel, pixels.data());

    return pixels;
  }

  template<typename uintX_t, int X>
  std::vector<uintX_t> a_hash(const std::string &filename) {
    std::vector<uint8_t> pixels = read_pixels(filename, X, X);

    int average = std::accumulate(pixels.begin(), pixels.end(), 0) / pixels.size();

    std::vector<uintX_t> hash(X);

    for(int i = 0; i < pixels.size(); ++i) {
      hash[i/X] |= (pixels[i] > average) << (X - 1 - i%X);
    }

    return hash;
  }

  template std::vector<uint8_t> a_hash<uint8_t, 8>(const std::string &filename);
  template std::vector<uint16_t> a_hash<uint16_t, 16>(const std::string &filename);
  template std::vector<uint32_t> a_hash<uint32_t, 32>(const std::string &filename);
  template std::vector<uint64_t> a_hash<uint64_t, 64>(const std::string &filename);

  template<typename uintX_t, int X>
  std::vector<uintX_t> d_hash(const std::string &filename) {
    size_t height = X;
    size_t width = height + 1;

    std::vector<uint8_t> pixels = read_pixels(filename, width, height);

    std::vector<uintX_t> hash(X);

    for(int row = 0; row < height; ++row) {
      for(int col = 0; col < width-1; ++col) {
        hash[row] |= (pixels[row*width + col] < pixels[row*width + col+1]) << (X - 1 - col);
      }
    }

    return hash;
  }

  template std::vector<uint8_t> d_hash<uint8_t, 8>(const std::string &filename);
  template std::vector<uint16_t> d_hash<uint16_t, 16>(const std::string &filename);
  template std::vector<uint32_t> d_hash<uint32_t, 32>(const std::string &filename);
  template std::vector<uint64_t> d_hash<uint64_t, 64>(const std::string &filename);

  template<typename uintX_t, int X>
  std::vector<uintX_t> dd_hash(const std::string &filename) {
    size_t size = X + 1;

    std::vector<uint8_t> pixels = read_pixels(filename, size, size);

    int hash_size = 2 * X;
    std::vector<uintX_t> hash(hash_size);
    uintX_t *row_hash = hash.data();
    uintX_t *col_hash = hash.data() + hash_size/2;

    for(int row = 0; row < size-1; ++row) {
      for(int col = 0; col < size-1; ++col) {
        row_hash[row] |= (pixels[row*size + col] < pixels[row*size + col+1]) << (X - 1 - col);
        col_hash[col] |= (pixels[row*size + col] < pixels[(row+1)*size + col]) << (X - 1 - row);
      }
    }

    return hash;
  }

  template std::vector<uint8_t> dd_hash<uint8_t, 8>(const std::string &filename);
  template std::vector<uint16_t> dd_hash<uint16_t, 16>(const std::string &filename);
  template std::vector<uint32_t> dd_hash<uint32_t, 32>(const std::string &filename);
  template std::vector<uint64_t> dd_hash<uint64_t, 64>(const std::string &filename);
}

