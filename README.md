# PMSA003I C library/Hardware-Driver for Raspberry Pi

A C-based driver for interfacing with the **Plantower PMSA003I** laser particle sensor using **I2C** and **GPIO** on a Raspberry Pi. This library enables you to initialize, reset and control the sensor and read air quality data from the PMSA003I sensor.

## 📦 Features

- I2C communication with PMSA003I
- GPIO-based reset and working state control (using `pigpio`)
- Read:
  - Factory-calibrated PM1.0, PM2.5, PM10 concentrations
  - Atmospheric environment PM values
  - Particle counts of various sizes (e.g. PM0.3, PM0.5, PM1.0, etc.)
- Clean initialization and shutdown functions

## 🛠️ Requirements

- Raspberry Pi or compatible Linux SBC
- `libpigpio` installed and daemon running
- I2C enabled (`/dev/i2c-1` or similar)
- C compiler (e.g. `gcc`)

## 🔧 Setup

### 1. Enable I2C and Install Dependencies

```bash
sudo raspi-config       # Enable I2C under Interface Options
sudo apt update
sudo apt install pigpio libi2c-dev gcc
sudo pigpiod
```

## 🌍 Supported Measurements
| Function                                      | Description                               |
| --------------------------------------------- | ----------------------------------------- |
| `pmsa003i_read_PM_factory_calibrated()`       | Factory-calibrated PM1.0, PM2.5, PM10     |
| `pmsa003i_read_PM_atmospheric_environment()`  | Atmospheric PM values                     |
| `pmsa003i_read_PM_size_and_greater_in_0_1L()` | Particle counts ≥ specified size per 0.1L |
