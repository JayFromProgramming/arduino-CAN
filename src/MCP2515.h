// Copyright (c) Sandeep Mistry. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#ifndef MCP2515_H
#define MCP2515_H

#include <SPI.h>
#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>
#include "CANController.h"

#define MCP2515_DEFAULT_CLOCK_FREQUENCY 16e6

#if defined(ARDUINO_ARCH_SAMD) && defined(PIN_SPI_MISO) && defined(PIN_SPI_MOSI) && defined(PIN_SPI_SCK) && (PIN_SPI_MISO == 10) && (PIN_SPI_MOSI == 8) && (PIN_SPI_SCK == 9)
// Arduino MKR board: MKR CAN shield CS is pin 3, INT is pin 7
#define MCP2515_DEFAULT_CS_PIN          3
#define MCP2515_DEFAULT_INT_PIN         7
#else
#define MCP2515_DEFAULT_CS_PIN          10
#define MCP2515_DEFAULT_INT_PIN         2
#endif

class MCP2515Class : public CANControllerClass {

public:
  MCP2515Class();
  virtual ~MCP2515Class();

  virtual int begin(long baudRate);
  virtual void end();

  virtual int endPacket();

  virtual int parsePacket();

  virtual void onReceive(void(*callback)(int));

  using CANControllerClass::filter;
  virtual int filter(int id, int mask);
  using CANControllerClass::filterExtended;
  virtual int filterExtended(long id, long mask);

  virtual int observe();
  virtual int loopback();
  virtual int sleep();
  virtual int wakeup();

  void setPins(int cs = MCP2515_DEFAULT_CS_PIN, int irq = MCP2515_DEFAULT_INT_PIN);
  void setSPIFrequency(uint32_t frequency);
  void setClockFrequency(long clockFrequency);

  void dumpRegisters(Stream& out);

private:
  void reset() const;

  bool handleInterrupt();

  uint8_t readRegister(uint8_t address) const;
  void modifyRegister(uint8_t address, uint8_t mask, uint8_t value) const;
  void writeRegister(uint8_t address, uint8_t value) const;

  void acquireSPIBus() const;
  void releaseSPIBus() const;


#ifdef ARDUINO_ARCH_ESP32
  static void interruptTask(void* arg);
  static void onInterrupt();
#else
  static void onInterrupt();
#endif


private:
  bool _hasBegun = false;
  SemaphoreHandle_t _spiSemaphore; // FreeRTOS semaphore for SPI bus sharing
  SemaphoreHandle_t _interruptSemaphore; // FreeRTOS semaphore for interrupt handling
  TaskHandle_t _interruptTask; // FreeRTOS task handle for interrupt handling
  SPISettings _spiSettings;
  int _csPin;
  int _intPin;
  long _clockFrequency;
};

extern MCP2515Class CAN;

#endif
