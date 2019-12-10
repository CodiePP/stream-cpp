
#include "stream.hpp"
#include "stream.ipp"

#include <iostream>

/*
 * compile: g++ -o test test.cpp -std=c++11 -g -I . -I ext/sizebounded
 *
 */

template <typename T, int sz>
class dbstream : public stream<T,sz>
{
  public:
    dbstream(stream<T,sz> *s, stream<T,sz> *t) : stream<T,sz>(s,t) {}
    virtual sizebounded<T,sz>& process(sizebounded<T,sz> &b) const {
      b.transform([](int i, T v) -> T {
          return v * 2;
      });
      return b;
    }
};

template <typename T, int sz>
class chstream : public stream<T,sz>
{
  public:
    chstream(stream<T,sz> *s, stream<T,sz> *t) : stream<T,sz>(s,t) {}
    virtual sizebounded<T,sz>& process(sizebounded<T,sz> &b) const {
      b.map([](int i, T v) {
          std::cout << i << ": " << v << std::endl;
      });
      return b;
    }
};

template <typename T, int sz>
class origstream : public stream<T,sz>
{
  public:
    origstream(stream<T,sz> *s, stream<T,sz> *t) : stream<T,sz>(s,t) {}
    virtual sizebounded<T,sz>& process(sizebounded<T,sz> &b) const {
      b.transform([](int i, T v)->T {
          if (i < 10) {
            return 'a'+i;
          } else {
            return '7';
          }
      });
      return b;
    }
};

template <typename T, int sz>
class upperstream : public stream<T,sz>
{
  public:
    upperstream(stream<T,sz> *s, stream<T,sz> *t) : stream<T,sz>(s,t) {}
    virtual sizebounded<T,sz>& process(sizebounded<T,sz> &b) const {
      b.transform([](int i, T v)->T {
          if (v <= 'z' && v >= 'a') {
            return 'A' + v - 'a';
          } else {
            return v;
          }
      });
      return b;
    }
};

int main (int argc, char **argv)
{
  // push to target
  sizebounded<int,21> d0;
  d0.transform([](int i, int v) -> int {
      return i;
  });
  chstream<int,21> s2(nullptr, nullptr);
  dbstream<int,21> s1(nullptr, &s2);

  s1.push(d0);

  // pull from a src
  origstream<char,12> s3(nullptr,nullptr);
  upperstream<char,12> s4(&s3,nullptr);
  chstream<char,12> s5(&s4,nullptr);
  s5.pull();
}

