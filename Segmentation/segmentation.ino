#include <Arduino.h>
#include <avr/pgmspace.h>
#include "frame_array.h"

static constexpr uint16_t FRAME_WIDTH  = 64;
static constexpr uint16_t FRAME_HEIGHT = 64;
static constexpr float    THRESH_FACTOR = 0.5f;

uint8_t processedFrame[FRAME_HEIGHT][FRAME_WIDTH];

float computeMean() {
    uint32_t sum = 0;
    for (uint16_t r = 0; r < FRAME_HEIGHT; ++r) {
        for (uint16_t c = 0; c < FRAME_WIDTH; ++c) {
            sum += pgm_read_byte(&frame[r][c]);
        }
    }
    return static_cast<float>(sum) / (FRAME_WIDTH * FRAME_HEIGHT);
}

float computeStdDev(float mean) {
    float acc = 0.0f;
    for (uint16_t r = 0; r < FRAME_HEIGHT; ++r) {
        for (uint16_t c = 0; c < FRAME_WIDTH; ++c) {
            float diff = pgm_read_byte(&frame[r][c]) - mean;
            acc += diff * diff;
        }
    }
    return sqrt(acc / (FRAME_WIDTH * FRAME_HEIGHT));
}

uint8_t computeThreshold(float mean, float stddev) {
    float t = mean + THRESH_FACTOR * stddev;
    if (t < 0.0f) t = 0.0f;
    if (t > 255.0f) t = 255.0f;
    return static_cast<uint8_t>(t);
}

void applyThreshold(uint8_t threshold) {
    for (uint16_t r = 0; r < FRAME_HEIGHT; ++r) {
        for (uint16_t c = 0; c < FRAME_WIDTH; ++c) {
            uint8_t px = pgm_read_byte(&frame[r][c]);
            processedFrame[r][c] = (px > threshold) ? 255 : 0;
        }
    }
}

void printFrame() {
    for (uint16_t r = 0; r < FRAME_HEIGHT; ++r) {
        for (uint16_t c = 0; c < FRAME_WIDTH; ++c) {
            Serial.print(processedFrame[r][c]);
            if (c + 1 < FRAME_WIDTH) Serial.print(',');
        }
        Serial.println();
    }
}

void setup() {
    Serial.begin(115200);
    while (!Serial) { delay(10); }

    float mean   = computeMean();
    float stddev = computeStdDev(mean);
    uint8_t thresh = computeThreshold(mean, stddev);

    applyThreshold(thresh);

    Serial.println(F("-- Processed Frame (0 or 255) --"));
    printFrame();
}

void loop() {
    // In order to make it run for all the frames
}