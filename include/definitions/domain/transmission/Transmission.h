#pragma once

#include <memory>
#include <vector>

#include "definitions/domain/transmission/TransmissionType.h"

namespace Transmission {

struct __attribute__((packed)) TransmissionHeader {
  TransmissionType type;
  uint32_t dataLength;
  uint16_t packetCount;
};

struct TransmissionBody {
  // ! MUST CALL DELETE WHEN RECEIVING FROM A QUEUE
  uint8_t* data;
};

struct Transmission {
  TransmissionHeader header;
  TransmissionBody body;
};

struct PacketHeader {
  uint16_t packetId;
  // Length of the buf
  uint8_t length;
  // TODO
  uint8_t crc;
};

constexpr uint16_t kMaxPacketSize = 256;
constexpr uint16_t kMaxPacketBodySize = kMaxPacketSize - sizeof(PacketHeader);

struct Packet {
  PacketHeader header;
  uint8_t buf[kMaxPacketBodySize];
};
}  // namespace Transmission