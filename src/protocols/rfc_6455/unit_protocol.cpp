#include "WebSocket.h"
#include <cassert>
#include <chrono>
#include <gtest/gtest.h>
#include <iostream>
#include <memory>
#include <string>

using scribble::websocket::WebSocket;
using std::invalid_argument;
using std::logic_error;
using std::ostream_iterator;
using std::string;
using std::stringstream;

TEST(WebSocket, InitializeWebSocket) {
  const string example_init =
      "GET /chat HTTP/1.1\r\nHost: server.example.com\r\nUpgrade: "
      "websocket\r\nConnection: Upgrade\r\nSec-WebSocket-Key: "
      "dGhlIHNhbXBsZSBub25jZQ==\r\nOrigin: "
      "http://example.com\r\nSec-WebSocket-Protocol: chat, "
      "superchat\r\nSec-WebSocket-Version: 13";
  const string example_handshake =
      "HTTP/1.1 101 Switching Protocols\r\nUpgrade: websocket\r\nConnection: "
      "Upgrade\r\nSec-WebSocket-Accept: s3pPLMBiTxaQ9kYGzzhZRbK+xOo=\r\n\r\n";
  WebSocket websocket(example_init);

  EXPECT_EQ(websocket.GetVersion(), 13);
  EXPECT_EQ(websocket.GetChannel(), "chat");
  EXPECT_EQ(websocket.GetHandshake(), example_handshake);
}

TEST(WebSocket, FailBadVersionWebSocket) {
  const string example_init =
      "GET /chat HTTP/1.1\r\nHost: server.example.com\r\nUpgrade: "
      "websocket\r\nConnection: Upgrade\r\nSec-WebSocket-Key: "
      "dGhlIHNhbXBsZSBub25jZQ==\r\nOrigin: "
      "http://example.com\r\nSec-WebSocket-Protocol: chat, "
      "superchat\r\nSec-WebSocket-Version: 15";
  const string example_handshake =
      "HTTP/1.1 101 Switching Protocols\r\nUpgrade: websocket\r\nConnection: "
      "Upgrade\r\nSec-WebSocket-Accept: s3pPLMBiTxaQ9kYGzzhZRbK+xOo=\r\n\r\n";
  EXPECT_THROW({ WebSocket websocket(example_init); }, invalid_argument);
}

TEST(WebSocket, SingleFrameSmallMessage) {
  const string example_wsmessage_small_encoded("\x81\x05\x48\x65\x6c\x6c\x6f");
  const string example_wsmessage_small_payload("Hello");
  WebSocket websocket(13, "chat", "guid");
  string decoded_message;
  size_t bytes_decoded =
      websocket.Decode(example_wsmessage_small_encoded, decoded_message,
                       /* Unimplemented */ 0);
  EXPECT_EQ(bytes_decoded, example_wsmessage_small_encoded.length());
  EXPECT_EQ(decoded_message, example_wsmessage_small_payload);

  string encoded_message;
  size_t bytes_encoded = websocket.Encode(
      example_wsmessage_small_payload, encoded_message, /* Unimplemented */ 0);
  EXPECT_EQ(bytes_encoded, example_wsmessage_small_payload.length());
  EXPECT_EQ(encoded_message, example_wsmessage_small_encoded);
}

TEST(WebSocket, SingleFrameLargeMessage) {
  const string example_wsmessage_large_encoded(
      "\x81\x7E\x5\xF"
      "data:image/"
      "webp;base64,"
      "UklGRrIDAABXRUJQVlA4IKYDAADwLwCdASosAZYAPm00k0emJKKhqNjpgMANiWdu2Tp7K8pV"
      "2noyyEzH6nD38iSAdaL9oMcEzHB8K9QAn7usMTDdUeE6EugYSm0wDRLIU3xLbexLLZceavkT"
      "uQclrdzGik0SNhHNH83brHV3INd08GfHXVhhX+D47ivsSs0iGx8G0Py6gAvFCS4gw/"
      "V1BHYc9XkbXleeBxeLOIe6xolDtv4tOsAYVMsDqwPG4pg3PX9sOjZSMp6x0qIAyyQ7lfqF6V"
      "driPTl7UvH3lf/ol40wCnnWvrV7/"
      "lsLWe1VlqYbGkOX9OBIn354uFZpmQTPooMXRFZPuyjhmqUySbSgERm/"
      "qTSvMVVfG4ufWeUixl/LsXE/aZbE9PCJXeaxIEVXuYBCRcG2N5/"
      "Lk2I06Iwpd4ZRDpmQRCkOxn9RArxHH06npAbJW99XWFr5zYUmbPUKw+"
      "KYSsxD1TAIl4z34PxFeFhykvfsI+i3OED/+bNf/"
      "QKyf4o7bsQFlflPrhrwgBu7u92n4NWiuz7oFVpIAD+ykQk+nme47V7U8bq9xFC+"
      "Dh63c7KvTyu7rkuvMOVIV+J59ls8oNvUBdBmDv+Mk8iM9YX73uhljeEPU+"
      "vSWA5h6JvqHbK99TSrGTTKrsFjl0kdJksoC1Gzm6Tms3Vb4izqFc/"
      "0CPP3pRALZxJgTUtwbxdVZTY1anuVBMWcqEfBJrl7QmkBdw8Qty4YVIatc8jn7SnF5sWkJ5E"
      "knBVilEM297vyjlEaBvKwx7D58sMHJntaB9eGK57vopTBwWsw0XLLdHsBlst/"
      "v3aST1NhcTEpKD1NzeEOc6xMbOAZvx7vHcRYsIMAudkXJtSTZydQxxJJfAkp8De34fH5S2tz"
      "eLyc6AsdJAN7iyuuukqJgrwKx5F52vmDqhxDxZbuHG4Qr0Cxsyx/"
      "R4Y1P86HxuQqkvAQalafGxqZrxhJ3vbGmmp4PXqmBVVBYdeJ0VzmstYcKP3RO3dpqe94YdQk"
      "qwjuSXFndhRjY2evUJcpfYn+ypx+"
      "elSVUtSklSCFRmJT5KvduKn5U3RyH6Ofj07krcS3s2nK/SIqqNkeaaf+qVPR/"
      "e+idfkVgA+G0lOyGDXqqpatT898AC/dVCmqrKyMajwyXO/"
      "wSGxbX6th0Ch7YBfS8oJDk0IuBw4ectwxMenNE+"
      "qIQZBvnYP4dH4x036uaOD1Qldz99ZHQc4XJiUojpj15jcmu/StkZoq97OGS5Az221t6/"
      "bDHnhdvMgebl0LgAA",
      1299);
  const string example_wsmessage_large_payload(
      "data:image/"
      "webp;base64,"
      "UklGRrIDAABXRUJQVlA4IKYDAADwLwCdASosAZYAPm00k0emJKKhqNjpgMANiWdu2Tp7K8pV"
      "2noyyEzH6nD38iSAdaL9oMcEzHB8K9QAn7usMTDdUeE6EugYSm0wDRLIU3xLbexLLZceavkT"
      "uQclrdzGik0SNhHNH83brHV3INd08GfHXVhhX+D47ivsSs0iGx8G0Py6gAvFCS4gw/"
      "V1BHYc9XkbXleeBxeLOIe6xolDtv4tOsAYVMsDqwPG4pg3PX9sOjZSMp6x0qIAyyQ7lfqF6V"
      "driPTl7UvH3lf/ol40wCnnWvrV7/"
      "lsLWe1VlqYbGkOX9OBIn354uFZpmQTPooMXRFZPuyjhmqUySbSgERm/"
      "qTSvMVVfG4ufWeUixl/LsXE/aZbE9PCJXeaxIEVXuYBCRcG2N5/"
      "Lk2I06Iwpd4ZRDpmQRCkOxn9RArxHH06npAbJW99XWFr5zYUmbPUKw+"
      "KYSsxD1TAIl4z34PxFeFhykvfsI+i3OED/+bNf/"
      "QKyf4o7bsQFlflPrhrwgBu7u92n4NWiuz7oFVpIAD+ykQk+nme47V7U8bq9xFC+"
      "Dh63c7KvTyu7rkuvMOVIV+J59ls8oNvUBdBmDv+Mk8iM9YX73uhljeEPU+"
      "vSWA5h6JvqHbK99TSrGTTKrsFjl0kdJksoC1Gzm6Tms3Vb4izqFc/"
      "0CPP3pRALZxJgTUtwbxdVZTY1anuVBMWcqEfBJrl7QmkBdw8Qty4YVIatc8jn7SnF5sWkJ5E"
      "knBVilEM297vyjlEaBvKwx7D58sMHJntaB9eGK57vopTBwWsw0XLLdHsBlst/"
      "v3aST1NhcTEpKD1NzeEOc6xMbOAZvx7vHcRYsIMAudkXJtSTZydQxxJJfAkp8De34fH5S2tz"
      "eLyc6AsdJAN7iyuuukqJgrwKx5F52vmDqhxDxZbuHG4Qr0Cxsyx/"
      "R4Y1P86HxuQqkvAQalafGxqZrxhJ3vbGmmp4PXqmBVVBYdeJ0VzmstYcKP3RO3dpqe94YdQk"
      "qwjuSXFndhRjY2evUJcpfYn+ypx+"
      "elSVUtSklSCFRmJT5KvduKn5U3RyH6Ofj07krcS3s2nK/SIqqNkeaaf+qVPR/"
      "e+idfkVgA+G0lOyGDXqqpatT898AC/dVCmqrKyMajwyXO/"
      "wSGxbX6th0Ch7YBfS8oJDk0IuBw4ectwxMenNE+"
      "qIQZBvnYP4dH4x036uaOD1Qldz99ZHQc4XJiUojpj15jcmu/StkZoq97OGS5Az221t6/"
      "bDHnhdvMgebl0LgAA",
      1295);
  WebSocket websocket(13, "chat", "guid");
  string decoded_message;
  size_t bytes_decoded = websocket.Decode(
      example_wsmessage_large_encoded, decoded_message, /* Unimplemented */ 0);
  EXPECT_EQ(bytes_decoded, example_wsmessage_large_encoded.length());
  EXPECT_EQ(decoded_message, example_wsmessage_large_payload);
  string encoded_message;
  size_t bytes_encoded = websocket.Encode(
      example_wsmessage_large_payload, encoded_message, /* Unimplemented */ 0);
  EXPECT_EQ(bytes_encoded, example_wsmessage_large_payload.length());
  EXPECT_EQ(encoded_message, example_wsmessage_large_encoded);
}

TEST(WebSocket, MultiFrameSmallMessage) {
  const string example_four_framed_messages_encoded =
      "\x01\x02\x48\x65\x01\x01\x6c\x01\x01\x6c\x81\x01\x6f";
  const string example_framed_message_payload = "Hello";
  WebSocket websocket(13, "chat", "guid");
  string decoded_message;
  size_t bytes_decoded =
      websocket.Decode(example_four_framed_messages_encoded, decoded_message,
                       /* Unimplemented */ 0);
  EXPECT_EQ(decoded_message, example_framed_message_payload);
  EXPECT_EQ(bytes_decoded, example_four_framed_messages_encoded.length());
}

TEST(WebSocket, IncompleteSingleFrame) {
  const string example_incomplete_frame_message_encoded = "\x01\x02\x48";
  const string example_incomplete_frame_message_final_encoded = "\x65";
  const string example_framed_message_payload = "He";
  WebSocket websocket(13, "chat", "guid");
  string decoded_message;
  size_t bytes_decoded = websocket.Decode(
      example_incomplete_frame_message_encoded, decoded_message,
      /* Unimplemented */ 0);
  bytes_decoded += websocket.Decode(
      example_incomplete_frame_message_final_encoded, decoded_message,
      /* Unimplemented */ 0);
  EXPECT_EQ(decoded_message, example_framed_message_payload);
  EXPECT_EQ(bytes_decoded,
            example_incomplete_frame_message_encoded.length() +
                example_incomplete_frame_message_final_encoded.length());
}

TEST(WebSocket, IncompleteMultiFrame) {
  const string incomplete_frame_message_encoded = "\x01\x02\x48\x65\x01";
  const string incomplete_frame_message_final_encoded =
      "\x01\x01\x6c\x01\x01\x6c\x81\x01\x6f";
  const string framed_message_payload = "Hello";
  WebSocket websocket(13, "chat", "guid");
  string decoded_message;

  std::chrono::steady_clock::time_point begin =
      std::chrono::steady_clock::now();
  size_t bytes_decoded =
      websocket.Decode(incomplete_frame_message_encoded, decoded_message,
                       /* Unimplemented */ 0);
  std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
  EXPECT_LE(
      std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin).count(),
      500);

  EXPECT_EQ(bytes_decoded, incomplete_frame_message_encoded.length() - 1);
  bytes_decoded +=
      websocket.Decode(incomplete_frame_message_final_encoded, decoded_message,
                       /* Unimplemented */ 0);
  EXPECT_EQ(decoded_message, framed_message_payload);
  EXPECT_EQ(bytes_decoded, incomplete_frame_message_encoded.length() +
                               incomplete_frame_message_final_encoded.length() -
                               1);
}

// TEST(WebSocket, DecodeOneMessageAtATime) {
//   const string two_messages_multi_frame_encoded =
//       "\x01\x02\x48\x65\x01\x01\x6c\x01\x01\x6c\x81\x01\x6f\x01\x02\x48\x65\x01"
//       "\x01\x6c\x01\x01\x6c\x81\x01\x6f";
//   const string example_framed_message_payload = "Hello";
//   WebSocket websocket(13, "chat", "guid");
//   string decoded_message;
//   size_t bytes_decoded =
//       websocket.Decode(two_messages_multi_frame_encoded, decoded_message,
//                        /* Unimplemented */ 0);
//   EXPECT_EQ(decoded_message, example_framed_message_payload);
//   EXPECT_EQ(bytes_decoded, two_messages_multi_frame_encoded.length() / 2);
// }