
#include "sizebounded.hpp"
#include "sizebounded.ipp"
#include "stream.hpp"

template <typename Ct, typename Vt, int sz>
stream<Ct,Vt,sz>::stream(Ct *c, stream *s, stream *t) {
  _config = c;
  _src = s;
  _tgt = t;
}

template <typename Ct, typename Vt, int sz>
stream<Ct,Vt,sz>::stream(stream *s, stream *t) {
  _src = s;
  _tgt = t;
}

template <typename Ct, typename Vt, int sz>
stream<Ct,Vt,sz>::~stream() {
  _config = nullptr;
  _src = nullptr;
  _tgt = nullptr;
}

template <typename Ct, typename Vt, int sz>
void stream<Ct,Vt,sz>::push(sizebounded<Vt,sz> &b) const {
  if (_tgt) {
    _tgt->push(process(_config, b));
  } else {
    process(_config, b);
  }
}

template <typename Ct, typename Vt, int sz>
sizebounded<Vt,sz>& stream<Ct,Vt,sz>::pull() const {
  if (_src) {
    return process(_config, _src->pull());
  } else {
    auto b = new sizebounded<Vt,sz>();
    return process(_config, *b);
  }
}

