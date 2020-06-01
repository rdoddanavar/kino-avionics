# hpr-hardware

> High Power Rocketry - Electronic Hardware

![](https://img.shields.io/badge/EDA-EAGLE-lightgrey.svg)
![](https://img.shields.io/github/license/rdoddanavar/hpr-hardware.svg)
![](https://img.shields.io/github/repo-size/rdoddanavar/hpr-hardware.svg)

## Kino

### Purpose

Flight computer to serve needs of a dual-deploy HPR model. Intended as a [Stratologger](http://www.perfectflite.com/SLCF.html) replacement with additional DAQ capabilities. Designed around a Raspberry Pi Zero.

### Features

| Control            | Data Acquisition | Telemetry | Misc.  |
|--------------------|------------------|-----------|--------|
| Main deploy pyro   | 9DOF IMU         | GPS       | Buzzer |
| Drogue deploy pyro | GPS              |           |        |
|                    | Pressure         |           |        |
|                    | Temperature      |           |        |
|                    | Humidity         |           |        |

## Project Index

 - `./*/cam/` - All CAM files necessary for board fabrication  
 - `./*/doc/` - Supporting documents & references 
 - `./*/lbr/` - EAGLE additional libraries 

```
.
├── fs1_kino/
│   ├── cam/
│   ├── doc/
│   ├── lbr/
│   └── fs1_kino.sch
├── gs1_gila/
├── README.md
└── update_index.sh*

5 directories, 3 files
```
