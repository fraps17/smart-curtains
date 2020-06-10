# Smart Curtains

Simple D.i.Y. smart curtains system, using a motor to drive the curtain over the rail, controlled by an Arduino.
Command to open/close the curtain is sent through web api to a Raspberry PI server, which will forward the command to the Arduino via radio (using the NFR24L01+ transceiver).

## Wiring diagram

![wiring diagram](https://github.com/fraps17/smart-curtains/blob/master/wiring_bb.png?raw=true)

## Components

 - Arduino (Nano or Uno)
 - Raspberry PI
 - NFR24L01+ Transceivers
 - L298N Motor Controller
 - Motor

## TODO:
 - Frontend
 - Alexa Skills