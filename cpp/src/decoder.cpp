#include <cstring>
#include <istream>
#include <string>
#include <utility>
#include <vector>
#include <stdexcept>

#include <ei.h>

#include "decoder.h"
#include "exception.h"

namespace ExPerHash {
  Decoder::Decoder(std::istream &in) : index(0) {
    char header_buf[4];

    try_read(in, header_buf, 4);

    int header = (header_buf[0] << 24)
               | (header_buf[1] << 16)
               | (header_buf[2] << 8)
               | header_buf[3];

    buffer = new char[header];

    try_read(in, buffer, header);

    if(ei_decode_version(buffer, &index, &version)) {
      throw DecodeError("bad_version");
    }
  }

  void Decoder::try_read(std::istream &in, char *buffer, int len) {
    if(!in.read(buffer, len)) {
      if(in.eof()) {
        throw EndOfFile("Reached end of stream while reading input.");
      }
      else {
        throw IOError("Error reading from stream.");
      }
    }
  }

  Decoder::Decoder(char *in_buffer, int len) : index(0) {
    buffer = new char[len];

    std::memcpy(buffer, in_buffer, len);
  }

  Decoder::~Decoder() {
    delete[] buffer;
  }

  std::pair<int, int> Decoder::get_type_and_size() {
    int type, size;

    if(ei_get_type(buffer, &index, &type, &size)) {
      throw DecodeError("bad_type");
    }

    return std::make_pair(type, size);
  }

  int Decoder::get_type() {
    return get_type_and_size().first;
  }

  int Decoder::get_size() {
    return get_type_and_size().second;
  }

  long Decoder::decode_long() {
    long ret;

    if(ei_decode_long(buffer, &index, &ret)) {
      throw DecodeError("bad_long");
    }

    return ret;
  }

  unsigned long Decoder::decode_ulong() {
    unsigned long ret;

    if(ei_decode_ulong(buffer, &index, &ret)) {
      throw DecodeError("bad_ulong");
    }

    return ret;
  }

  long long Decoder::decode_longlong() {
    long long ret;

    if(ei_decode_longlong(buffer, &index, &ret)) {
      throw DecodeError("bad_longlong");
    }

    return ret;
  }

  unsigned long long Decoder::decode_ulonglong() {
    unsigned long long ret;

    if(ei_decode_ulonglong(buffer, &index, &ret)) {
      throw DecodeError("bad_ulonglong");
    }

    return ret;
  }

  double Decoder::decode_double() {
    double ret;

    if(ei_decode_double(buffer, &index, &ret)) {
      throw DecodeError("bad_double");
    }

    return ret;
  }

  bool Decoder::decode_boolean() {
    int ret;

    if(ei_decode_boolean(buffer, &index, &ret)) {
      throw DecodeError("bad_boolean");
    }

    return ret != 0;
  }

  char Decoder::decode_char() {
    char ret;

    if(ei_decode_char(buffer, &index, &ret)) {
      throw DecodeError("bad_char");
    }

    return ret;
  }

  std::string Decoder::decode_string() {
    char *temp = new char[get_size()+1];

    if(ei_decode_string(buffer, &index, temp)) {
      delete[] temp;
      throw DecodeError("bad_string");
    }

    std::string ret(temp);

    delete[] temp;

    return ret;
  }

  std::string Decoder::decode_atom() {
    char ret[MAXATOMLEN+1];

    if(ei_decode_atom(buffer, &index, ret)) {
      throw DecodeError("bad_atom");
    }

    return ret;
  }

  std::vector<uint8_t> Decoder::decode_binary() {
    uint8_t *temp = new uint8_t[get_size()];
    long len;

    if(ei_decode_binary(buffer, &index, reinterpret_cast<char*>(temp), &len)) {
      delete[] temp;
      throw DecodeError("bad_binary");
    }

    std::vector<uint8_t> ret(temp, temp+len);

    delete[] temp;

    return ret;
  }

  std::string Decoder::decode_binary_string() {
    char *temp = new char[get_size()];
    long len;

    if(ei_decode_binary(buffer, &index, temp, &len)) {
      delete[] temp;
      throw DecodeError("bad_binary_string");
    }

    std::string ret(temp, len);

    delete[] temp;

    return ret;
  }

  erlang_fun Decoder::decode_fun() {
    erlang_fun* ret = NULL;

    if(ei_decode_fun(buffer, &index, ret)) {
      throw DecodeError("bad_fun");
    }

    return *ret;
  }

  erlang_pid Decoder::decode_pid() {
    erlang_pid ret;

    if(ei_decode_pid(buffer, &index, &ret)) {
      throw DecodeError("bad_pid");
    }

    return ret;
  }

  erlang_port Decoder::decode_port() {
    erlang_port ret;

    if(ei_decode_port(buffer, &index, &ret)) {
      throw DecodeError("bad_port");
    }

    return ret;
  }

  erlang_ref Decoder::decode_ref() {
    erlang_ref ret;

    if(ei_decode_ref(buffer, &index, &ret)) {
      throw DecodeError("bad_ref");
    }

    return ret;
  }

  erlang_trace Decoder::decode_trace() {
    erlang_trace ret;

    if(ei_decode_trace(buffer, &index, &ret)) {
      throw DecodeError("bad_trace");
    }

    return ret;
  }


  int Decoder::decode_tuple_header() {
    int arity;

    if(ei_decode_tuple_header(buffer, &index, &arity)) {
      throw DecodeError("bad_tuple_header");
    }

    return arity;
  }

  int Decoder::decode_list_header() {
    int arity;

    if(ei_decode_list_header(buffer, &index, &arity)) {
      throw DecodeError("bad_list_header");
    }

    return arity;
  }

  int Decoder::decode_map_header() {
    int arity;

    if(ei_decode_map_header(buffer, &index, &arity)) {
      throw DecodeError("bad_map_header");
    }

    return arity;
  }

  ei_term Decoder::decode_ei_term() {
    ei_term ret;

    if(ei_decode_ei_term(buffer, &index, &ret) != 1) {
      throw DecodeError("bad_ei_term");
    }

    return ret;
  }

  bool Decoder::skip_term() {
    return 0 != ei_skip_term(buffer, &index);
  }
}

