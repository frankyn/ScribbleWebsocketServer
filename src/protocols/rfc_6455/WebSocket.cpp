#include "WebSocket.h"

using namespace std;
using namespace boost::uuids::detail;
using namespace boost::archive::iterators;

WebSocket::WebSocket() {}

WebSocket::~WebSocket() {}

int WebSocket::handshake(const string &input, WSAttributes &response) {
  /*Grab Version*/
  if (input.compare("") == 0)
    return -1;
  string versionLookupStart = "Sec-WebSocket-Version: ";
  string versionLookupEnd = "\r\n";
  int verStart = input.find(versionLookupStart);
  int verEnd = input.find(versionLookupEnd, verStart);
  response.version =
      input.substr(verStart + versionLookupStart.length(),
                   verEnd - (verStart + versionLookupStart.length()));
  if (response.version.compare("13") != 0)
    return -1;
  /*Grab Channel Name*/
  string channelLookupStart = "GET /";
  string channelLookupEnd = " HTTP/1.1";
  int channelStart = input.find(channelLookupStart);
  int channelEnd = input.find(channelLookupEnd, channelStart);
  response.channel =
      input.substr(channelStart + channelLookupStart.length(),
                   channelEnd - (channelStart + channelLookupStart.length()));
  /*Create Server Response*/
  stringstream os;
  string GUID = "258EAFA5-E914-47DA-95CA-C5AB0DC85B11";
  os << "HTTP/1.1 101 Switching Protocols\r\n";
  os << "Upgrade: websocket\r\n";
  os << "Connection: Upgrade\r\n";
  os << "Sec-WebSocket-Accept: ";
  string keyLookupStart = "Sec-WebSocket-Key: ";
  string keyLookupEnd = "\r\n";
  int keyStart = input.find(keyLookupStart);
  int keyEnd = input.find(keyLookupEnd, keyStart);
  string inputKey =
      input.substr(keyStart + keyLookupStart.length(),
                   keyEnd - (keyStart + keyLookupStart.length())) +
      GUID;
  unsigned int digest[5];
  unsigned char longDigest[20];
  sha1 sha1;
  sha1.process_bytes(inputKey.c_str(), inputKey.length());
  sha1.get_digest(digest);
  for (int i = 0, j = 0; i < 5; i++) {
    longDigest[j++] = (digest[i] & 0xFF000000) >> 24;
    longDigest[j++] = (digest[i] & 0x00FF0000) >> 16;
    longDigest[j++] = (digest[i] & 0x0000FF00) >> 8;
    longDigest[j++] = (digest[i] & 0x000000FF);
  }
  typedef base64_from_binary<transform_width<const char *, 6, 8>> base64_text;
  copy(base64_text(longDigest), base64_text(longDigest + 20),
       std::ostream_iterator<char>(os));
  os << '=';
  os << "\r\n\r\n";
  response.response = os.str();

  return 0;
}

/*
        Check if the packet has the MASK bit turned on.
*/
int WebSocket::hasMask(const string &data) {
  if (data.empty())
    return 0;
  return (data[1] & 0x80);
}

/*
        Check if the packet is fragmented state
*/

int WebSocket::packetFragmented(const string &data) {
  if (data.empty())
    return 0;
  return !(data[0] & 0x80);
}

/*
        Check if the packet is complete.
*/

int WebSocket::packetComplete(const string &input) {
  if (input.empty())
    return 0;
  std::string tmp = input;
  int completed = 0;
  // Using a loop instead of using recursion.
  unsigned long len = packetRealLength(tmp);
  if (len != 0 && len <= input.size()) {
    completed = 1;
  }

  return completed;
}

/*
        Get full size of packet even if it's fragged.
*/

unsigned long WebSocket::packetRealLength(const string &data) {
  if (data.empty())
    return 0;
  std::string tmp = data;
  int foundEndPacket = 0;
  unsigned long sizeOfPacket = 0;
  // Using a loop instead of using recursion.
  while (!tmp.empty() && !foundEndPacket) {
    if (packetFragmented(tmp)) {
      // Packet is a partial let's check to make sure all the packets are here
      // before we allow decoding.
      WSPacketLength pcktLen;
      packetLength(tmp, pcktLen);
      sizeOfPacket += pcktLen.packetLen;
      if (sizeOfPacket > data.size()) {
        // Incomplete packet
        return 0;
      }
      tmp = tmp.substr(pcktLen.packetLen, tmp.size());

    } else {
      WSPacketLength pcktLen;
      packetLength(tmp, pcktLen);
      sizeOfPacket += pcktLen.packetLen;
      if (sizeOfPacket > data.size()) {
        // Incomplete packet
        return 0;
      }
      foundEndPacket = 1;
    }
  }
  return sizeOfPacket;
}

/*
        Check packet length:
                There are 3 different cases for large packet sizes.
*/
void WebSocket::packetLength(const string &data, WSPacketLength &pcktLen) {
  pcktLen.length = 0;
  pcktLen.payloadOffset = 0;
  pcktLen.packetLen = 0;

  if (data.empty()) {
    return;
  }
  /*
        0                   1                   2                   3
0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
+-+-+-+-+-------+-+-------------+-------------------------------+
|F|R|R|R| opcode|M| Payload len |    Extended payload length    |
|I|S|S|S|  (4)  |A|     (7)     |             (16/64)           |
|N|V|V|V|       |S|             |   (if payload len==126/127)   |
| |1|2|3|       |K|             |                               |
+-+-+-+-+-------+-+-------------+ - - - - - - - - - - - - - - - +
|     Extended payload length continued, if payload len == 127  |
+ - - - - - - - - - - - - - - - +-------------------------------+
|                               |Masking-key, if MASK set to 1  |
+-------------------------------+-------------------------------+
| Masking-key (continued)       |          Payload Data         |
+-------------------------------- - - - - - - - - - - - - - - - +
:                     Payload Data continued ...                :
+ - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - +
|                     Payload Data continued ...                |
+---------------------------------------------------------------+
  */
  const unsigned char *inputBytes = (unsigned char *)data.c_str();
  pcktLen.length = inputBytes[1] & 0x7F;

  if (pcktLen.length == 126) {
    // 126 - 2 bytes
    // std::cout<<"Ver: 126 Size"<<std::endl;
    pcktLen.length = inputBytes[3] | inputBytes[2] << 8;
    pcktLen.payloadOffset = 4;
    pcktLen.packetLen =
        pcktLen.payloadOffset + (hasMask(data) ? 4 : 0) + pcktLen.length;
  } else if (pcktLen.length == 127) {
    // 127 - 8 bytes
    // NOT TESTED || Need a test packet.
    // std::cout<<"Ver: 127 Size"<<std::endl;
    pcktLen.length = inputBytes[2] << 24 | inputBytes[3] << 16 |
                     inputBytes[4] << 8 | inputBytes[5];
    pcktLen.payloadOffset = 6;
    pcktLen.packetLen =
        pcktLen.payloadOffset + (hasMask(data) ? 4 : 0) + pcktLen.length;
  } else {
    // std::cout<<"Ver: 125 Size"<<std::endl;
    pcktLen.payloadOffset = 2;
    pcktLen.packetLen =
        pcktLen.payloadOffset + (hasMask(data) ? 4 : 0) + pcktLen.length;
  }
}

string WebSocket::decode(const string &data) {
  string decodedInput = "";
  /*

     A single-frame unmasked text message

   0x81 0x05 0x48 0x65 0x6c 0x6c 0x6f (contains "Hello")

  A single-frame masked text message

   0x81 0x85 0x37 0xfa 0x21 0x3d 0x7f 0x9f 0x4d 0x51 0x58
   (contains "Hello")

  */
  // If packet isn't complete fail decoding.
  if (!packetComplete(data))
    return decodedInput;
  std::string unknownPacket = data;

  do {
    // std::cout << "LOOP" << std::endl;
    const char *inputBytes = unknownPacket.c_str();
    WSPacketLength pcktLen;
    packetLength(unknownPacket, pcktLen);

    // std::cout << "STARTING DECODING" << std::endl;
    // std::cout << "INPUT LENGTH: " << input.size() << std::endl;
    // std::cout << "LENGTH: " << pcktLen.packetLen << std::endl;
    // std::cout << "LENGTH: " << input.size()  << std::endl;
    if (hasMask(unknownPacket)) {
      // std::cout<<"MASKED"<<std::endl;
      // Masked Input
      unsigned long byteCounter = 0;
      unsigned long maskCounter = 0;
      unsigned long maskOffset = pcktLen.payloadOffset;
      unsigned long offset = 4 + pcktLen.payloadOffset;
      // printf ( "---------%X\n", inputBytes[offset] );
      while (byteCounter < pcktLen.length) {
        // std::cout << (byteCounter);
        // printf ( "--0x%X--" , inputBytes[byteCounter+offset] );
        // std::cout << " " << input.at ( byteCounter ) << " ";
        // std::cout << " " << (unsigned long)(inputBytes[byteCounter+offset] ^
        // inputBytes[(maskCounter)+maskOffset]) << std::endl;
        decodedInput += inputBytes[byteCounter + offset] ^
                        inputBytes[((maskCounter++) % 4) + maskOffset];
        byteCounter++;
      }
    } else {
      // Not Masked
      // std::cout<<"NOT MASKED"<<std::endl;
      unsigned long byteCounter = 0;
      int offset = pcktLen.payloadOffset;
      while (byteCounter < pcktLen.length) {
        decodedInput += inputBytes[byteCounter + offset];
        byteCounter++;
      }
    }
    // std::cout << "ENDING DECODING" << std::endl;
    // std::cout << "Length of unknownPacket: " << unknownPacket.size() <<
    // std::endl; std::cout << "REMOVING LENGTH: " << pcktLen.packetLen <<
    // std::endl;
    unknownPacket = unknownPacket.substr(pcktLen.packetLen);
    // std::cout << "AFTER SUBSTR" << std::endl;
  } while (!unknownPacket.empty());
  return decodedInput;
}

std::string WebSocket::encode(const string &data) {
  std::string encodedInput;

  int len = data.length();
  // std::cout<<"len: "<< len << std::endl;

  encodedInput += "\x81";

  if (len <= 125) {
    encodedInput += (char)(len);
  } else if (len <= 65535) {
    encodedInput += (char)(len >> 8);
    encodedInput += (char)(len & 0xff);
  } else {
    // NEEDS TO BE EXTENDED for largest possibility
    encodedInput += "??????";
  }

  encodedInput += data;

  return encodedInput;
}
