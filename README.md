# mameduino

Arcade controls for the MAME cabinate at MakeICT :)


This software is written for the MakeICT Arcade Machine, which uses two Arduino Leonardo boards to convert button presses into USB keyboard codes. Both Leonardo boards plug into the Arcade PC's USB ports, effectively giving the PC two keyboards.

Leonardo 1 handles player 1 button presses, while Leonardo 2 handles player 2.

The Arduinos are used as a temporary solution to allow the machine to function while working on developing a system that uses a matrix system for button inputs, or a multiplex system with a 4067 IC, or something similar. Of course, like all temporary solutions, these Arduinos will be in place indefinitely.
