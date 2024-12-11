#pragma once

#include <Arduino.h>

#include <string>
#include <string_view>

class IDCreator {
 public:
  static uint16_t createId(const std::string& id);
  static uint16_t createId(std::string_view id);

 private:
  static uint16_t createIdHelper(std::string_view id);
};