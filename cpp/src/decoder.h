#ifndef EXPERHASH_DECODER_H
#define EXPERHASH_DECODER_H

#include <iostream>
#include <string>
#include <utility>
#include <vector>

#include <ei.h>

namespace ExPerHash {
  class Decoder {
    char *buffer;
    int index;
    
    int version;

    void try_read(std::istream &in, char *buffer, int len);

    public:
    
    Decoder() : Decoder(std::cin) {}
    Decoder(std::istream &in);
    Decoder(char *in_buffer, int len);
    ~Decoder();

    std::pair<int, int> get_type_and_size();
    int get_type();
    int get_size();
    long decode_long();
    unsigned long decode_ulong();
    long long decode_longlong();
    unsigned long long decode_ulonglong();
    double decode_double();
    bool decode_boolean();
    char decode_char();
    std::string decode_string();
    std::string decode_atom();
    // ??? decode_atom(erlang_char_encoding wants);
    std::vector<uint8_t> decode_binary();
    std::string decode_binary_string();
    erlang_fun decode_fun();
    erlang_pid decode_pid();
    erlang_port decode_port();
    erlang_ref decode_ref();
    erlang_trace decode_trace();

    int decode_tuple_header();
    int decode_list_header();
    int decode_map_header();

    ei_term decode_ei_term();

    bool skip_term();
  };
}

#endif

