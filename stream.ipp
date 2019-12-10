
#include "sizebounded.hpp"
#include "sizebounded.ipp"
#include "stream.hpp"

template <typename T, int sz>
stream<T,sz>::stream(stream *s, stream *t) {
  _src = s;
  _tgt = t;
}

template <typename T, int sz>
stream<T,sz>::~stream() {
  _src = nullptr;
  _tgt = nullptr;
}

template <typename T, int sz>
void stream<T,sz>::push(sizebounded<T,sz> &b) const {
  if (_tgt) {
    _tgt->push(process(b));
  } else {
    process(b);
  }
}

template <typename T, int sz>
sizebounded<T,sz>& stream<T,sz>::pull() const {
  if (_src) {
    return process(_src->pull());
  } else {
    auto b = new sizebounded<T,sz>();
    return process(*b);
  }
}

