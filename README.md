# hpr-hardware

`High Power Rocketry - Electronic Hardware`

![](https://img.shields.io/badge/EDA-EAGLE-lightgrey.svg)
![](https://img.shields.io/github/license/rdoddanavar/hpr-hardware.svg)
![](https://img.shields.io/github/repo-size/rdoddanavar/hpr-hardware.svg)

## FS-1 Kino ![](https://img.shields.io/static/v1.svg?label=status&message=initial%20dev&color=red&style=for-the-badge)

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

## GS-1 Gila ![](https://img.shields.io/static/v1.svg?label=status&message=N/A&color=black&style=for-the-badge)

### Purpose

Ground station to provide remote launch and weather logging capabilities.

### Features

< >

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
