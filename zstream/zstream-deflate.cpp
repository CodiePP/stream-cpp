
/*
 * compile: g++ -o zdeflate zstream-deflate.cpp -std=c++11 -g -I . -I ../ext/sizebounded -lz
 *
 *
 */

#include "stream-cpp/zstream.hpp"
#include "stream-cpp/zstream.ipp"

#include <iostream>
#include <zlib.h>

static constexpr int sz = 4096;

class configuration
{
  public:
    configuration() {}
    ~configuration() {}
};

stream<configuration,char,sz>* prepare_stream(int & ncomp, FILE *fout);

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

    int ncomp = 0;
    auto *zstream = prepare_stream(ncomp, fout);
    sizebounded<char,sz> d0;

    FILE *fin = fopen(argv[1], "rb");
    if (! fin) { exit(1); }

    int ntot = 0;
    while (true) {
        int nread = fread((char*)d0.ptr(), 1, 4096, fin);
        ntot += nread;
        zstream->push(nread, d0);
        if (feof(fin)) { break; }
    }
    fclose(fin);

    int nhave = ncomp;
    zstream->push(0, d0); // flush
    while (ncomp - nhave >= sz) {
        nhave = ncomp;
        zstream->push(0, d0); // flush
    }
    fclose(fout);

    std::cout << "in total read " << ntot << " bytes and compressed to " << ncomp << " bytes." << std::endl;

}

stream<configuration,char,sz>* prepare_stream(int & ncomp, FILE *fout)
{
    auto *sinfo = new stream<configuration,char,sz>(nullptr, nullptr);
    sinfo->processor([&ncomp,fout](configuration const * const _c, int _l, sizebounded<char,sz>& _b)->int {
            ncomp += _l;
            std::cout << " compressed = " << ncomp << " bytes." << std::endl;
            if (_l > 0) {
              int nw = fwrite((const char*)_b.ptr(), 1, _l, fout);
              std::cout << "     written " << nw << " bytes" << std::endl;
            }
            return _l;
        });
    auto *deflater = new deflatestream<configuration,char,sz>(nullptr, nullptr, sinfo);
    return deflater;
}
