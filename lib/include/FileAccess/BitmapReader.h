#pragma once

#include <Arduino.h>
#include <FS.h>
#include <FreeRTOS.h>
#include <queue.h>
#include <semphr.h>
#include <task.h>

#include <memory>
#include <optional>
#include <string_view>
#include <vector>

#include "Bitmap.h"
#include "ConfigurationParser/configs/Bitmap.h"

class BitmapReader {
 public:
  enum class ReadResult {
    FileNotFound = 0,
    OK,
    InvalidBitmap,
  };

  BitmapReader(fs::FS& fs);

  std::optional<Bitmap::BitmapInfo> readBitmapMetadata(
      const std::string_view filePath, ReadResult* result);

  // ! delete[] MUST be called after data is done being accessed
  std::optional<uint8_t*> readBitmapData(const std::string_view filePath,
                                         Bitmap::BitmapInfo* info,
                                         ReadResult* result);

  /**
   * @brief Check if the bitmap(s) declared in the config exist on the
   * filesystem
   *
   * @return int16_t -1 if invalid, 1 if static, 1 to N if animated and can find
   * at least one validly named file
   */
  int16_t validBitmapCount(const Configuration::Bitmap&);

 private:
  Bitmap::BitmapInfo readBitmapMetadata(File& file);

  static uint16_t read16(File& file);
  static uint32_t read32(File& file);

  fs::FS& _fs;
};