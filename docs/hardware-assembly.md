## Hardware Design and Assembly

The objective of this project is to build a handheld device that simulates a marble maze toy with an LED matrix and an accelerometer. The LED matrix will display a virtual representation of a maze and the user will tilt the entire matrix to guide a virtual marble through the maze to the goal. Key objectives for the hardware design are:

1. The device is self-contained and portable - untethered by power cables, computing happens in the device
2. The device can measure changes in its physical orientation as a user input
3. The device responds quickly to user input - for example, if tilted to the right, the virtual marble promptly rolls to the right on-matrix

To meet the first objective, the device must have a small, lightweight microcontroller or computer on-board, and be battery-powered. To meet the second objective, the device needs a sensor to measure changes in its orientation, such as an accelerometer. Finally, the computing and communication between the processor and the display must happen quickly enough for a smooth user experience. 

The major hardware components for marble maze are:

* A **64x64 RGB LED Matrix** to serve as the game's display
* A **LIS3DH 3-Axis Accelerometer** to detect rotation and tilting of the device during gameplay
* A **Raspberry Pi Zero W** to sample accelerometer data, do the computation, and drive the LED matrix
* An **Adafruit RGB Matrix HAT + RTC** to interface the RGB Matrix with the Raspberry Pi
* A **PowerBoost 1000 Charger** to serve as a rechargeable, portable power supply for the handheld device 
* Two **pushbuttons**, to reset the game or change game modes, and
* a **Lithium Ion battery pack**.

<img src="https://i.imgur.com/2IJf5Z0.jpg" width="600">

## Assembly
