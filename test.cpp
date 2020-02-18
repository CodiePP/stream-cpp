
#include "stream.hpp"
#include "stream.ipp"

#include <iostream>

/*
 * compile: g++ -o test test.cpp -std=c++11 -g -I .. -I ext
 *
 */

class configuration
{
  public:
    configuration() {}
    ~configuration() {}
    int nth { 2 };
};
class state
{
  public:
    state() {}
    ~state() {}
};

template <typename Ct, typename St, typename Vt, int sz>
class dbstream : public stream<Ct,St,Vt,sz>
{
  public:
    dbstream(Ct *c, St *st, stream<Ct,St,Vt,sz> *s, stream<Ct,St,Vt,sz> *t)
      : stream<Ct,St,Vt,sz>(c,st,s,t) {}
    dbstream(stream<Ct,St,Vt,sz> *s, stream<Ct,St,Vt,sz> *t)
      : dbstream<Ct,St,Vt,sz>(nullptr,nullptr,s,t) {}
    virtual int process(Ct const * const, St*, int blen, sizebounded<Vt,sz> &b) const {
      b.transform([](int i, Vt v) -> Vt {
          return v * 2;
      });
      return sz;
    }
};

template <typename Ct, typename St, typename Vt, int sz>
class chstream : public stream<Ct,St,Vt,sz>
{
  public:
    chstream(Ct *c, St *st, stream<Ct,St,Vt,sz> *s, stream<Ct,St,Vt,sz> *t)
      : stream<Ct,St,Vt,sz>(c,st,s,t) {}
    chstream(stream<Ct,St,Vt,sz> *s, stream<Ct,St,Vt,sz> *t)
      : chstream<Ct,St,Vt,sz>(nullptr,nullptr,s,t) {}
    virtual int process(Ct const * const, St*, int blen, sizebounded<Vt,sz> &b) const {
      b.map([](int i, Vt v) {
          std::cout << i << ": " << v << std::endl;
      });
      return sz;
    }
};

template <typename Ct, typename St, typename Vt, int sz>
class origstream : public stream<Ct,St,Vt,sz>
{
  public:
    origstream(Ct *c, St *st, stream<Ct,St,Vt,sz> *s, stream<Ct,St,Vt,sz> *t)
      : stream<Ct,St,Vt,sz>(c,st,s,t) {}
    origstream(stream<Ct,St,Vt,sz> *s, stream<Ct,St,Vt,sz> *t)
      : origstream<Ct,St,Vt,sz>(nullptr,nullptr,s,t) {}
    virtual int process(Ct const * const, St*, int blen, sizebounded<Vt,sz> &b) const {
      b.transform([](int i, Vt v)->Vt {
          if (i < 10) {
            return 'a'+i;
          } else {
            return '7';
          }
      });
      return sz;
    }
};

template <typename Ct, typename St, typename Vt, int sz>
class upperstream : public stream<Ct,St,Vt,sz>
{
  public:
    upperstream(Ct *c, St *st, stream<Ct,St,Vt,sz> *s, stream<Ct,St,Vt,sz> *t)
      : stream<Ct,St,Vt,sz>(c,st,s,t) {}
    upperstream(stream<Ct,St,Vt,sz> *s, stream<Ct,St,Vt,sz> *t)
      : upperstream<Ct,St,Vt,sz>(nullptr,nullptr,s,t) {}
    virtual int process(Ct const * const c, St *, int blen, sizebounded<Vt,sz> &b) const {
      b.transform([&c](int i, Vt v)->Vt {
          //c = nullptr;
          if (c && i % c->nth == 0) {
            if (v <= 'z' && v >= 'a') {
              return 'A' + v - 'a';
            } else {
              return v;
            }
          } else {
            return v;
          }
      });
      return sz;
    }
};

int main (int argc, char **argv)
{
  configuration config;
  state st;

  // push to target
  constexpr int n = 21;
  sizebounded<int,n> d0;
  d0.transform([](int i, int v) -> int {
      return i;
  });
  chstream<configuration,state,int,n> s2(nullptr, nullptr);
  dbstream<configuration,state,int,n> s1(nullptr, &s2);
  s1.push(d0.size(), d0);

  // pull from a src
  constexpr int len = 12;
  sizebounded<char,len> d1;
  origstream<configuration,state,char,len> s3(&config,&st,nullptr,nullptr);
  upperstream<configuration,state,char,len> s4(&config,&st,&s3,nullptr);
  chstream<configuration,state,char,len> s5(&config,&st,&s4,nullptr);
  int blen=s5.pull(d1);

  // push to target with inline function objects
  constexpr int num = 5;
  sizebounded<int,num> d2;
  d2.transform([num](int i, int v) -> int {
      return num-i;
  });
  stream<configuration,state,int,num> s7(nullptr, nullptr, nullptr, nullptr);
  s7.processor([](configuration const * const, state*, int, sizebounded<int,num>&b)->int
      { b.map([](int i, int v) {
          std::cout << i << ": " << v << std::endl;
        });
        return 0;
      });
  stream<configuration,state,int,num> s6(nullptr, nullptr, &s7);
  s6.processor([](configuration const * const, state*, int len, sizebounded<int,num>&b)->int
      { b.transform([](int i, int v) {
          return v*3;
        });
        return len;
      });
  s6.push(d2.size(), d2);
  return 0;
}
