#pragma once

#include <FastLED_NeoMatrix.h>

#include "FileAccess/FileService.h"
#include "LedService/Zone.h"
#include "lumyn/domain/command/led/LEDCommand.h"

namespace Command = lumyn::internal::Command;
#include "definitions/led/BitmapInstance.h"

class ZoneMatrix : public Zone {
 public:
  ZoneMatrix(const Configuration::Zone&, CRGB*);

  bool update() override;

  void setBitmap(const Bitmap::BitmapInstance* instance,
                 std::optional<Configuration::ActionColor> color,
                 bool oneShot = false);

  void setText(const Command::LED::SetMatrixTextData data,
               Configuration::ActionColor color, const char* text);

 protected:
  bool incrementState();
  std::optional<uint8_t*> readBitmapData(
      const std::string&, FileService::QueuedBitmapResponse* response);
  void drawBitmap(uint8_t* data, FileService::QueuedBitmapResponse* response);
  std::string currentBitmapFileName() const;

  const Bitmap::BitmapInstance* _curBitmap;
  std::optional<Command::LED::SetMatrixTextData> _curText;
  std::unique_ptr<FastLED_NeoMatrix> _matrix;
  FileService** _fileService = &FileAccessService;
  QueueHandle_t _bmpRecQueue;
};