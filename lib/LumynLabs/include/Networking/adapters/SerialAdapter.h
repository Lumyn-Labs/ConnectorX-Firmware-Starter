#pragma once

#include <Adafruit_TinyUSB.h>
#include <Arduino.h>
#include <PacketSerial.h>

#include "BaseStreamAdapter.h"

extern Adafruit_USBD_CDC& USBSer1;

namespace Networking {
class SerialAdapter : public BaseStreamAdapter {
 public:
  SerialAdapter();

 protected:
  void initStream(void) override;
  bool isConnected(void) override;
  void onTransmission(
      std::function<void(Transmission::Transmission*)> cb) override;

 private:
  void transmitTask(void) override;
  void receiveTask(void) override;

  void onPacketReceived(const uint8_t*, size_t);

  Adafruit_USBD_CDC& _usbSer1 = USBSer1;
  PacketSerial_<COBS, 0U, Transmission::kMaxPacketSize * 32> _serial;
  Transmission::Transmission _curTransmission;
  uint16_t _lastPacketId = -1;

  std::function<void(Transmission::Transmission*)> _onTransmissionCb;
};
}  // namespace Networking