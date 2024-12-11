#pragma once

#include <Adafruit_TinyUSB.h>
#include <Arduino.h>

#include "BaseStreamAdapter.h"
#include "ConfigurationParser/ConfigurationValidator.h"
#include "Eventing/EventingService.h"
#include "INetworkingAdapter.h"

namespace Networking {
class WebUSBAdapter : public BaseStreamAdapter {
 public:
  WebUSBAdapter();

 protected:
  void initStream(void) override;
  bool isConnected(void) override;

 private:
  static void connectedCb(bool);

  inline static EventingService* _evtService = &EventService;
  Adafruit_USBD_WebUSB _webUsb;
  // TODO: change this
  WEBUSB_URL_DEF(landingPage, 1 /*https*/,
                 "example.tinyusb.org/webusb-serial/index.html");
};
}  // namespace Networking