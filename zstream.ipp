
#include "stream-cpp/stream.hpp"
#include "stream-cpp/stream.ipp"
#include "stream-cpp/zstream.hpp"

#include <cstring>

/* the size of the window */
#define WINDOW_BITS -12

template <typename Ct, typename St, typename Vt, int sz>
deflatestream<Ct,St,Vt,sz>::deflatestream(Ct const * const c, St *st, stream<Ct,St,Vt,sz> *s, stream<Ct,St,Vt,sz> *t)
    : stream<Ct,St,Vt,sz>(c,st,s,t)
{
    _strm.zalloc = Z_NULL;
    _strm.zfree = Z_NULL;
    _strm.opaque = Z_NULL;
    int res;
    res = deflateInit2(
            &_strm,
            Z_BEST_COMPRESSION, // level
            Z_DEFLATED, // method
            WINDOW_BITS, // window bits
            9, // memory level; controls allocation of internal buffers
            Z_DEFAULT_STRATEGY // tune the strategy
        );
    if (res != Z_OK) {
        std::cout << "failed to initialise deflate state: " << res << std::endl;
        deflateEnd(&_strm);
        _ok = false;
    } else {
        _ok = true;
    }
}

template <typename Ct, typename St, typename Vt, int sz>
deflatestream<Ct,St,Vt,sz>::~deflatestream() {
    if (_ok) {
        deflateEnd(&_strm);
    }
    _ok = false;
}

template <typename Ct, typename St, typename Vt, int sz>
int deflatestream<Ct,St,Vt,sz>::process(Ct const * const cfg, St *st, int len, sizebounded<Vt,sz> & inbuf) const {
    if (! _ok) {
        std::cout << "cannot process deflate stream: not OK!" << std::endl;
        return 0; }
    _strm.avail_in = len;
    _strm.next_in = (unsigned char*)inbuf.ptr();
    _strm.avail_out = _buf.size();
    _strm.next_out = (unsigned char*)_buf.ptr();
    int res = deflate(&_strm, len==0?Z_FINISH:Z_NO_FLUSH);
    if (res != Z_OK && res != Z_STREAM_END) {
        std::cout << "failed to deflate: " << res << std::endl;
        deflateEnd(&_strm);
        _ok = false;
        return -1;
    }
    // std::cout << "result: " << res << std::endl;
    // std::cout << "compressed in " << (len - _strm.avail_in) << " of " << len << " bytes." << std::endl;
    // std::cout << "compressed out " << (_buf.size() - _strm.avail_out) << " of " << _buf.size() << " bytes." << std::endl;
    int ncomp = _buf.size() - _strm.avail_out;
    std::memcpy((void*)inbuf.ptr(), _buf.ptr(), ncomp);
    return ncomp;
}

template <typename Ct, typename St, typename Vt, int sz>
inflatestream<Ct,St,Vt,sz>::inflatestream(Ct const * const c, St *st, stream<Ct,St,Vt,sz> *s, stream<Ct,St,Vt,sz> *t)
    : stream<Ct,St,Vt,sz>(c,st,s,t)
{
  _strm.zalloc = Z_NULL;
  _strm.zfree = Z_NULL;
  _strm.opaque = Z_NULL;
  int res;
  res = inflateInit2(
      &_strm, // z_streamp
      WINDOW_BITS  // window bits
  );
  if (res != Z_OK) {
      std::cout << "failed to initialise inflate state: " << res << std::endl;
      inflateEnd(&_strm);
      _ok = false;
  } else {
      _ok = true;
  }
}

template <typename Ct, typename St, typename Vt, int sz>
inflatestream<Ct,St,Vt,sz>::~inflatestream() {
    if (_ok) {
        inflateEnd(&_strm);
    }
    _ok = false;
}

template <typename Ct, typename St, typename Vt, int sz>
int inflatestream<Ct,St,Vt,sz>::process(Ct const * const cfg, St *st, int len, sizebounded<Vt,sz> & inbuf) const {
    if (! _ok) { return 0; }
    _strm.avail_in = len;
    _strm.next_in = (unsigned char*)inbuf.ptr();
    _strm.avail_out = _buf.size();
    _strm.next_out = (unsigned char*)_buf.ptr();
    int res = inflate(&_strm, len==0?Z_FINISH:Z_NO_FLUSH);
    if (res != Z_OK && res != Z_STREAM_END) {
      std::cout << "failed to inflate: " << res << std::endl;
      inflateEnd(&_strm);
      return {};
    }
    // std::cout << "result: " << res << std::endl;
    // std::cout << "decompressed in " << (len - _strm.avail_in) << " of " << len << " bytes." << std::endl;
    // std::cout << "decompressed out " << (sz - _strm.avail_out) << " of " << sz << " bytes." << std::endl;
    int ndecmp = _buf.size() - _strm.avail_out;
    std::memcpy((void*)inbuf.ptr(), _buf.ptr(), ndecmp);
    return ndecmp;
}

template <typename Ct, typename St, typename Vt, int sz>
void inflatestream<Ct,St,Vt,sz>::push(int len, sizebounded<Vt,sz>& buf) const {
    sizebounded<Vt,sz> orig {buf};
    stream<Ct,St,Vt,sz>::push(len, buf);
    while (_strm.avail_in > 0) {
        std::memcpy((void*)buf.ptr(), orig.ptr() + (len - _strm.avail_in), _strm.avail_in);
        stream<Ct,St,Vt,sz>::push(_strm.avail_in, buf);
    }
}
