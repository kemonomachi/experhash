#include <ostream>
#include <string>

#include <ei.h>

#include "encoder.h"
#include "exception.h"

namespace ExPerHash {
  Encoder::Encoder() {
    if(ei_x_new(&buffer)) {
      throw EncodeError("response_buffer_allocation_failed");
    }
  }

  Encoder::~Encoder() {
    ei_x_free(&buffer);
  }

  Encoder& Encoder::write(std::ostream &out) {
    char header[4];

    for(int i = 0; i < 4; ++i) {
      header[i] = (buffer.index >> (3-i)*8) & 0xff;
    }

    if(!out.write(header, 4)) {
      throw IOError("Writing header to stream failed.");
    }

    if(!out.write(buffer.buff, buffer.index)) {
      throw IOError("Writing payload to stream failed.");
    }

    return *this;
  }

  Encoder& Encoder::encode_version() {
    if(ei_x_encode_version(&buffer)) {
      throw EncodeError("encode_version_failed");
    }

    return *this;
  }

  Encoder& Encoder::encode_long(long p) {
    if(ei_x_encode_long(&buffer, p)) {
      throw EncodeError("encode_long_failed");
    }

    return *this;
  }

  Encoder& Encoder::encode_ulong(unsigned long p) {
    if(ei_x_encode_ulong(&buffer, p)) {
      throw EncodeError("encode_ulong_failed");
    }

    return *this;
  }

  Encoder& Encoder::encode_longlong(long long p) {
    if(ei_x_encode_longlong(&buffer, p)) {
      throw EncodeError("encode_longlong_failed");
    }

    return *this;
  }

  Encoder& Encoder::encode_ulonglong(unsigned long long p) {
    if(ei_x_encode_ulonglong(&buffer, p)) {
      throw EncodeError("encode_ulonglong_failed");
    }

    return *this;
  }

  Encoder& Encoder::encode_double(double p) {
    if(ei_x_encode_double(&buffer, p)) {
      throw EncodeError("encode_double_failed");
    }

    return *this;
  }

  Encoder& Encoder::encode_boolean(bool p) {
    if(ei_x_encode_boolean(&buffer, p)) {
      throw EncodeError("encode_boolean_failed");
    }

    return *this;
  }

  Encoder& Encoder::encode_char(char p) {
    if(ei_x_encode_char(&buffer, p)) {
      throw EncodeError("encode_char_failed");
    }

    return *this;
  }

  Encoder& Encoder::encode_string(const std::string &str) {
    if(ei_x_encode_string(&buffer, str.c_str())) {
      throw EncodeError("encode_string_failed");
    }

    return *this;
  }

  Encoder& Encoder::encode_atom(const std::string &atom) {
    if(ei_x_encode_atom(&buffer, atom.c_str())) {
      throw EncodeError("encode_atom_failed");
    }

    return *this;
  }

  //Encoder& Encoder::encode_atom_as(std::string atom, erlang_char_encoding from_enc, erlang_char_encoding to_enc);
  
  template<typename T>
  Encoder& Encoder::encode_binary(const std::vector<T> &binary) {
    return encode_binary(binary.data(), binary.size() * sizeof(T));
  }

  template Encoder& Encoder::encode_binary(const std::vector<uint8_t> &binary);
  template Encoder& Encoder::encode_binary(const std::vector<uint16_t> &binary);
  template Encoder& Encoder::encode_binary(const std::vector<uint32_t> &binary);
  template Encoder& Encoder::encode_binary(const std::vector<uint64_t> &binary);

  Encoder& Encoder::encode_binary(const void *binary, long len) {
    if(ei_x_encode_binary(&buffer, binary, len)) {
      throw EncodeError("encode_binary_failed");
    }

    return *this;
  }
  
  Encoder& Encoder::encode_pid(const erlang_pid &p) {
    if(ei_x_encode_pid(&buffer, &p)) {
      throw EncodeError("encode_pid_failed");
    }

    return *this;
  }

  Encoder& Encoder::encode_fun(const erlang_fun &p) {
    if(ei_x_encode_fun(&buffer, &p)) {
      throw EncodeError("encode_fun_failed");
    }

    return *this;
  }

  Encoder& Encoder::encode_port(const erlang_port &p) {
    if(ei_x_encode_port(&buffer, &p)) {
      throw EncodeError("encode_port_failed");
    }

    return *this;
  }

  Encoder& Encoder::encode_ref(const erlang_ref &p) {
    if(ei_x_encode_ref(&buffer, &p)) {
      throw EncodeError("encode_ref_failed");
    }

    return *this;
  }

  Encoder& Encoder::encode_trace(const erlang_trace &p) {
    if(ei_x_encode_trace(&buffer, &p)) {
      throw EncodeError("encode_trace_failed");
    }

    return *this;
  }

  Encoder& Encoder::encode_tuple_header(int arity) {
    if(ei_x_encode_tuple_header(&buffer, arity)) {
      throw EncodeError("encode_tuple_header_failed");
    }

    return *this;
  }

  Encoder& Encoder::encode_list_header(int arity) {
    if(ei_x_encode_list_header(&buffer, arity)) {
      throw EncodeError("encode_list_header_failed");
    }

    return *this;
  }

  Encoder& Encoder::encode_empty_list() {
    if(ei_x_encode_empty_list(&buffer)) {
      throw EncodeError("encode_empty_list_failed");
    }

    return *this;
  }

  Encoder& Encoder::encode_map_header(int arity) {
    if(ei_x_encode_map_header(&buffer, arity)) {
      throw EncodeError("encode_map_header_failed");
    }

    return *this;
  }
}

