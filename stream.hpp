#pragma once

#include "sizebounded.hpp"

template <typename Ct, typename Vt, int sz>
class stream
{
  public:
    stream(stream *src, stream *tgt);
    stream(Ct *config, stream *src, stream *tgt);
    virtual ~stream();

    void push(int len, sizebounded<Vt,sz>&) const;
    int pull(sizebounded<Vt,sz>&) const;

    virtual int process(Ct const * const, int len, sizebounded<Vt,sz>&) const = 0;

  private:
    Ct *_config;
    stream *_src;
    stream *_tgt;
};

