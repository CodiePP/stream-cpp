
#include "sizebounded/sizebounded.hpp"
#include "sizebounded/sizebounded.ipp"
#include "stream-cpp/stream.hpp"

template <typename Ct, typename St, typename Vt, int sz>
stream<Ct,St,Vt,sz>::stream(Ct const * const c, St *st, stream *s, stream *t) {
  _config = c;
  _state = st;
  _src = s;
  _tgt = t;
}

template <typename Ct, typename St, typename Vt, int sz>
stream<Ct,St,Vt,sz>::stream(St *st, stream *s, stream *t) {
  _state = st;
  _src = s;
  _tgt = t;
}

template <typename Ct, typename St, typename Vt, int sz>
stream<Ct,St,Vt,sz>::~stream() {
  _proc= nullptr;
  _config = nullptr;
  _state = nullptr;
  _src = nullptr;
  _tgt = nullptr;
}

template <typename Ct, typename St, typename Vt, int sz>
void stream<Ct,St,Vt,sz>::processor(std::function<int(Ct const * const, St *, int, sizebounded<Vt,sz>&)> p) {
  _proc = p;
}

template <typename Ct, typename St, typename Vt, int sz>
int stream<Ct,St,Vt,sz>::process(Ct const * const, St *, int len, sizebounded<Vt,sz>&) const {
  return 0;
}

template <typename Ct, typename St, typename Vt, int sz>
void stream<Ct,St,Vt,sz>::push(int len, sizebounded<Vt,sz> &b) const {
  if (_tgt) {
    int len2 = _proc?_proc(_config,_state,len,b):process(_config, _state, len, b);
    _tgt->push(len2, b);
  } else {
    _proc?_proc(_config,_state,len,b):process(_config, _state, len, b);
  }
}

template <typename Ct, typename St, typename Vt, int sz>
int stream<Ct,St,Vt,sz>::pull(sizebounded<Vt,sz> &b) const {
  if (_src) {
    int len = _src->pull(b);
    int len2 = _proc?_proc(_config,_state,len,b):process(_config, _state, len, b);
    return len;
  } else {
    return _proc?_proc(_config,_state,sz,b):process(_config, _state, sz, b);
  }
}

