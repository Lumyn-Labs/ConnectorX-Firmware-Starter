#pragma once

#include <Adafruit_TinyUSB.h>
#include <Arduino.h>

#include "BaseStreamAdapter.h"

extern Adafruit_USBD_CDC& USBSer1;

namespace Networking {
class SerialAdapter : public BaseStreamAdapter {
 public:
  SerialAdapter();

 protected:
  void initStream(void) override;
  bool isConnected(void) override;

 private:
  void transmitTask(void) override;
  void receiveTask(void) override;

  Adafruit_USBD_CDC& _usbSer1 = USBSer1;
};
}  // namespace Networking