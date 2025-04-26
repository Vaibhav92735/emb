#include <Arduino.h>
#include <EEPROM.h>
#include <avr/pgmspace.h>
#include "frame_array.h"

static constexpr uint16_t FRAME_WIDTH   = 64;
static constexpr uint16_t FRAME_HEIGHT  = 64;
static constexpr uint16_t DELTA_ADDR    =   0;
static constexpr uint16_t RLE_ADDR      = 2048;
static constexpr uint8_t  MAX_RUN       = 255;

void eepWriteInt8(int address, int8_t val) {
  EEPROM.update(address, static_cast<uint8_t>(val));
}

void eepWriteUint8(int address, uint8_t val) {
  EEPROM.update(address, val);
}

//----------------------------------------------
// Delta compression → EEPROM
//----------------------------------------------
void compressDeltaToEEPROM() {
  uint16_t addr = DELTA_ADDR;
  // Base pixel
  uint8_t prev = pgm_read_byte(&frame[0][0]);
  eepWriteUint8(addr++, prev);

  // Deltas
  for (uint16_t r = 0; r < FRAME_HEIGHT; ++r) {
    for (uint16_t c = (r==0?1:0); c < FRAME_WIDTH; ++c) {
      uint8_t curr = pgm_read_byte(&frame[r][c]);
      int16_t diff = int16_t(curr) - int16_t(prev);
      diff = constrain(diff, -128, 127);
      eepWriteInt8(addr++, static_cast<int8_t>(diff));
      prev = curr;
    }
  }
  Serial.println(F("Delta compression stored to EEPROM."));
}

//----------------------------------------------
// RLE compression → EEPROM
//----------------------------------------------
void compressRLEToEEPROM() {
  uint16_t addr = RLE_ADDR;
  uint8_t lastVal = pgm_read_byte(&frame[0][0]);
  uint8_t runLen  = 1;

  for (uint16_t r = 0; r < FRAME_HEIGHT; ++r) {
    for (uint16_t c = (r==0?1:0); c < FRAME_WIDTH; ++c) {
      uint8_t curr = pgm_read_byte(&frame[r][c]);
      if (curr == lastVal && runLen < MAX_RUN) {
        ++runLen;
      } else {
        eepWriteUint8(addr++, lastVal);
        eepWriteUint8(addr++, runLen);
        lastVal = curr;
        runLen  = 1;
      }
    }
  }
  eepWriteUint8(addr++, lastVal);
  eepWriteUint8(addr++, runLen);
  Serial.println(F("RLE compression stored to EEPROM."));
}

void setup() {
  Serial.begin(115200);
  while (!Serial) { delay(5); }

  Serial.println(F("\nChoose compression: 'd'=delta  'r'=RLE"));
  while (!Serial.available());
  char choice = Serial.read();
  if (choice == 'd' || choice == 'D') {
    compressDeltaToEEPROM();
  } else if (choice == 'r' || choice == 'R') {
    compressRLEToEEPROM();
  } else {
    Serial.println(F("Invalid choice. Halting."));
    while (true) { delay(1000); }
  }
}

void loop() {
  // In order to make it run for all the frames
}
