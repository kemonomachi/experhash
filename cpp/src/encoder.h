#ifndef EXPERHASH_ENCODER_H
#define EXPERHASH_ENCODER_H

#include <iostream>
#include <string>
#include <vector>

#include <ei.h>

namespace ExPerHash {
  class Encoder {
    ei_x_buff buffer;

    public:

    Encoder();
    ~Encoder();

    Encoder& write(std::ostream &out = std::cout);

    Encoder& encode_version();

    Encoder& encode_long(long p);
    Encoder& encode_ulong(unsigned long p);
    Encoder& encode_longlong(long long p);
    Encoder& encode_ulonglong(unsigned long long p);
    Encoder& encode_double(double p);
    Encoder& encode_boolean(bool p);
    Encoder& encode_char(char p);
    Encoder& encode_string(const std::string &str);
    Encoder& encode_atom(const std::string &atom);
    //Encoder& encode_atom_as(std::string atom, erlang_char_encoding from_enc, erlang_char_encoding to_enc);
    template<typename T> Encoder& encode_binary(const std::vector<T> &binary);
    Encoder& encode_binary(const void *binary, long len);
    Encoder& encode_pid(const erlang_pid &p);
    Encoder& encode_fun(const erlang_fun &p);
    Encoder& encode_port(const erlang_port &p);
    Encoder& encode_ref(const erlang_ref &p);
    Encoder& encode_trace(const erlang_trace &p);

    Encoder& encode_tuple_header(int arity);
    Encoder& encode_list_header(int arity);
    Encoder& encode_empty_list();
    Encoder& encode_map_header(int arity);
  };
}

#endif

