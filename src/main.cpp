#include <SystemService/SystemService.h>
#include <Modules/builtin_modules/VL53L1X.h>

void setup()
{
  bool success = SystemManagerService.init();
  if (!success)
  {
    Serial.println("FAILURE");
    while (1)
    {
      delay(1000);
    }
  }

  // TODO: Register your custom Animations here
  // TODO: Register your custom Modules here
  SystemManagerService.registerModuleType(
      "VL53L1X", [](const Configuration::Sensor &cfg)
      { return std::make_shared<Modules::Builtin::VL53L1XModule>(cfg); });

  success = SystemManagerService.initServices();
  if (!success)
  {
    Serial.println("FAILURE");
    while (1)
    {
      delay(1000);
    }
  }
}

// TODO: Remove vTaskDelete(NULL) if running code within here
void loop() { vTaskDelete(NULL); }

void setup1() {}

// TODO: Remove vTaskDelete(NULL) if running code within here
// ! NOTE: Make sure your code is thread-safe since this is a separate
// ! RTOS task
void loop1() { vTaskDelete(NULL); }