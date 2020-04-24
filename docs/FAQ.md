![alt text][logo]

## What printers can I use with *Drunken Octopus*?

If you are savvy with electrical and mechanical things, *Drunken Octopus* will give you the software to match any hardware upgrades you make to your printer.

Many of the following combinations of electronics and toolheads have been tried either at LulzBot R&D or by LulzBot employees on their own printers, so any of these choices are within the realm of the possible!

Printer Chassis and Electronics (your pick!)                                  | Toolhead (your pick!)
------------------------------------------------------------------------------|---------------------------
Mini with original electronics with or w/o B&W display and SD card  :factory: | Single Extruder for Mini :factory:
Mini with Einsy Retro with or w/o B&W display                                 | Flexystruder for Mini :raising_hand:
Mini with original electronics, Color Touch Screen and SD card :raising_hand: | Aerostruder for Mini 2 :factory:
Mini with Einsy Retro and Color Touch Screen and SD card       :raising_hand: | Single Extruder for TAZ :factory:
Mini 2 with original electronics, B&W display and SD card           :factory: | Flexystruder for TAZ :raising_hand:
Mini 2 with original electronics, Color Touch Screen and SD or USB            | MOARstruder for TAZ :raising_hand:
Bio with original electronics, Color Touch Screen and SD card       :factory: | Goostruder for Bio :factory:
TAZ 5 with original electronics                                     :factory: | FlexyDually for TAZ :raising_hand:
TAZ 5 with original electronics and BLTouch                                   | DualExtruder V2 for TAZ :raising_hand:
TAZ 5 with Archim 2.0 upgrade and Filament Sensor                             | DualExtruder V3 for TAZ
TAZ 5 with Archim 2.0 upgrade, Filament Sensor and BLTouch                    | Universal Adapter for Mini :raising_hand:
TAZ 6 with original electronics                                     :factory: | Universal Adapter for TAZ :raising_hand:
TAZ 6 with original electronics and BLTouch                                   | Universal SL 0.25mm :raising_hand:
TAZ 6 with Archim 2.0 upgrade and Filament Sensor                             | Universal SE 0.5mm :factory:
TAZ 6 with Archim 2.0 upgrade, Filament Sensor and BLTouch                    | Universal HS 0.8mm :raising_hand:
TAZ Workhorse with original electronics                             :factory: | Universal HS+ 1.2mm :raising_hand:
TAZ Workhorse with original electronics and BLTouch                           | 
TAZ Workhorse with Archim 2.0 and Filament Sensor                             | 
TAZ Workhorse with Archim 2.0 upgrade, Filament Sensor and BLTouch            |
TAZ Pro with Color Touch Screen, USB Flashdrive and Filament Sensor :factory: | 

:factory: = Standard/stock configuration
:raising_hand: = No longer being provided as binaries; available upon request

## How can you contribute to *Drunken Octopus*?

Aside from contributing at my [Patreon page], I could use some help with development and testing. I no longer have the resources of the company to help me test firmware; all I have is an Einsy Retro upgraded first-generation Mini and a TAZ 5. Because of this, *Drunken Octopus* will need to be a collaborative effort to survive.

Here's how you can participate:

- If you have a LulzBot printer and some toolheads, you can become a tester!
- If you are experienced with Marlin, you can help triage issues and build config files!
- If you are experienced with slicers, you can develop start and end GCODE and help others with Cura or other slicers!
- If you have coding skills, you can help by squashing bugs or building binaries!

## How does *Drunken Octopus* differ from LulzBot Marlin?

To simplify the code, a few things have changed:

- I have removed or replaced LulzBot logos to prevent possible trademark complications.
- After changing the Z offset you must now save it using the save settings menu item, for consistency with other settings.
- I have simplified firmware file names for ease of understanding.
- You need to tell Cura to connect to unknown printers, as Cura LE does not know about *Drunken Octopus*.
- The M115 output is now unmodified from the standard Marlin response.
- I have removed EMI mitigation code to simplify endstop and probe troubleshooting.
- All Mini firmware now outputs LCD data, regardless of whether one is hooked up.
- You can build it on a Windows 10 PC using *Windows Subsystem for Linux* (in addition to the Arduino IDE or standalone Linux)

## How does *Drunken Octopus* differ from Marlin 2.0 upstream?

Hopefully it differs as little as possible. *Drunken Octopus* will be where I first implement new features before sending them upstream for incorporation. *Drunken Octopus* also includes source and build scripts for making configuration files for all the varieties of firmware.

## Will using *Drunken Octopus* void my printer warranty?

Probably, if you let LulzBot know about it.

## Will *Drunken Octopus* damage my printer?

He doesn't mean to, but it depends on how much he has had to drink the night before! **Use *Drunken Octopus* at your own risk.**

## What if I want my printer to remain sober?

Reach out to [LulzBot] and use their firmware. Rumor has it they are still answering the phone.

## Will *Drunken Octopus* binaries and pre-build configuration files be provided?

To get you started with *Drunken Octopus*, pre-built binaries and configs will be provided for the **stock toolhead** for each **standard LulzBot printer** (as indicated by :factory: above). Check the [releases page](https://github.com/marciot/drunken-octopus-marlin/releases) on GitHub.

Patreon supporters will receive firmware or configuration files for modified printers and accessory toolheads (and possibly other variations, as time goes on!). These will also be in the releases page, in the form of a password protected file, sometime after the basic firmware is released. Patreon supporters will be notified of the password once these releases are made.

Source code for building everything will always be available to anyone!

## What pins are used on a RAMBO for extra features?

| Feature                | Port  | Arduino Pin | Rambo Pin  | Description                  |
|------------------------|-------|-------------|------------|------------------------------|
| FIL_RUNOUT_PIN         | PE3   | 5           | X34 Pin 5  | Filament Runout 1            |
| FIL_RUNOUT2_PIN        | PE3   | 5           | X34 Pin 5  | Filament Runout 2            |
| SERVO0_PIN             | PA0   | 22          | MX1 Pin 3  | Bed Washers or BLTouch Servo |

There is only one available pin for filament runout sensing on the Rambo. Use normally
open microswitches for sensing filament on dual toolheads and wire them in series.

## What pin are used when upgrading a TAZ to an Archim 2.0?

| Feature                | Port  | Arduino Pin | Archim Pin | Description                  |
|------------------------|-------|-------------|------------|------------------------------|
| NEOPIXEL_PIN           | PB5   | 94          | J20 Pin 5  | Unused                       |
| -                      | PB0   | 95          | J20 Pin 6  | Unused                       |
| -                      | PB3   | 103         | J20 Pin 7  | Unused                       |
| FIL_RUNOUT_PIN         | PB15  | 66          | J20 Pin 15 | Filament Runout 1            |
| FIL_RUNOUT2_PIN        | PB16  | 67          | J20 Pin 16 | Filament Runout 2            |
| SERVO0_PIN             | PB12  | 20          | J20 Pin 20 | Bed Washers or BLTouch Servo |
| SERVO1_PIN             | PB13  | 21          | J20 Pin 19 | Unused                       |

## How do I compile *Drunken Octopus* from source?

**Windows 10:** If you are using Windows 10, one of the easiest ways to build the firmware is using *Windows Subsystem for Linux (WSL)*. Simply follow [this guide] to set up WSL and get yourself an Ubuntu shell, then proceed with the steps for Linux.

**Linux:** Open a shell and execute the following commands one at a time:

```
sudo apt-get update
sudo apt-get install python git make rename
sudo apt-get install gcc-avr avr-libc
sudo apt-get install gcc-arm-none-eabi
sudo ln -s "/mnt/c/Users/YOUR_WINDOWS_USER_DIR/Documents" Documents
cd Documents
git clone https://github.com/marciot/drunken-octopus-marlin.git marlin
cd marlin
./build-configs.sh
./build-firmware.sh
```

Once this process is complete (it may take several hours), the firmware files will be in the `build` directory of your Documents folder.

**Arduino IDE:** In order to use the Arduino IDE, you will need download and extract the zip file from the [GitHub repo].

The, go into the unpacked folder and replace the "Configuration.h" and "Configuration_adv.h" files in the "Marlin" subdirectory with one of the example configuration files from "config/examples/AlephObjects". *Drunken Octopus* will only include the configuration files for the standard printers and toolheads.

Open the "Marlin.ino" file from the "Marlin" subdirectory in the Arduino IDE.

Choose "Preferences" from the "File" menu and add "https://raw.githubusercontent.com/ultimachine/ArduinoAddons/master/package_ultimachine_index.json" to the "Additional Boards Manager URLs".

In the "Boards Manager":
    - For TAZ Pro: Search for "Archim" and install "Archim by UltiMachine"
    - For all others: Search for "RAMBo" and install "RepRap Arduino-compatible Mother Board (RAMBo) by UltiMachine"

Choose the board corresponding to your printer from the "Board" submenu menu of the "Tools" menu. For a TAZ Pro, select "Archim"; for all others, select "RAMBo"

To compile and upload the firmware to your printer, select "Upload" from the "Sketch" menu.

## Where can I find parts to upgrade my printer?

Einsy Retro and Archim 2.0 boards are available from [ultimachine]. Although I believe ultimachine was manufacturing all boards for LulzBot, I do not know whether they have any remaining stock to sell. If you want to make your own, here are the open-source files for the PCBs:

Source Files for PCB design                                 | Component
------------------------------------------------------------|-------------------------
[Frying Pan](https://code.alephobjects.com/source/aotctl/)  | Color touch display
[Green](https://code.alephobjects.com/diffusion/GREEN/)     | USB reader
[Hidden](https://code.alephobjects.com/diffusion/HIDDEN/)   | SD reader
[Kettle](https://code.alephobjects.com/source/kettle/)      | Filament sensor
[Dolores](https://code.alephobjects.com/diffusion/DOLORES/) | Brake board
[Elk](https://code.alephobjects.com/diffusion/ELK/)         | Alternative B&W LCD panel
[Illinois](https://code.alephobjects.com/source/illinois/)  | TAZ Pro dual extruder

## License

Marlin is published under the [GPL license](/LICENSE) because we believe in open development. The GPL comes with both rights and obligations. Whether you use Marlin firmware as the driver for your open or closed-source product, you must keep Marlin open, and you must provide your compatible Marlin source code to end users upon request. The most straightforward way to comply with the Marlin license is to make a fork of Marlin on Github, perform your modifications, and direct users to your modified fork.

While we can't prevent the use of this code in products (3D printers, CNC, etc.) that are closed source or crippled by a patent, we would prefer that you choose another firmware or, better yet, make your own.

[logo]: https://github.com/marciot/drunken-octopus-marlin/raw/master/images/drunk-octopus-lcd-2.bmp "Drunken Octopus Logo"
[Patreon page]: https://www.patreon.com/marciot "Marcio's Patreon Page"
[ultimachine]: https://ultimachine.com/
[this guide]: https://www.howtogeek.com/249966/how-to-install-and-use-the-linux-bash-shell-on-windows-10/
[GitHub repo]: https://github.com/marciot/drunken-octopus-marlin