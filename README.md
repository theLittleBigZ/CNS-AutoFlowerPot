# CNS-AutoFlowerPot
ESP8266 controlled robotic flower pot developed by the Code Ninjas STEM CLUB

### Requirements
In order to work on this project you'll need the following:
- ESP8266 (ESP32 can be used as well)
- Submersible water pump
- Capacitive soil moisture sensor
- Single relay (CW-019 / KY-019)
- wires
- Flower pot
- Arduino IDE

### Setup 
For this project we used Arduino IDE as our Editor, when editing this project you can use whatever editor you prefer the following will be a walk through on how to setup Arduino IDE to work with our mircocontroller.

1. Install [Arduino IDE](https://www.arduino.cc/en/software)
2. Install [CH340 Drivers](https://www.wch-ic.com/downloads/CH341SER_ZIP.html)
3. Open Arduino IDE
4. Click on File tab and than open preferences
5. In the additional Boards Manager URLs add the following link [http://arduino.esp8266.com/stable/package_esp8266com_index.json](http://arduino.esp8266.com/stable/package_esp8266com_index.json)
6. Goto Tools>Borads>Boards Manager
7. Connect the ESP9266 to you're computer via USB
8. In arduino IDE goto tools>Boards>select NODEMCU 1.0 (ESP - 12E Module)
9. With the editor open the Sketch
10. update lines 5,6,9,10 accordingly
