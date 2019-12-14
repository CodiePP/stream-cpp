#pragma once

#include "sizebounded.hpp"

template <typename Ct, typename Vt, int sz>
class stream
{
  public:
    stream(stream *src, stream *tgt);
    stream(Ct *config, stream *src, stream *tgt);
    virtual ~stream();

    void push(sizebounded<Vt,sz>&) const;
    sizebounded<Vt,sz>& pull() const;

    virtual sizebounded<Vt,sz>& process(Ct const * const, sizebounded<Vt,sz>&) const = 0;

  private:
    Ct *_config;
    stream *_src;
    stream *_tgt;
};

