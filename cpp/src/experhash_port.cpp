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
          .encode_binary(hash.data(), hash.size())
          .write();
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

