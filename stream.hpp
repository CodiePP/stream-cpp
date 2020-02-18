#pragma once

#include <functional>

#include "sizebounded/sizebounded.hpp"

template <typename Ct, typename St, typename Vt, int sz>
class stream
{
  public:
    stream(St *, stream *src, stream *tgt);
    stream(Ct const * const config, St *, stream *src, stream *tgt);
    virtual ~stream();

    void processor(std::function<int(Ct const * const, St *, int,sizebounded<Vt,sz>&)>);

    virtual void push(int len, sizebounded<Vt,sz>&) const;
    virtual int pull(sizebounded<Vt,sz>&) const;

    virtual int process(Ct const * const, St *, int len, sizebounded<Vt,sz>&) const;

  private:
    Ct const * _config;
    St *_state;
    stream *_src;
    stream *_tgt;
    std::function<int(Ct const * const, St *, int,sizebounded<Vt,sz>&)> _proc;
};

