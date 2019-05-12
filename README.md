# hpr-hardware

`High Power Rocketry - Flight Hardware`

## DL-1 Pima

### Purpose

Flight computer to serve needs of a small (L1-L2) HPR model. Intended as a Stratologger replacement with additional DAQ capabilities. Designed around a Raspberry Pi Zero.

### Features

| Control            | Data Acquisition | Telemetry | Misc.  |
|--------------------|------------------|-----------|--------|
| Main deploy pyro   | 9DOF IMU         | GPS       | Buzzer |
| Drogue deploy pyro | GPS              |           |        |
|                    | Pressure         |           |        |
|                    | Temperature      |           |        |
|                    | Humidity         |           |        |

### Hardware

< >

### Software

< >

## Project Index

 - `./*/cam/` - All CAM files necessary for board fabrication  
 - `./*/doc/` - Supporting documents & references 
 - `./*/lbr/` - EAGLE additional libraries 

```
.
├── dl1_pima
│   ├── cam
│   │   └── placeholder
│   ├── doc
│   │   └── dl1_materials.ods
│   ├── lbr
│   │   ├── raspberrypi.lbr
│   │   └── rpi-zero.lbr
│   ├── dl1_pima.s#1
│   ├── dl1_pima.sch
│   └── dl1_pima_snap.scr
├── README.md
└── update_index.sh

4 directories, 9 files
```
