# Video Compression & Segmentation on Microcontrollers

> **Project Objective:**  
> Implement video compression and segmentation algorithms on microcontroller platforms, optimizing them for performance and memory usage to make them viable for mobile and edge-device deployments.

---

## 📋 Table of Contents
1. [Overview](#overview)
2. [Features](#features)
3. [Components & Workflow](#components--workflow)
4. [Prerequisites](#prerequisites)
5. [Installation & Setup](#installation--setup)
6. [Usage](#usage)
    - [1. Frame Extraction (Python)](#1-frame-extraction-python)
    - [2. Image-to-C Array Conversion (Python)](#2-image-to-c-array-conversion-python)
    - [3. Compression on Microcontroller (Arduino)](#3-compression-on-microcontroller-arduino)
    - [4. Serial Raw Capture (Python)](#4-serial-raw-capture-python)

---

## 🌟 Overview
This project demonstrates a full pipeline for processing video frames on resource-constrained platforms:

1. **Extract** individual frames from a video file in Python.
2. **Convert** grayscale frames into C-style 2D arrays for embedding in firmware.
3. **Compress** the embedded frames on a microcontroller via delta or run-length encoding (RLE), storing compressed data in flash memory (EEPROM).
4. **Capture** the raw compressed output over serial back to a host PC for analysis or storage.

The focus is on optimizing memory footprint and CPU load so that these techniques can run on low-power microcontrollers commonly used in mobile and edge devices.

---

## ✅ Features
- **Frame extraction** via OpenCV script with command-line options.
- **Image-to-C array** header generation using Pillow.
- **Delta and RLE compression** routines in Arduino C++ (with selection at runtime).
- **EEPROM storage** of compressed data for non-volatile retention.
- **Raw serial capture** utility in Python for host-side logging.

---

## ⚙ Components & Workflow

1. **`extract_frames.py`** (Python):  
   - Reads a video file and saves every _n_<sup>th</sup> frame as an image.

2. **`image_to_c_array.py`** (Python):  
   - Converts a single grayscale image into a `.h` file defining a 2D `uint8_t` array.

3. **`compressor.ino`** (Arduino C++):  
   - Embeds `frame_array.h` via `PROGMEM`.  
   - Offers two modes:  
     - **Delta compression**: differences between successive pixels.  
     - **RLE compression**: runs of identical pixel values.  
   - Stores output in EEPROM at separate address regions.  
   - User selects mode (`d` or `r`) over serial at startup.

4. **`serial_raw_dumper.py`** (Python):  
   - Connects to the microcontroller’s serial port.  
   - Reads raw bytes in fixed-size chunks and writes them to a binary file.

---

## 📋 Prerequisites
- **Python 3.7+** with packages:  
  - `opencv-python`  
  - `pillow`  
  - `pyserial`  
- **Arduino IDE** or VSCode with PlatformIO  
- A **microcontroller** (e.g., Arduino Uno/Pro Mini) with EEPROM support  
- USB cable for serial connection  

---

## 🚀 Installation & Setup

1. **Clone the repository**  
   ```bash
   git clone https://github.com/Vaibhav92735/emb.git
   cd video-edge-compression

2. **Install Python dependencies**
    ```bash
    pip install opencv-python pillow pyserial

3. **Prepare Arduino sketch**
    - Open compressor.ino in the Arduino IDE.
    - Install any required libraries (none external besides built-ins).
