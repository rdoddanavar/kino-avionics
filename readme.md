# kino-avionics

> High Power Rocketry - Electronic Hardware

![](https://img.shields.io/badge/EDA-EAGLE-lightgrey.svg)
![](https://img.shields.io/github/license/rdoddanavar/hpr-hardware.svg)
![](https://img.shields.io/github/repo-size/rdoddanavar/hpr-hardware.svg)

Flight computer to serve data acquisition & tracking needs of a dual-deploy HPR model. Designed around a Raspberry Pi Zero.

## Overview

| Control            | Data Acquisition | Telemetry | Misc.  |
|--------------------|------------------|-----------|--------|
| < >                | 9DOF IMU         | GPS       | Buzzer |
| < >                | GPS              |           |        |
|                    | Pressure         |           |        |
|                    | Temperature      |           |        |
|                    | Humidity         |           |        |

## Project Index

 - `cam/` - All CAM files necessary for board fabrication  
 - `doc/` - Supporting documents & references 
 - `lbr/` - EAGLE additional libraries 

```
.
├── cam/
│   └── placeholder
├── doc/
│   └── honeywell-sensing-trustability-ssc-series-standard-740340.pdf
├── license
├── materials.gdsheet
├── readme.md
└── update_index.sh

2 directories, 6 files
```
