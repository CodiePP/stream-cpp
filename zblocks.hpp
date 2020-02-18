#pragma once

#include <iostream>
#include <zlib.h>
#include <functional>

#include "sizebounded/sizebounded.hpp"
#include "stream-cpp/stream.hpp"


// deflate stream (compress data)
template <typename Ct, typename St, typename Vt, int sz>
class deflatestream : public stream<Ct,St,Vt,sz>
{
  public:
    deflatestream(Ct const * const c, St *st, stream<Ct,St,Vt,sz> *s, stream<Ct,St,Vt,sz> *t);
    deflatestream(stream<Ct,St,Vt,sz> *s, stream<Ct,St,Vt,sz> *t) : deflatestream(nullptr,nullptr,s,t) {}
    virtual ~deflatestream();
    virtual int process(Ct const * const, St *, int blen, sizebounded<Vt,sz> &b) const;
  private:
    mutable z_stream _strm;
    mutable sizebounded<Vt,sz> _buf;
    mutable bool _ok {false};
};

// inflate stream (uncompress data)
template <typename Ct, typename St, typename Vt, int sz>
class inflatestream : public stream<Ct,St,Vt,sz>
{
  public:
    inflatestream(Ct const * const c, St *st, stream<Ct,St,Vt,sz> *s, stream<Ct,St,Vt,sz> *t);
    inflatestream(stream<Ct,St,Vt,sz> *s, stream<Ct,St,Vt,sz> *t) : inflatestream(nullptr,nullptr,s,t) {}
    virtual ~inflatestream();
    virtual int process(Ct const * const, St *, int blen, sizebounded<Vt,sz> &b) const;
    virtual void push(int len, sizebounded<Vt,sz>&) const;
  private:
    mutable z_stream _strm;
    mutable sizebounded<Vt,sz> _buf;
    mutable bool _ok {false};
};
