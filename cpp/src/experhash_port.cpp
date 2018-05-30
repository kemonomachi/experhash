#include <string>
#include <stdexcept>
#include <stdint.h>
#include <vector>

#include <Magick++.h>

#include "decoder.h"
#include "encoder.h"
#include "exception.h"
#include "hash.h"

void write_error(std::string type, std::string reason);

template<typename uintX_t, int X>
std::vector<uintX_t> do_hash(const std::string type, const std::string filename);

template<typename uintX_t>
void send_hash_response(std::vector<uintX_t> hash);

int main(int argc, char *argv[]) {
  Magick::InitializeMagick(*argv);

  while(true) {
    try {
      ExPerHash::Decoder decoder;

      int arity  = decoder.decode_tuple_header();
      std::string command = decoder.decode_atom();

      if(arity == 3 && command == "hash") {
        std::string type = decoder.decode_atom();
        std::string filename = decoder.decode_binary_string();

        std::vector<uint8_t> hash;

        if(type == "a") {
          hash = ExPerHash::a_hash<uint8_t, 8>(filename);
        }
        else if(type == "d") {
          hash = ExPerHash::d_hash<uint8_t, 8>(filename);
        }
        else if(type == "dd") {
          hash = ExPerHash::dd_hash<uint8_t, 8>(filename);
        }
        else {
          throw ExPerHash::DecodeError("unknown_hash_type");
        }

        ExPerHash::Encoder response;

        response
          .encode_version()
          .encode_tuple_header(2)
          .encode_atom("ok")
          .encode_binary(hash)
          .write();
      }
      else if(arity == 4 && command == "hash") {
        std::string type = decoder.decode_atom();
        std::string filename = decoder.decode_binary_string();
        long hash_size = decoder.decode_long();

        switch(hash_size) {
          case 8:
            send_hash_response(do_hash<uint8_t, 8>(type, filename));
            break;

          case 16:
            send_hash_response(do_hash<uint16_t, 16>(type, filename));
            break;

          case 32:
            send_hash_response(do_hash<uint32_t, 32>(type, filename));
            break;

          case 64:
            send_hash_response(do_hash<uint64_t, 64>(type, filename));
            break;

          default:
            throw ExPerHash::DecodeError("illegal_hash_size");
            break;
        }
      }
      else if(arity == 3 && command == "hamming_distance") {
        std::vector<uint8_t> hash1 = decoder.decode_binary();
        std::vector<uint8_t> hash2 = decoder.decode_binary();
        
        int dist = 0;

        for(std::vector<uint8_t>::size_type i = 0; i < hash1.size(); ++i) {
          //Algorithm by Peter Wegner. See https://en.wikipedia.org/wiki/Hamming_distance#Algorithm_example
          uint8_t val = hash1[i] ^ hash2[i];

          while(val != 0) {
            ++dist;
            val &= val - 1;
          }
        }

        ExPerHash::Encoder response;

        response
          .encode_version()
          .encode_tuple_header(2)
          .encode_atom("ok")
          .encode_long(dist)
          .write();
      }
      else {
        write_error("comm_error", "unknown_command");
      }
    }
    catch(ExPerHash::EndOfFile&) {
      return 0;
    }
    catch(ExPerHash::DecodeError &ex) {
      write_error("decode_error", ex.what());
    }
    catch(ExPerHash::EncodeError &ex) {
      write_error("encode_error", ex.what());
    }
  }
}

template<typename uintX_t, int X>
std::vector<uintX_t> do_hash(const std::string type, const std::string filename) {
  if(type == "a") {
    return ExPerHash::a_hash<uintX_t, X>(filename);
  }
  else if(type == "d") {
    return ExPerHash::d_hash<uintX_t, X>(filename);
  }
  else if(type == "dd") {
    return ExPerHash::dd_hash<uintX_t, X>(filename);
  }
  else {
    throw ExPerHash::DecodeError("unknown_hash_type");
  }
}

template<typename uintX_t>
void send_hash_response(std::vector<uintX_t> hash) {
  ExPerHash::Encoder response;

  response
    .encode_version()
    .encode_tuple_header(2)
    .encode_atom("ok")
    .encode_binary(hash)
    .write();
}

void write_error(std::string type, std::string reason) {
  ExPerHash::Encoder error;

  error
    .encode_version()
    .encode_tuple_header(2)
    .encode_atom("error")
    .encode_tuple_header(2)
    .encode_atom(type.c_str())
    .encode_atom(reason.c_str())
    .write();
}

