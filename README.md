# ConnectorX-Fimware-Starter

- [ConnectorX-Fimware-Starter](#connectorx-fimware-starter)
  - [What can you do with custom firmware?](#what-can-you-do-with-custom-firmware)
  - [Recovery](#recovery)
    - [Flash a default UF2](#flash-a-default-uf2)
    - [Revert to empty main.cpp](#revert-to-empty-maincpp)
  - [Set up](#set-up)
    - [Install PlatformIO](#install-platformio)
    - [main.cpp](#maincpp)
    - [Register Custom Animations](#register-custom-animations)
      - [What is an Animation?](#what-is-an-animation)
      - [More about `Animation::AnimationFrameCallback`](#more-about-animationanimationframecallback)
      - [Creating one](#creating-one)
      - [Registration](#registration)
    - [Register Custom Modules](#register-custom-modules)
    - [Flashing firmware](#flashing-firmware)
    - [:warning: BE CAREFUL :warning:](#warning-be-careful-warning)
  - [Available classes/methods](#available-classesmethods)
    - [SystemManagerService](#systemmanagerservice)
    - [LedService](#ledservice)
    - [SerialLogger](#seriallogger)
    - [EventingService](#eventingservice)

## What can you do with custom firmware?

At Lumyn Labs, we believe in empowering everyone to truly "connect anything", which also means letting you to use your device in the way you want. Whether it's creating a new Animation for use in a Sequence or adding a Custom Module to read from a favorite sensor, custom firmware allows for it all. By using this repository, you can leverage the existing ConnectorX infrastructure/libraries to do incredible things.

## Recovery

### Flash a default UF2

Before we get into how new firmware is written and flashed to your ConnectorX, let's go over what to do if you or your device get stuck. This repository contains a default UF2 file that is identical in function to the image that shipped on your device (no custom Modules or Animations), although be aware that future releases of the firmware will also be reflected in it, such as bug fixes or a new feature. Every ConnectorX is based on an RP2040 chip and can be flashed in the same way!

With the board plugged into your computer, simply:

1. Hold down the `BOOT`/`BOOTSEL` button near the USB-C port
2. While holding the button, click `RST`/`RESET` and _continue_ holding the `BOOT`/`BOOTSEL` button
3. Release the `BOOT`/`BOOTSEL` button when a drive mounts to your computer
4. Drag-n-drop the UF2 file found [here](/firmware/firmware.uf2) in the repository and wait for it to upload
5. Watch as your ConnectorX reboots

This process will work regardless of the software running on the ConnectorX, so if you accidentally created an infinite loop- no worries!

### Revert to empty main.cpp

Alternatively, follow the instructions below to compile the default firmware locally and then flash via PlatformIO

## Set up

### Install PlatformIO

After cloning the repository and opening in VS Code, [install the PlatformIO extension](https://marketplace.visualstudio.com/items?itemName=platformio.platformio-ide) and then [follow these instructions](https://arduino-pico.readthedocs.io/en/latest/platformio.html#important-steps-for-windows-users-before-installing) if you're on Windows to enable long filepaths.

Re-open the repository in VS Code after rebooting and watch as PlatformIO installs the necessary toolchains, libraries, and board files automatically. Do note that this step can take several minutes, so be patient.

### main.cpp

With PlatformIO installed, open [main.cpp](/src/main.cpp) and notice the `TODO` comment between the two `init()` calls. Ordering here is **very** important since `init()` sets up the board's basic functionality and gives a place for your custom Modules/Animations to go while `initServices()` then takes the registered entities and allows the Configuration in code or on your SD card to use them.

### Register Custom Animations

#### What is an Animation?

LED Animations revolve around the core concept of "states". If an LED blinks on or off, it has 2 states ('on' and 'off'). If an LED breathes (gradually moves to full brightness and then back to off), it has 512 states: 256 when going up (0 to 255) and 256 more when going back down (256 to 511). Your Animations must follow this same model. Because some Animations depend on the number of LEDs in addition to a set number of states (think Chase or one where the LEDs 'grow' along the entire strip), we have the concept of a `StateMode` which can be either `Constant` (number of states never changes) or `LedCount` (number of states is related to the number of LEDs). If it's `LedCount`, then the number of states is: `# of LEDs + the state count`.

Let's go through the core building block of every Animation: the `Animation::AnimationInstance` struct:

|  Member name   |                Type                 |                              Description                               |
| :------------: | :---------------------------------: | :--------------------------------------------------------------------: |
|      `id`      |         `std::string_view`          | The ID to reference this Animation in commands and Animation Sequences |
|  `stateMode`   |       `Animation::StateMode`        |    `Constant` for constant; `LedMode` for LED count + `stateCount`     |
|  `stateCount`  |             `uint16_t`              |                            Number of states                            |
| `defaultDelay` |             `uint16_t`              |      The default delay between each state update in milliseconds       |
| `defaultColor` |    `Configuration::ActionColor`     |   The default color of the Animation given as 8-bit `r, g, b` values   |
|      `cb`      | `Animation::AnimationFrameCallback` |            The function called every time the state updates            |

#### More about `Animation::AnimationFrameCallback`

This function is called every time the state changes. The signature is: `std::function<bool(CRGB*, CRGB, uint16_t, uint16_t)>` with the parameters of:

|  Name   |    Type    |                                                      Description                                                       |
| :-----: | :--------: | :--------------------------------------------------------------------------------------------------------------------: |
| `strip` |  `CRGB*`   |   The array of raw color values. This corresponds 1:1 with the zone so do not modify values outside of its boundary    |
| `color` |   `CRGB`   |  The requested color. Note that this may be ignored depending on the Animation's needs (such as a rainbow animation)   |
| `state` | `uint16_t` |        The current, 0-indexed state that the callback needs to handle. It is incremented automatically for you         |
| `count` | `uint16_t` | The number of LEDs in the Zone. This must be used when updating the `strip` array in order to not exceed its boundary! |

The callback **must** return a `bool`. Return `true` if the Channel should be updated for this Animation's state or `false` to not update.

#### Creating one

To create a custom Animation, it is recommended to create a new header file (`.h`) inside the [animations folder](/include/animations) with the name of your custom Animation. It must be of the type `Animation::AnimationInstance` and have a name that does not conflict with any existing Animations. After creating your Animation, include the header file in `main.cpp` and then register it:

#### Registration

Simply call `AnimationMngr.registerAnimation(std::move(MyAnimationStruct));` where `MyAnimationStruct` is the name of your `static Animation::AnimationInstance` value.

Note: The Animations in [`include/animations/BuiltInAnimations.h`](/include/animations/BuiltInAnimations.h) are registered **automatically** by the system and shouldn't typically be modified!

### Register Custom Modules

Similar to how all Animations are an `Animation::AnimationInstance`, all Modules must inherit from the `Modules::BaseModule` class. To create a new Custom Module in your firmware, first take a look at [our docs](https://docs.lumynlabs.com/#/lumyn-studio/modules-page/?id=creating-a-custom-module) which covers how to get started and generate some boilerplate code that can be copy-pasted into a header file. Once the header has been added inside of the `include` directory, make sure to address the `TODO` comments for each configuration parameter, if any, as well as populating the custom Payload inside of the `poll()` method.

To make your Custom Module known to the rest of the system and be able to instantiate an instance(s) from a Configuration, place

```c++
SystemManagerService.registerModuleType(
  "MY_CUSTOM_MODULE_ID", [](const Configuration::Sensor& cfg) {
    return std::make_shared<MyCustomModuleClass>(cfg);
});
```

into the appropriate section in `main.cpp`. Remember that `"MY_CUSTOM_MODULE_ID"` should be replaced with the same ID you gave it in the Custom Module Editor in Lumyn Studio and `MyCustomModuleClass` is the class name in the header for your Custom Module.

### Flashing firmware

On the left pane, click the PlatformIO icon (the ant), then open the folder labeled `pico` and click `General > Upload` after ensuring your board is available for flashing (see [Flash a default UF2](#flash-a-default-uf2) and follow through step 3).

### :warning: BE CAREFUL :warning:

PlatformIO gives the option to **Upload Filesystem Image**. DO NOT CLICK THIS because the ConnectorX ships with its filesystem already flashed with important internal files that will render the device **inoperable** if deleted/overwritten.

## Available classes/methods

### SystemManagerService

Used to initialize the system. Optionally, you can call `getStatus`, `getErrorFlags`, or `getAssignedId` to check on your Device's status. **Do not call start()** as this will start additional `SystemService` tasks in the RTOS and lead to conflicts.

### LedService

In here, you can manually register additional Channels, Animations, Animation Sequences, and Image Sequences. There are also methods that expose sending asynchronous LED commands from your code.

### SerialLogger

Use `SerialLogger` to log messages via the secondary serial port in a thread-safe way. There are 5 different logging levels: `Verbose`, `Info`, `Warn`, `Error`, and `Fatal`.

### EventingService

You can send an `Eventing::Event` with a call to `EventService.sendEvent()`. If calling from an interrupt, use `EventService.sendEventFromISR()`.
