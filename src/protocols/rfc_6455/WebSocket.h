#ifndef WEBSOCKET_HEADER
#define WEBSOCKET_HEADER
#include <boost/archive/iterators/base64_from_binary.hpp>
#include <boost/archive/iterators/insert_linebreaks.hpp>
#include <boost/archive/iterators/ostream_iterator.hpp>
#include <boost/archive/iterators/transform_width.hpp>
#include <boost/json/string_view.hpp>
#include <boost/uuid/detail/sha1.hpp>
#include <cstdio>
#include <cstring>
#include <fcntl.h>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <vector>

namespace scribble {
namespace websocket {
// WebSocket - based on http://tools.ietf.org/html/rfc6455
class WebSocket {
public:
  WebSocket(uint64_t t_version, std::string t_channel, std::string t_guid);
  WebSocket(const std::string &t_data);
  ~WebSocket();
  const std::string GetHandshake();
  /**
   * @brief Decode data from a frame.
   * @param t_data Data to decode
   * @param t_decoded_data Buffer to write decoded data into
   * @param t_bytes_to_read Number of bytes to read from t_data
   * @return Number of bytes read from t_data
   */
  const size_t Decode(const std::string &t_data, std::string &t_decoded_data,
                      const size_t t_bytes_to_read);
  /**
   * @brief Encode data into a frame of specified size.
   * @param t_data Data to encode
   * @param t_encoded_data Buffer to write encoded data into
   * @param t_bytes_to_read Number of bytes to read from t_data
   * @return Number of bytes read from t_data
   */
  const size_t Encode(const std::string &t_data, std::string &t_encoded_data,
                      const size_t t_bytes_to_read);
  const uint64_t GetVersion();
  const std::string GetChannel();

private:
  void ParseVersion(const std::string &t_data);
  void ParseChannel(const std::string &t_data);
  void ParseGuid(const std::string &t_data);
  void AddDigest(std::stringstream &os);
  uint64_t m_version;
  std::string m_channel;
  std::string m_guid;
};
} // namespace websocket
} // namespace scribble
#endif
