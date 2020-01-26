#pragma once

#include <functional>

#include "sizebounded/sizebounded.hpp"

template <typename Ct, typename Vt, int sz>
class stream
{
  public:
    stream(stream *src, stream *tgt);
    stream(Ct *config, stream *src, stream *tgt);
    virtual ~stream();

    void processor(std::function<int(Ct const * const, int,sizebounded<Vt,sz>&)>);

    virtual void push(int len, sizebounded<Vt,sz>&) const;
    virtual int pull(sizebounded<Vt,sz>&) const;

    virtual int process(Ct const * const, int len, sizebounded<Vt,sz>&) const;

  private:
    Ct *_config;
    stream *_src;
    stream *_tgt;
    std::function<int(Ct const * const, int,sizebounded<Vt,sz>&)> _proc;
};

