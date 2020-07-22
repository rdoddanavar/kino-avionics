# kino-avionics

> High Power Rocketry - Avionics Hardware & Software

![](https://img.shields.io/badge/EDA-KiCAD-lightgrey.svg)
![](https://img.shields.io/github/license/rdoddanavar/hpr-hardware.svg)
![](https://img.shields.io/github/repo-size/rdoddanavar/hpr-hardware.svg)

Flight computer to serve data acquisition & tracking needs of a dual-deploy high power amateur model rocket. Designed around a Raspberry Pi Zero.

## Overview

| Control            | Data Acquisition | Telemetry | Misc.  |
|--------------------|------------------|-----------|--------|
| N/A                | 9DOF IMU         | Recovery  | Buzzer |
|                    | GPS              | IMU/GPS   |        |
|                    | Pressure         |           |        |
|                    | Temperature      |           |        |
|                    | Humidity         |           |        |

## File Formats

| Purpose             | Application      | Format        |
|---------------------|------------------|---------------|
| Spreadsheet         | LibreOffice Calc | `*.fods`      |
| EDA                 | KiCAD            | `< >`         |
| CAM                 | " "              | `< >`         |
| Arduino Source      | avr-gcc          | `*.ino`,`*.h` |
| Raspberry Pi Source | gcc              | `*.c`,`*.h`   |


## Directory Index

 - `cam/` - CAM files for board fabrication  
 - `doc/` - Supporting documents, references, & datsheets 
 - `software/`
   - `kino_arduino/` - Arduino flight software deployed on Seeeduino Xiao
   - `kino_rpi/` - C/C++ flight software deployed on RPI Zero

```
.
├── cam/
│   └── placeholder
├── doc/
│   ├── arduino_mini_pinout.png
│   ├── honeywell-sensing-trustability-ssc-series-standard-740340.pdf
│   ├── rpi_pinout.png
│   └── seeeduino_xiao_pinout.jpg
├── software/
│   ├── kino_arduino/
│   └── kino_rpi/
├── license
├── readme.md
└── update_index.sh

5 directories, 8 files
```
