#ifndef EXPERHASH_EXCEPTION_H
#define EXPERHASH_EXCEPTION_H

#include <stdexcept>

namespace ExPerHash {
  class ErrorBase : public std::runtime_error {
    public:
      using std::runtime_error::runtime_error;
  };

  class DecodeError : public ErrorBase {
    public:
      using ErrorBase::ErrorBase;
  };

  class EncodeError : public ErrorBase {
    public:
      using ErrorBase::ErrorBase;
  };

  class EndOfFile : public ErrorBase {
    public:
      using ErrorBase::ErrorBase;
  };

  class IOError : public ErrorBase {
    public:
      using ErrorBase::ErrorBase;
  };
}

#endif

