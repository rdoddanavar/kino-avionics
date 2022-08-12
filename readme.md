# kino-avionics

> High Power Rocketry - Avionics Hardware & Software

![](https://img.shields.io/badge/EDA-KiCAD-lightgrey.svg)
![](https://img.shields.io/github/license/rdoddanavar/hpr-hardware.svg)
![](https://img.shields.io/github/repo-size/rdoddanavar/hpr-hardware.svg)

Series of avionics systems to support data acquisition & tracking needs of a dual-deploy high power amateur model rocket.

## Overview

### Kino 1

> Flight data logging board

| Component                    | Purpose                                             |
|------------------------------|-----------------------------------------------------|
| Raspberry Pi Zero W          | Main flight computer                                |
| Arduino Pro Mini (3.3V/8MHz) | Data acquisition                                    |
| BME280                       | Atmospheric sensor: temperature, pressure, humidity |
| MPU-9250/6500                | 9-DOF IMU                                           |
| NEO-6M                       | GPS module                                          |

### Kino 2 

> TBD

## Directory Index

 - `doc/` - Supporting documents, references, & datasheets
 - `hardware/`
   - `cam/` - CAM files for board fabrication  
   - `lib/` - KiCAD library files
 - `software/`
   - `kino_arduino/` - C++ flight software deployed on Arduino Pro Mini
   - `kino_rpi/` - C/C++ flight software deployed on RPI Zero

```
.
├── doc/
├── hardware/
│   ├── cam/
│   └── lib/
├── software/
│   ├── kino_arduino/
│   └── kino_rpi/
├── license
├── readme.md
└── update_index.sh

7 directories, 9 files
```

### What's with the name?

 - Eusebio Kino ([Wikipedia](https://en.wikipedia.org/wiki/Eusebio_Kino)):

    > Kino ... was reportedly an expert astronomer, mathematician and cartographer, who drew the first accurate maps of Pimería Alta, the Gulf of California, and Baja California.