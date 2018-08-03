# keyboard-macro

### Teensy keyboard macro

Keyboard macro - It maps 16 buttons `4x4 button matrix` to strings `126 characters long each`.  
The strings are sent to the computer from an USB Keyboard HID. A serial console 9600,8,1,n is created to manage the strings.  
  
To compile it needs the [teensyduino add-on](https://www.pjrc.com/teensy/teensyduino.html)

On the arduino IDE select:
*  Board: "Teensy 3.2/3.1"  
*  USB Type: "Serial + Keyboard + Mouse + Joystick"  
*  CPU Sped: "72 MHz"  
*  Keyaboard Layout: "US English"  <- has only been tested with this layout, others may also work.  
*  Optimize: "Fast"  
*  Port: `Select teensy usb <serial port>`  

Libraries (standard libraries already installed):
*  [Keypad](http://playground.arduino.cc/Code/Keypad)
*  [EEPROM](https://www.arduino.cc/en/Reference/EEPROM)

Pins are configured as follows:
*  Row pins: 23, 22, 21, 20
*  Column pins: 19, 18, 17, 16
 
73's  
