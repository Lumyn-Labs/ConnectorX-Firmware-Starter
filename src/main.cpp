// =============================================================================
// ConnectorX - Custom Firmware Example
// =============================================================================
//
// This example demonstrates how to create and register custom modules
// (sensors/actuators) using the Lumyn Labs SDK.
//
// =============================================================================

#include <Arduino.h>
#include <LumynLabs.h>
#include <LumynLabs/Cx.h> // Includes all variant-specific APIs

// =============================================================================
// Example Custom Module: A Simple Sensor
// =============================================================================

/**
 * Data structure for our custom sensor.
 * This is what gets sent when the module is polled.
 */
struct MySensorData
{
  float temperature;
  float humidity;
  uint32_t counter;
};

/**
 * Example custom sensor module.
 * Shows how to implement the Module<T> interface.
 */
class MySensor : public LumynLabs::Module<MySensorData>
{
public:
  /**
   * Constructor - receives configuration from the system.
   */
  MySensor(const LumynLabs::ModuleConfig &config)
      : Module(config), _counter(0) {}

  /**
   * Initialize the module hardware.
   * Called once when the module is registered.
   */
  LumynLabs::ModuleError initModule() override
  {
    // Example: Initialize I2C sensor at address 0x40
    // auto& i2c = peripherals().getI2C();
    // i2c.beginTransmission(0x40);
    // if (i2c.endTransmission() != 0) {
    //   return LumynLabs::ModuleError::initError("Sensor not found");
    // }

    Serial.println("MySensor initialized!");
    return LumynLabs::ModuleError::ok();
  }

  /**
   * Read data from the sensor.
   * Called periodically based on pollingRateMs.
   */
  LumynLabs::ModuleError readData(MySensorData *dataOut) override
  {
    // In a real implementation, read from your sensor:
    // auto& i2c = peripherals().getI2C();
    // i2c.requestFrom(0x40, 4);
    // dataOut->temperature = readTemperature();
    // dataOut->humidity = readHumidity();

    // Demo: Return fake data
    dataOut->temperature = 25.0f + (random(100) / 100.0f);
    dataOut->humidity = 50.0f + (random(200) / 100.0f);
    dataOut->counter = ++_counter;

    return LumynLabs::ModuleError::ok();
  }

  /**
   * Optional: Handle incoming commands.
   * Override this if your module can receive commands.
   */
  bool pushData(ArduinoJson::JsonVariantConst json) override
  {
    // Example: Handle a "reset" command
    if (json["command"].is<const char *>())
    {
      if (strcmp(json["command"], "reset") == 0)
      {
        _counter = 0;
        return true;
      }
    }
    return false;
  }

private:
  uint32_t _counter;
};

// =============================================================================
// Setup & Loop
// =============================================================================

void setup()
{
  // Initialize the system (networking, display, etc.)
  bool success = LumynLabs::System::init();
  if (!success)
  {
    Serial.println("System init failed");
    while (1)
    {
      delay(1000);
    }
  }

  // Register your custom module type
  // The string "MY_SENSOR" is used to match this module in configuration
  LumynLabs::registerModule<MySensorData, MySensor>("MY_SENSOR");

  // You can register additional modules here:
  // LumynLabs::registerModule<OtherData, OtherModule>("OTHER_MODULE");

  // Complete initialization (starts services)
  success = LumynLabs::System::initServices();
  if (!success)
  {
    Serial.println("Service init failed");
    while (1)
    {
      delay(1000);
    }
  }

  Serial.printf("ConnectorX ready! SDK v%s\n", LumynLabs::getSDKVersion());
}

void loop() { delay(10000); }

// Second core setup, safest to do nothing here
void setup1() {}

void loop1() { delay(10000); }