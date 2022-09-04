#include <boost/archive/iterators/base64_from_binary.hpp>
#include <boost/archive/iterators/insert_linebreaks.hpp>
#include <boost/archive/iterators/ostream_iterator.hpp>
#include <boost/archive/iterators/transform_width.hpp>
#include <boost/uuid/detail/sha1.hpp>
#include <cstdio>
#include <cstring>
#include <fcntl.h>
#include <sstream>

#ifndef WEBSOCKET_HEADER
#define WEBSOCKET_HEADER

/*
    WEBSOCKET - based on http://tools.ietf.org/html/rfc6455
*/

struct WSAttributes {
  std::string version;
  std::string channel;
  std::string response;
};

struct WSPacketLength {
  unsigned long packetLen;
  unsigned long length;
  unsigned payloadOffset;
};

class WebSocket {
public:
  WebSocket();

  ~WebSocket();

  void packetLength(const std::string &data, WSPacketLength &ws_packet_length);

  unsigned long packetRealLength(const std::string &data);

  int packetComplete(const std::string &data);

  int packetFragmented(const std::string &data);

  int handshake(const std::string &data, WSAttributes &response);

  int hasMask(const std::string &data);

  std::string decode(const std::string &data);

  std::string encode(const std::string &data);
};

#endif
