
#include "stream.hpp"
#include "stream.ipp"

#include <iostream>

/*
 * compile: g++ -o test test.cpp -std=c++11 -g -I . -I ext/sizebounded
 *
 */

class configuration
{
  public:
    configuration() {}
    ~configuration() {}
    int nth { 2 };
};

template <typename Ct, typename Vt, int sz>
class dbstream : public stream<Ct,Vt,sz>
{
  public:
    dbstream(Ct *c, stream<Ct,Vt,sz> *s, stream<Ct,Vt,sz> *t)
      : stream<Ct,Vt,sz>(c,s,t) {}
    dbstream(stream<Ct,Vt,sz> *s, stream<Ct,Vt,sz> *t)
      : stream<Ct,Vt,sz>(s,t) {}
    virtual sizebounded<Vt,sz>& process(Ct const * const, sizebounded<Vt,sz> &b) const {
      b.transform([](int i, Vt v) -> Vt {
          return v * 2;
      });
      return b;
    }
};

template <typename Ct, typename Vt, int sz>
class chstream : public stream<Ct,Vt,sz>
{
  public:
    chstream(Ct *c, stream<Ct,Vt,sz> *s, stream<Ct,Vt,sz> *t)
      : stream<Ct,Vt,sz>(c,s,t) {}
    chstream(stream<Ct,Vt,sz> *s, stream<Ct,Vt,sz> *t)
      : stream<Ct,Vt,sz>(s,t) {}
    virtual sizebounded<Vt,sz>& process(Ct const * const, sizebounded<Vt,sz> &b) const {
      b.map([](int i, Vt v) {
          std::cout << i << ": " << v << std::endl;
      });
      return b;
    }
};

template <typename Ct, typename Vt, int sz>
class origstream : public stream<Ct,Vt,sz>
{
  public:
    origstream(Ct *c, stream<Ct,Vt,sz> *s, stream<Ct,Vt,sz> *t)
      : stream<Ct,Vt,sz>(c,s,t) {}
    origstream(stream<Ct,Vt,sz> *s, stream<Ct,Vt,sz> *t)
      : stream<Ct,Vt,sz>(s,t) {}
    virtual sizebounded<Vt,sz>& process(Ct const * const, sizebounded<Vt,sz> &b) const {
      b.transform([](int i, Vt v)->Vt {
          if (i < 10) {
            return 'a'+i;
          } else {
            return '7';
          }
      });
      return b;
    }
};

template <typename Ct, typename Vt, int sz>
class upperstream : public stream<Ct,Vt,sz>
{
  public:
    upperstream(Ct *c, stream<Ct,Vt,sz> *s, stream<Ct,Vt,sz> *t)
      : stream<Ct,Vt,sz>(c,s,t) {}
    upperstream(stream<Ct,Vt,sz> *s, stream<Ct,Vt,sz> *t)
      : stream<Ct,Vt,sz>(s,t) {}
    virtual sizebounded<Vt,sz>& process(Ct const * const c, sizebounded<Vt,sz> &b) const {
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
      return b;
    }
};

int main (int argc, char **argv)
{
  configuration config;

  // push to target
  constexpr int n = 21;
  sizebounded<int,n> d0;
  d0.transform([](int i, int v) -> int {
      return i;
  });
  chstream<configuration,int,n> s2(nullptr, nullptr);
  dbstream<configuration,int,n> s1(nullptr, &s2);
  s1.push(d0);

  // pull from a src
  constexpr int len = 12;
  origstream<configuration,char,len> s3(&config,nullptr,nullptr);
  upperstream<configuration,char,len> s4(&config,&s3,nullptr);
  chstream<configuration,char,len> s5(&config,&s4,nullptr);
  s5.pull();
}

