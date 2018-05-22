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
  const int SIZE = 8;
  const int PIXEL_COUNT = SIZE*SIZE;
  const int A_HASH_SIZE = PIXEL_COUNT / 8;
  const int D_HASH_SIZE = PIXEL_COUNT / 8;
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

  std::vector<uint8_t> read_pixels(const std::string &filename) {
    return read_pixels(filename, SIZE, SIZE);
  }

  std::vector<uint8_t> a_hash(const std::string &filename) {
    std::vector<uint8_t> pixels = read_pixels(filename);

    int average = std::accumulate(pixels.begin(), pixels.end(), 0) / PIXEL_COUNT;

    std::vector<uint8_t> hash(A_HASH_SIZE);

    for(int i = 0; i < PIXEL_COUNT; ++i) {
      hash[i/8] |= (pixels[i] > average) << (7 - i%8);
    }

    return hash;
  }

  std::vector<uint8_t> d_hash(const std::string &filename) {
    size_t height = 8;
    size_t width = height + 1;

    std::vector<uint8_t> pixels = read_pixels(filename, width, height);

    std::vector<uint8_t> hash(D_HASH_SIZE);

    for(int row = 0; row < height; ++row) {
      for(int col = 0; col < width-1; ++col) {
        hash[row] |= (pixels[row*width + col] < pixels[row*width + col+1]) << (7-col);
      }
    }

    return hash;
  }

  std::vector<uint8_t> dd_hash(const std::string &filename) {
    std::vector<uint8_t> pixels = read_pixels(filename);
    pixels.push_back(pixels[0]);

    std::vector<uint8_t> hash(DD_HASH_SIZE);
    uint8_t *row_hash = hash.data();
    uint8_t *col_hash = hash.data() + DD_HASH_SIZE/2;

    for(int i = 0; i < SIZE; ++i) {
      for(int j = 0; j < SIZE; ++j) {
        row_hash[i] |= (pixels[i*SIZE + j] < pixels[i*SIZE + j+1]) << (7-j);
        col_hash[i] |= (pixels[i + j*SIZE] < pixels[(i + (j+1)*SIZE)%PIXEL_COUNT + j/7]) << (7-j);
      }
    }

    return hash;
  }
}

