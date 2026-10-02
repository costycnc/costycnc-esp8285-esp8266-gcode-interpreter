# CostyCNC Wi-Fi G-code controller for ESP8266 / ESP8285

An early CostyCNC experiment that turns an **ESP8266/ESP8285 into a small Wi-Fi CNC motion controller**.

The repository contains the source code, web interface, G-code interpreter, stepper-control code, Potrace-based image tools, measurements, experiments and development notes from 2018.

It is both a historical CostyCNC project and an educational example of how the pieces of a simple CNC controller can be built from scratch.

## What problem does this project solve?

How can an inexpensive ESP8266/ESP8285:

- create its own Wi-Fi network;
- provide a web interface;
- receive files from a phone or computer;
- store G-code in flash;
- interpret G-code commands;
- calculate coordinated X/Y movement;
- drive two unipolar stepper motors;
- display a toolpath in the browser;
- and control a CNC machine without a traditional PC controller?

This repository is an implementation of that idea.

## Basic architecture

**Phone / PC → Wi-Fi → ESP8266/ESP8285 → web interface → G-code → G-code interpreter → step generation → unipolar steppers**

The ESP works in **Access Point mode** and creates a Wi-Fi network named:

`Costycnc`

The browser can communicate directly with the controller.

## Interesting files

### Wi-Fi + web server

[`costycnc.wifi.1.10.ino`](./costycnc.wifi.1.10.ino) contains the main ESP8266 web-controller code.

It uses ESP8266 Wi-Fi, HTTP server functions and SPIFFS flash storage. It implements file upload and routes for operations such as `/upload`, `/fileread`, `/list`, movement commands and toolpath display.

### G-code interpreter

[`process_string.ino`](./process_string.ino) parses G-code and converts coordinates into movement instructions.

It contains handling for:

- G0 rapid movement
- G1 linear movement
- G2 clockwise arcs
- G3 counter-clockwise arcs
- X/Y/Z coordinates
- absolute and incremental positioning
- feedrate
- conversion from machine units to steps

The educational chain is:

**G-code text → coordinates → target position → steps → motor sequence**

### Coordinated stepper movement

[`stepper_control.ino`](./stepper_control.ino) contains the movement logic.

The `dda_move()` function performs coordinated movement so that different axes can progress together.

For example:

`G01 X10 Y5`

is transformed into coordinated X/Y step activity instead of simply moving X and then Y.

The code also contains direct eight-state sequences for the two unipolar motors.

### Direct browser control

[`custom.ino`](./custom.ino) contains simple browser commands such as X+, X-, Y+ and Y-.

A browser action can generate a normal G-code command such as:

`G01 X10 Y0`

and send it through the same G-code processing path.

This is a useful concept:

**a browser button can generate the same movement command used by G-code instead of requiring a separate motor-control protocol.**

### G-code → browser visualization

The project reads `test.nc` and can generate an SVG/polyline representation of the toolpath for display in the browser.

The `image()` function in `custom.ino` is an early example of generating HTML/SVG directly from an embedded controller.

### Potrace and image-to-path experiments

The repository contains JavaScript Potrace code:

- [`toate/Potrace.js`](./toate/Potrace.js)
- [`index/potracex.js`](./index/potracex.js)

and browser tools such as:

- [`toate/potrace.html`](./toate/potrace.html)
- [`upload.html`](./upload.html)

These files show an early attempt to connect image processing, path generation and CNC cutting in a browser-based workflow.

## Educational experiments

The repository is interesting because it preserves the problems encountered during development instead of only showing the final firmware.

### Runtime configuration

The 2018 notes describe moving values such as steps/mm and feedrate from hard-coded source code into `settings.txt`.

This illustrates a practical embedded-software principle:

**configuration does not always have to require recompiling the firmware.**

### A real initialization bug

[`27.10.18_modificari.txt`](./27.10.18_modificari.txt) documents a subtle bug.

`x_units` and `y_units` were initialized from `X_STEPS_PER_MM` and `Y_STEPS_PER_MM` before those values were later changed from the configuration file.

The fix was to update the working values again inside `init_process_string()`.

This is a concrete example of the difference between **initialization time** and **runtime configuration**.

### ESP8266 watchdog

The Arduino test code documents a watchdog-reset problem caused by a long stepper loop.

The solution was to insert:

`yield();`

inside the movement loop.

This is an important ESP8266 lesson: a long CPU-intensive loop must still give the system time to handle background tasks and watchdog requirements.

### Measuring G-code execution time

The `arduino_time/` folder contains an experiment that measures the time required to process individual G-code movements.

[`arduino_time/time_arduino.txt`](./arduino_time/time_arduino.txt) contains real measurements for commands such as:

`G01 X10.51`

and

`G01 X10.51 Y0.5`.

It is useful for studying:

**G-code command → calculation → step generation → execution time**

rather than guessing performance.

### Sending large generated HTML/SVG responses

[`03.12.18_modificari.txt`](./03.12.18_modificari.txt) documents the use of `server.sendContent()` with unknown content length.

The goal was to send larger generated browser content from the ESP8266 without constructing the entire response as one large string.

### Development notes

Other files such as:

- [`26.10.18_modificari.txt`](./26.10.18_modificari.txt)
- [`27.10.18_modificari.txt`](./27.10.18_modificari.txt)
- [`03.12.18_modificari.txt`](./03.12.18_modificari.txt)

preserve the actual development process: configuration changes, debugging, performance problems and implementation decisions.

## Hardware

The original project was developed around:

- ESP8266 / ESP8285
- two unipolar stepper motors
- direct GPIO control
- SPIFFS flash storage
- Wi-Fi Access Point mode

The code contains explicit ESP8266 GPIO mappings and eight-state sequences for the unipolar motors.

**Important:** this is an old experimental project. Treat it as a historical and educational reference rather than as a guaranteed modern drop-in CNC controller.

## Original firmware

A precompiled binary is included:

`costycnc.wifi.1.10.cpp.bin`

The original documentation described flashing it with NodeMCU Flasher and an ESP8285 configuration.

Because ESP8266/ESP8285 board variants and flash layouts differ, verify the hardware and flash configuration before writing a binary.

## Original workflow

1. Flash the ESP8266/ESP8285 firmware.
2. Connect to the `Costycnc` Wi-Fi network.
3. Open the controller in a browser.
4. Upload the required web files to SPIFFS.
5. Upload or select a G-code file such as `test.nc`.
6. Inspect the toolpath in the browser.
7. Start the cut.
8. The ESP parses the G-code and drives the two unipolar motors.

## Why this repository is worth studying

The project shows a CNC controller as a sequence of understandable problems:

**Wi-Fi → web server → file storage → G-code parser → coordinate calculation → DDA movement → unipolar motor sequence → browser visualization**

A good way to study the code is:

1. Start with `custom.ino` to see simple browser movement commands.
2. Read `process_string.ino` to see how G-code becomes coordinates.
3. Read `stepper_control.ino` to see coordinated movement.
4. Read `costycnc.wifi.1.10.ino` to see how Wi-Fi, HTTP and SPIFFS connect everything.
5. Read the `*_modificari.txt` files to see how real problems were found and fixed.

## Historical context

This project dates from the 2018 development period.

It represents an early CostyCNC approach to putting CNC control, web access, file handling and image/toolpath processing together on a small Wi-Fi microcontroller.

The original code is intentionally preserved because the implementation itself is useful for studying how the system was built.

It also shows an early version of an idea later continued in other CostyCNC controller projects:

> A CNC controller can be treated as a small programmable motion computer, not only as a device tied to one specific CNC application.

## Third-party code

This repository contains third-party code and libraries in addition to CostyCNC code. Check individual source files for their original copyright and license information before redistributing modified versions.

## CostyCNC

Created and developed by **Boboaca Costel / CostyCNC**.

Website: https://www.costycnc.it  
YouTube: https://www.youtube.com/@bobyca2003
