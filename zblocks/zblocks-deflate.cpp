
/*
 * compile: g++ -o zdeflate zblocks-deflate.cpp -std=c++11 -g -I ../.. -I ../ext -lz
 *
 *
 */

#include "stream-cpp/zblocks.hpp"
#include "stream-cpp/zblocks.ipp"

#include <iostream>
#include <zlib.h>

static constexpr int sz = 4096;

class zconf
{
  public:
    zconf() {}
    ~zconf() {}
};
class zstate
{
  public:
    zstate() {}
    ~zstate() {}
    int ndecmp() const { return _ndecmp; };
    void ndecmp(int n) { _ndecmp+=n; };
  private:
    int _ndecmp{0};
};

stream<zconf,zstate,char,sz>* prepare_stream(zstate & st, FILE *fout);

int main (int argc, char **argv)
{
    if (argc != 3) { exit(1); }
    std::cout << "compressing " << argv[1] << std::endl;
    std::cout << "to output " << argv[2] << std::endl;

    FILE *fout = fopen(argv[2], "wbx");
    if (! fout) {
        std::cout << "error opening '" << argv[2] << "' for output." << std::endl;
        exit(1);
    }

    zstate state;
    auto *zstream = prepare_stream(state, fout);
    sizebounded<char,sz> d0;

    FILE *fin = fopen(argv[1], "rb");
    if (! fin) { exit(1); }

    int ntot = 0;
    while (true) {
        int nread = fread((char*)d0.ptr(), 1, sz, fin);
        ntot += nread;
        zstream->push(nread, d0);
        if (feof(fin)) { break; }
    }
    fclose(fin);

    int nhave = state.ndecmp();
    zstream->push(0, d0); // flush
    while (state.ndecmp() - nhave >= sz) {
        nhave = state.ndecmp();
        zstream->push(0, d0); // flush
    }
    fclose(fout);

    std::cout << "in total read " << ntot << " bytes and compressed to " << state.ndecmp() << " bytes." << std::endl;

}

stream<zconf,zstate,char,sz>* prepare_stream(zstate & st, FILE *fout)
{
    auto *sinfo = new stream<zconf,zstate,char,sz>(nullptr, &st, nullptr, nullptr);
    sinfo->processor([fout](zconf const * const _c, zstate * _st, int _l, sizebounded<char,sz>& _b)->int {
            _st->ndecmp(_l);
            std::cout << " compressed = " << _st->ndecmp() << " bytes." << std::endl;
            if (_l > 0) {
              int nw = fwrite((const char*)_b.ptr(), 1, _l, fout);
              std::cout << "     written " << nw << " bytes" << std::endl;
            }
            return _l;
        });
    auto *deflater = new deflatestream<zconf,zstate,char,sz>(nullptr, sinfo);
    return deflater;
}
