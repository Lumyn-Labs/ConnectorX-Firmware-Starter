#pragma once

#include <Arduino.h>
#include <FreeRTOS.h>
#include <message_buffer.h>
#include <semphr.h>
#include <task.h>

#include <string>

class Logger {
 public:
  enum class LogLevel {
    Verbose = 0,
    Info,
    Warning,
    Error,
    Fatal,
  };

  Logger(Adafruit_USBD_CDC& output, uint32_t maxBufferSize,
         LogLevel minLevel = LogLevel::Info);

  void verbose(const char* fmt, ...);
  void info(const char* fmt, ...);
  void warn(const char* fmt, ...);
  void error(const char* fmt, ...);
  void fatal(const char* fmt, ...);

  void start(void);

  void log(LogLevel, const char* fmt, va_list ap);
  void logFn(LogLevel level, const char* s);

 private:
  std::string levelToString(LogLevel);

  static void taskImpl(void* _this);
  void task(void);

  static inline const uint16_t kMaxBufSize = 1024;

  Adafruit_USBD_CDC& _stream;
  LogLevel _minLevel;

  TaskHandle_t _task;
  MessageBufferHandle_t _msgBuf;
  SemaphoreHandle_t _mutex;
};

extern Logger SerialLogger;
extern Adafruit_USBD_CDC USBDataPort;