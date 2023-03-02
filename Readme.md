# Paragliding Variometer on Basis of Bikepilo2+ or Falk Tiger evo.
|         AutoRunCE XCSoar             |    AutoRunCE Main Dialog             |    AutoRunCE Vario Settings          |
|--------------------------------------|--------------------------------------|--------------------------------------|
|[<img src="./image/AutoRunCE_1.png">](https://youtube.com/shorts/pAdE8dn8gpc)|[<img src="./image/AutoRunCE_2.png">](https://youtube.com/shorts/2ggxs6w_VHQ)|[<img src="./image/AutoRunCE_3.png">](https://youtube.com/shorts/l3dTKEIAxWs)|

## Build Step 1
build serial variometer and upload firmware
|                             |                                                                                                   |
|-----------------------------|---------------------------------------------------------------------------------------------------|
|Build informations           |[./ATMega328P-AU/Readme.md](./ATMega328P-AU/Readme.md)                                             |
|Firmware with bootloader     |[./Firmware/SerialVariometer.with_bootloader.hex](./Firmware/SerialVariometer.with_bootloader.hex) |
|Firmware without bootloader  |[./Firmware/SerialVariometer.hex](./Firmware/SerialVariometer.hex)                                 |

## Build Step 2
Open the BikePilot2+ and Install Serilal Variometer.
|                             |                                              |
|-----------------------------|----------------------------------------------|
|Vario                        |<img src="./image/Vario.png">                 |

## Build Step 3
Prepare the USB stick and insert the device (BikePilot2+).
|                                                                                        |
|----------------------------------------------------------------------------------------|
|copy [./Firmware/AutoRunCE.exe](./Firmware/AutoRunCE.exe) into root folder from USB stick |
|copy [./Firmware/AutoRunCE.ini](./Firmware/AutoRunCE.ini) into root folder from USB stick |
|copy [./Firmware/XCSoar.exe](./Firmware/XCSoar.exe) or www.xcsoar.org/Downloads into XCSoarData folder from USB stick |

|                                         |                                               |
|-----------------------------------------|-----------------------------------------------|
|First  NMEA port / GPS signal|XCSoar gets GPS signal from COM0: (Bikepilot internal GPS).|
|Second NMEA port / pressure |AutoRunCE blocks serial variometer port (COM4 ) and forwards it to XCSoar via UDP broadcast on port 8882, BlueFly protocol should be used here.|

## done
