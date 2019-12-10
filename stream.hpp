#pragma once

#include "sizebounded.hpp"

template <typename T, int sz>
class stream
{
  public:
    stream(stream *src, stream *tgt);
    virtual ~stream();

    void push(sizebounded<T,sz> &) const;
    sizebounded<T,sz>& pull() const;

    virtual sizebounded<T,sz>& process(sizebounded<T,sz>&) const = 0;

  private:
    stream *_src;
    stream *_tgt;
};

