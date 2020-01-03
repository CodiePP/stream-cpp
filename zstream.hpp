#pragma once

#include <iostream>
#include <zlib.h>
#include <functional>

#include "sizebounded.hpp"
#include "stream.hpp"


// deflate stream (compress data)
template <typename Ct, typename Vt, int sz>
class deflatestream : public stream<Ct,Vt,sz>
{
  public:
    deflatestream(Ct *c, stream<Ct,Vt,sz> *s, stream<Ct,Vt,sz> *t);
    deflatestream(stream<Ct,Vt,sz> *s, stream<Ct,Vt,sz> *t);
    virtual ~deflatestream();
    virtual int process(Ct const * const, int blen, sizebounded<Vt,sz> &b) const;
  private:
    mutable z_stream _strm;
    mutable sizebounded<Vt,sz> _buf;
    mutable bool _ok {false};
};

// inflate stream (uncompress data)
template <typename Ct, typename Vt, int sz>
class inflatestream : public stream<Ct,Vt,sz>
{
  public:
    inflatestream(Ct *c, stream<Ct,Vt,sz> *s, stream<Ct,Vt,sz> *t);
    inflatestream(stream<Ct,Vt,sz> *s, stream<Ct,Vt,sz> *t);
    virtual ~inflatestream();
    virtual int process(Ct const * const, int blen, sizebounded<Vt,sz> &b) const;
    virtual void push(int len, sizebounded<Vt,sz>&) const;
  private:
    mutable z_stream _strm;
    mutable sizebounded<Vt,sz> _buf;
    mutable bool _ok {false};
};
