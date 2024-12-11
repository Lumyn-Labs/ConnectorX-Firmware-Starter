#pragma once

#include <Arduino.h>

#include <map>
#include <optional>

#include "FileAccess/FileService.h"
#include "IDCreator/IDCreator.h"
#include "definitions/led/BitmapInstance.h"

class BitmapManager {
 public:
  BitmapManager(FileService**);

  uint16_t registerBitmap(const Configuration::Bitmap&);

  inline std::optional<const Bitmap::BitmapInstance*> getBitmap(
      uint16_t id) const {
    if (_bitmaps.contains(id)) {
      return &_bitmaps.at(id);
    }

    return std::nullopt;
  }

 private:
  std::map<uint16_t, Bitmap::BitmapInstance> _bitmaps;
  FileService** _fileService;
};