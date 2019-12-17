
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
void stream<Ct,Vt,sz>::push(int len, sizebounded<Vt,sz> &b) const {
  if (_tgt) {
    int len2 = process(_config, len, b);
    _tgt->push(len2, b);
  } else {
    process(_config,len, b);
  }
}

template <typename Ct, typename Vt, int sz>
int stream<Ct,Vt,sz>::pull(sizebounded<Vt,sz> &b) const {
  if (_src) {
    int len = _src->pull(b);
    int len2 = process(_config, len, b);
    return len;
  } else {
    return process(_config, sz, b);
  }
}

