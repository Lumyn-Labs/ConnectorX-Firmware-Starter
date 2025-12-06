#pragma once

#include <Arduino.h>
#include <Wire.h>

namespace lumyn::sensors {
class APDS9151 {
 public:
  enum class Register : uint8_t {
    MAIN_CTRL = 0x00,
    PS_LED = 0x01,
    PS_PULSES = 0x02,
    PS_MEAS_RATE = 0x03,
    LS_MEAS_RATE = 0x04,
    LS_GAIN = 0x05,
    PART_ID = 0x06,
    MAIN_STATUS = 0x07,
    PS_DATA_0 = 0x08,
    PS_DATA_1 = 0x09,
    LS_DATA_IR_0 = 0x0A,
    LS_DATA_IR_1 = 0x0B,
    LS_DATA_IR_2 = 0x0C,
    LS_DATA_GREEN_0 = 0x0D,
    LS_DATA_GREEN_1 = 0x0E,
    LS_DATA_GREEN_2 = 0x0F,
    LS_DATA_BLUE_0 = 0x10,
    LS_DATA_BLUE_1 = 0x11,
    LS_DATA_BLUE_2 = 0x12,
    LS_DATA_RED_0 = 0x13,
    LS_DATA_RED_1 = 0x14,
    LS_DATA_RED_2 = 0x15,
    INT_CFG = 0x19,
    INT_PST = 0x1A,
    PS_THRES_UP_0 = 0x1B,
    PS_THRES_UP_1 = 0x1C,
    PS_THRES_LOW_0 = 0x1D,
    PS_THRES_LOW_1 = 0x1E,
    PS_CAN_0 = 0x1F,
    PS_CAN_1 = 0x20,
    LS_THRES_UP_0 = 0x21,
    LS_THRES_UP_1 = 0x22,
    LS_THRES_UP_2 = 0x23,
    LS_THRES_LOW_0 = 0x24,
    LS_THRES_LOW_1 = 0x25,
    LS_THRES_LOW_2 = 0x26,
    LS_THRES_VAR = 0x27
  };

  enum class Gain : uint8_t {
    X1 = 0x00,
    X3 = 0x01,
    X6 = 0x02,
    X9 = 0x03,
    X18 = 0x04
  };

  enum class LEDCurrent : uint8_t {
    MA_12_5 = 0x00,
    MA_25 = 0x01,
    MA_50 = 0x02,
    MA_100 = 0x03
  };

  enum class MeasurementRate : uint8_t {
    MS_25 = 0x00,
    MS_50 = 0x01,
    MS_100 = 0x02,
    MS_200 = 0x03,
    MS_500 = 0x04,
    MS_1000 = 0x05,
    MS_2000 = 0x06
  };

  enum class Resolution : uint8_t {
    BIT_20 = 0x00,
    BIT_19 = 0x08,
    BIT_18 = 0x10,
    BIT_17 = 0x18,
    BIT_16 = 0x20,
    BIT_13 = 0x28
  };

  enum class MainCtrl : uint8_t {
    LS_EN = 0x02,
    PS_EN = 0x01,
    RGB_MODE = 0x04,
    OFF = 0x00
  };

  enum class Status : uint8_t {
    PS_DATA_READY = 0x01,
    PS_INT = 0x02,
    LS_DATA_READY = 0x08,
    LS_INT = 0x10
  };

  struct Color {
    uint32_t red;
    uint32_t green;
    uint32_t blue;
    uint32_t clear;
  };

  struct NormalizedColor {
    float red;
    float green;
    float blue;
  };

  struct ProximityData {
    uint16_t value;
    bool overflow;
  };

  static constexpr uint8_t DEFAULT_ADDRESS = 0x52;
  static constexpr uint8_t PART_ID_VALUE = 0xC2;
  static constexpr uint8_t DEFAULT_PS_PULSES = 0x08;
  static constexpr uint16_t PS_MAX_VALUE = 0x07FF;   // 11-bit max
  static constexpr uint32_t LS_MAX_VALUE = 0xFFFFF;  // 20-bit max

  explicit APDS9151(uint8_t address = DEFAULT_ADDRESS);

  bool begin(TwoWire& wire = Wire);
  void enable();
  void disable();
  void reset();

  // Configuration
  void setLSGain(Gain gain);
  void setLEDCurrent(LEDCurrent current);
  void setPSPulses(uint8_t pulses);
  void setLSMeasurementRate(Resolution resolution, MeasurementRate rate);
  void setPSMeasurementRate(Resolution resolution, MeasurementRate rate);
  void setPSCancellation(uint16_t digitalCancel, uint8_t analogCancel = 0);

  // Status
  bool isLSDataReady();
  bool isPSDataReady();
  uint8_t getStatus();
  uint8_t getPartID();

  // Data reading
  Color getColor();
  NormalizedColor getNormalizedColor();
  ProximityData getProximityData();
  uint16_t getProximity();  // Legacy method, doesn't return overflow
  uint32_t getRed();
  uint32_t getGreen();
  uint32_t getBlue();
  uint32_t getClear();

  // Utility
  float getColorTemperature();
  float getIlluminance();

 private:
  uint8_t _address;
  TwoWire* _wire;
  Gain _currentGain;

  void writeRegister(Register reg, uint8_t value);
  uint8_t readRegister(Register reg);
  void readRegisterBlock(Register startReg, uint8_t* buffer, uint8_t length);
  uint32_t read20BitColor(Register regLow);

  static constexpr float MAX_20BIT = 1048575.0f;
  static constexpr uint8_t PS_OVERFLOW_BIT = 0x08;  // Bit 3 of PS_DATA_1
  static constexpr uint8_t PS_DATA_MASK = 0x07;     // Lower 3 bits of PS_DATA_1
};
}