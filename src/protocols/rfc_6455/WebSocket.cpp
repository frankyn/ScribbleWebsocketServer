#include "WebSocket.h"

namespace scribble {
namespace websocket {
using boost::archive::iterators::base64_from_binary;
using boost::archive::iterators::transform_width;
using boost::json::string_view;
using boost::uuids::detail::sha1;
using std::cout;
using std::invalid_argument;
using std::ostream_iterator;
using std::shared_ptr;
using std::string;
using std::stringstream;

WebSocket::WebSocket(uint64_t t_version, string t_channel, string t_guid)
    : m_version(t_version), m_channel(t_channel), m_guid(t_guid) {
  if (m_version != 13) {
    throw invalid_argument("WebSocket version 13 is the only supported");
  }
}

WebSocket::WebSocket(const string &t_data) {
  ParseVersion(t_data);
  ParseChannel(t_data);
  ParseGuid(t_data);
  if (m_version != 13) {
    throw invalid_argument("WebSocket version 13 is the only supported");
  }
}

WebSocket::~WebSocket() {}

const uint64_t WebSocket::GetVersion() { return m_version; }

const std::string WebSocket::GetChannel() { return m_channel; }

void WebSocket::ParseChannel(const string &t_data) {
  /*Grab Channel Name*/
  string channelLookupStart = "GET /";
  string channelLookupEnd = " HTTP/1.1";
  int channelStart = t_data.find(channelLookupStart);
  int channelEnd = t_data.find(channelLookupEnd, channelStart);
  m_channel =
      t_data.substr(channelStart + channelLookupStart.length(),
                    channelEnd - (channelStart + channelLookupStart.length()));
}

void WebSocket::ParseVersion(const string &t_data) {
  const string versionLookupStart = "Sec-WebSocket-Version: ";
  const string versionLookupEnd = "\r\n";
  const int verStart = t_data.find(versionLookupStart);
  const int verEnd = t_data.find(versionLookupEnd, verStart);
  m_version =
      ::atol(t_data
                 .substr(verStart + versionLookupStart.length(),
                         verEnd - (verStart + versionLookupStart.length()))
                 .c_str());
}

void WebSocket::ParseGuid(const string &t_data) {
  const string GUID = "258EAFA5-E914-47DA-95CA-C5AB0DC85B11";
  string keyLookupStart = "Sec-WebSocket-Key: ";
  string keyLookupEnd = "\r\n";
  int keyStart = t_data.find(keyLookupStart);
  int keyEnd = t_data.find(keyLookupEnd, keyStart);
  m_guid = t_data.substr(keyStart + keyLookupStart.length(),
                         keyEnd - (keyStart + keyLookupStart.length())) +
           GUID;
}

void WebSocket::AddDigest(stringstream &os) {
  unsigned int digest[5];
  unsigned char longDigest[20];
  sha1 sha1;
  sha1.process_bytes(m_guid.c_str(), m_guid.length());
  sha1.get_digest(digest);
  for (int i = 0, j = 0; i < 5; i++) {
    longDigest[j++] = (digest[i] & 0xFF000000) >> 24;
    longDigest[j++] = (digest[i] & 0x00FF0000) >> 16;
    longDigest[j++] = (digest[i] & 0x0000FF00) >> 8;
    longDigest[j++] = (digest[i] & 0x000000FF);
  }
  typedef base64_from_binary<transform_width<const char *, 6, 8>> base64_text;
  copy(base64_text(longDigest), base64_text(longDigest + 20),
       ostream_iterator<char>(os));
}

const std::string WebSocket::GetHandshake() {
  stringstream os;
  os << "HTTP/1.1 101 Switching Protocols\r\n";
  os << "Upgrade: websocket\r\n";
  os << "Connection: Upgrade\r\n";
  os << "Sec-WebSocket-Accept: ";
  AddDigest(os);
  os << "=\r\n\r\n";
  return os.str();
}

const size_t WebSocket::Decode(const string &t_data, string &t_decoded_data,
                               const size_t t_bytes_to_read) {
  // TODO: Handle multi frame messages
  if (t_data.empty() || t_data.size() < 2) {
    return 0;
  }
  bool complete = t_data[0] & 0x80;
  size_t payload_length = 0;
  size_t payload_offset = 2;
  if (t_data[1] == 0x7E) {
    payload_length = t_data[2] << 8 | t_data[3];
    payload_offset += 2;
  } else if (t_data[1] == 0x7F) {
    payload_length =
        t_data[2] << 24 | t_data[3] << 16 | t_data[4] << 8 | t_data[5];
    payload_offset += 4;
  } else {
    payload_length = t_data[1] & 0x7D;
  }
  // TODO: add mask length to payload_offset
  t_decoded_data = t_data.substr(payload_offset, payload_length);
  return payload_offset + payload_length;
}

const size_t WebSocket::Encode(const string &t_data, string &t_encoded_data,
                               const size_t t_bytes_to_read) {
  // TODO: add support for t_bytes_to_read
  stringstream encoded_output;
  encoded_output << (uint8_t)0x81;
  const size_t data_len = t_data.length();
  if (data_len <= 0x7D) {
    encoded_output << (uint8_t)(data_len);
  } else if (data_len <= 0xFFFF) {
    encoded_output << (uint8_t)0x7E;
    encoded_output << (uint8_t)((data_len >> 8) & 0xFF);
    encoded_output << (uint8_t)((data_len & 0xFF));
  } else {
    encoded_output << (uint8_t)0x7F;
    encoded_output << (uint8_t)((data_len >> 24) & 0xFE);
    encoded_output << (uint8_t)((data_len >> 16) & 0xFF);
    encoded_output << (uint8_t)((data_len >> 8) & 0xFF);
    encoded_output << (uint8_t)((data_len & 0xFF));
  }
  encoded_output << t_data;
  t_encoded_data = encoded_output.str();
  return t_data.length();
}
} // namespace websocket
} // namespace scribble