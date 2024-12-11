#pragma once

#include <cinttypes>

#include "FileType.h"

namespace Files {
struct __attribute__((packed)) FileTransferInfoHeader {
  char path[32];
};

struct __attribute__((packed)) SendConfigInfoHeader {};

struct __attribute__((packed)) SetZonePixelBuffer {
  uint16_t zoneId;
  // Added for extra safety; the size MUST be 3 * zoneLength (r, g, b)
  uint16_t zoneLength;
};

struct __attribute__((packed)) FilesHeader {
  FileType type;
  union {
    FileTransferInfoHeader fileTransfer;
    SendConfigInfoHeader sendConfig;
    SetZonePixelBuffer setZoneBuffer;
  };
  uint16_t md5;
  uint32_t fileSize;
};

struct __attribute__((packed)) Files {
  FilesHeader header;
  uint8_t* bytes;
};
}  // namespace Files