# ConnectorX - Custom Firmware Starter

Custom firmware for ConnectorX: your own modules and animations, built on the Lumyn Labs SDK. Everything the stock firmware does keeps working, including Lumyn Studio, the WPILib vendordep and the other SDKs.

## Quick start

1. Install PlatformIO (the VS Code extension is easiest). On Windows, [enable long file paths](https://arduino-pico.readthedocs.io/en/latest/platformio.html#important-steps-for-windows-users-before-installing) first.
2. Open this folder. PlatformIO installs the toolchain and libraries, which takes a few minutes the first time.
3. Edit `src/main.cpp`, then build and flash: `pio run -t upload`, or **Upload** in the PlatformIO pane.

The SDK is already in `lib/LumynLabsSDK/` (headers in `include/`, the compiled library in `lib/`). Keep `platformio.ini` as it is: the SDK is compiled against that pinned platform, with that USB configuration (`CFG_TUSB_CONFIG_FILE`), interrupt table (`maxIRQs`) and FreeRTOS settings, and your firmware must match them. The SDK's `library.json` pins the ArduinoJson version it was built with.

## Startup order

```cpp
void setup() {
  LumynLabs::System::init(true);  // true sends logs to the board's second USB serial port
  // Register your modules and animations here, before initServices().
  LumynLabs::System::initServices();
}

void loop() { vTaskDelete(NULL); }
```

The SDK runs in its own tasks, so `loop()` ends its task and frees its 4 KB stack for the heap. A `loop()` that keeps running costs those 4 KB, and so does defining `setup1()` or `loop1()`, which starts a task on the second core.

## Custom modules

A module reads a sensor, or drives something, on one of the board's connectors. Robot code receives what `readData()` returns at the module's polling rate, and can send the module JSON, which arrives in `handleReceivedJson()`.

A module is a class derived from `LumynLabs::Module<T>`, where `T` is the plain struct it reports. `src/main.cpp` has a complete one, `MySensor`:

- `initModule()` sets up the hardware once. Return `LumynLabs::ModuleError::ok()`, or `LumynLabs::ModuleError::error(type)` if it fails.
- `readData(T *dataOut)` fills in a reading. It's called at the module's polling rate.
- `handleReceivedJson(json)` is optional, for modules that take commands.
- `peripherals()` gives the module its bus, for example `peripherals().getI2C()`.

Register the type by name, then add a module of that type to the device's config in Lumyn Studio:

```cpp
LumynLabs::registerModule<MySensorData, MySensor>("MY_SENSOR");
```

ConnectorX runs up to five modules, on I2C, SPI, UART, DIO or AIO.

## Custom animations

An animation is an `Effect`. `step()` moves it on by one tick and `draw()` paints the current frame, which starts cleared each time. Ticks run 60 times a second at the animation's default delay; a command with a shorter delay runs them faster, up to 10 times as fast. `make()` builds a fresh effect for each zone it plays on, so an effect can size its state to the zone.

```cpp
class BlinkEffect : public LumynLabs::Effect {
public:
  void step() override { _tick = (_tick + 1) % 60; }
  void draw(LumynLabs::Frame &frame) override {
    if (_tick >= 30) return;  // off for the second half
    for (int i = 0; i < frame.size(); i++) frame.pixels[i] = frame.color;
  }
private:
  uint32_t _tick = 0;
};

static const LumynLabs::AnimationInstance MyBlink = {
    .id = "MY_BLINK",           // the name commands and sequences use
    .defaultDelay = 500,
    .defaultColor = LumynLabs::Color::Blue(),
    .cycleTicks = 60,           // one cycle; a sequence step plays one
    .flags = 0,                 // LumynLabs::kStill and the other flags, in LumynLabs/Led/Animation.h
    .make = LumynLabs::makeEffect<BlinkEffect>};

// In setup(), before initServices():
LumynLabs::registerAnimation(MyBlink);
```

A frame is the zone's pixels row by row from the top left (`frame.at(x, y)`); a strip is one row. Play it from robot code or Lumyn Studio by its name, `MY_BLINK`.

The SDK includes Adafruit GFX, BusIO and SSD1306, and FastLED, and pins ArduinoJson. Use them from the SDK and don't add any of them to `lib_deps`: a second copy won't link, and another ArduinoJson version breaks modules' JSON.

## Recovery

If your firmware gets the board stuck, flash the stock firmware:

1. Hold **BOOTSEL**, press and release **RST**, then release **BOOTSEL**. A drive appears on your computer.
2. Copy [`firmware/firmware.uf2`](firmware/firmware.uf2) to it. The board restarts with the stock firmware.

This works whatever is running on the board.

Don't use PlatformIO's **Upload Filesystem Image**: it erases files the board needs.
