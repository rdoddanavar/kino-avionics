# Raspberry Pi Setup

> Raspberry Pi Zero W Rev 1.1 (9000c1)

## OS Install

https://projects.raspberrypi.org/en/projects/raspberry-pi-setting-up/2

1. Get `Raspberry Pi Imager` application
2. Flash SD card w/ `Raspberry Pi OS Lite (32-bit)`

## Networking setup

https://www.raspberrypi.org/documentation/remote-access/ssh/
https://www.raspberrypi.org/documentation/configuration/wireless/wireless-cli.md

1. Create empty `ssh` file on `boot/` partition of SD card (`touch ssh`)
2. On `rootfs/` partition, enter network details
   - Modify `/etc/wpa_supplicant/wpa_supplicant.conf`
   - Add following to bottom of file:
```
network={
    ssid="<network-name>"
    psk="<password>"
}
```

3. Choose `2.4 GHz` network; `5GHz` not supported on RPI Zero W
   - Localization details only needed for `5 GHz`; ignore this
4. Insert SD card, boot; reboot to finish setup

## Network Access

https://www.raspberrypi.org/documentation/remote-access/ssh/unix.md
https://www.raspberrypi.org/documentation/remote-access/ip-address.md

 - SSH access: `ssh pi@<ip-address>` or `ssh pi@raspberrypi.local`
   - Default password: `raspberry`
 - Check connection: `ping <ip-address>` or `ping raspberrypi.local`
```
PING raspberrypi.local (192.168.0.33) 56(84) bytes of data.
64 bytes from 192.168.0.33 (192.168.0.33): icmp_seq=1 ttl=64 time=11.4 ms
...
```
 - Scan for RPI's IP address: `sudo nmap -sn 192.168.1.0/24`
```
...
Nmap scan report for raspberrypi (192.168.1.8)
Host is up (0.0030s latency).
...
```

## Environment Setup

1. Install programs (`sudo apt install`)
   - `vim`
   - `git`
   - `cmake`
2. Clone git repository
   - `git clone https://github.com/rdoddanavar/kino-avionics.git`
3. Clone git submodules
   - Within submodule directory: `git submodule init && git submodule update`
4. Setup git authentication 
   - `git config --global user.name "<username>"`
   - `git config --global user.email <email@email.com>`
   - `git config credential.helper store`
   - Enter personal access token generated on GitHub as password