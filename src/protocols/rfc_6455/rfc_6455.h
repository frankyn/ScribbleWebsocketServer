#include <cstdio>
#include <cstring>
#include <fcntl.h>
#include "./base64/base64.h"
#include "./sha1/sha1.h"
#include "../WSProtocol.h"

#ifndef RFC6455_HEADER
#define RFC6455_HEADER

// RFC6455 - based on http://tools.ietf.org/html/rfc6455

class RFC6455 : public WSProtocol{
  public:
    RFC6455();
    ~RFC6455();

    void packetLength(const std::string input, WSPacketLength *);
    unsigned long packetRealLength(const std::string);
    int packetComplete(const std::string input);
    int packetFragmented(const std::string input);
    int handshake(const std::string input, WSAttributes * response);
    int hasMask(const std::string input);
    std::string decode(const std::string input);
    std::string encode(const std::string input);
};

#endif
