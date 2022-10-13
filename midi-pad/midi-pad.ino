// Set board to teensy 3.1/3.2 and USB mode to MIDI
#include <Keypad.h>

byte led = 13;
const int channel = 1;
const int vel = 100;
const int offset = 36;

const byte ROWS = 4; //four rows
const byte COLS = 4; //four columns
byte keys [ROWS][COLS] = { {15,14,13,12},{11,10,9,8},{7,6,5,4},{3,2,1,0}};

byte rowPins[ROWS] = {23, 22, 21, 20}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {19, 18, 17, 16}; //connect to the column pinouts of the keypad

Keypad kpd = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

void setup(){
 kpd.setDebounceTime(20);
 pinMode(led, OUTPUT);
}

void loop(){
 if (kpd.getKeys()) {
    digitalWrite(led, HIGH);
    for (int i=0; i<LIST_MAX; i++) {
      if ( kpd.key[i].stateChanged ) {
        switch (kpd.key[i].kstate) {
            case PRESSED:
                usbMIDI.sendNoteOn((offset+kpd.key[i].kchar), vel, channel);
                break;
            case HOLD: 
                break;
            case RELEASED:
                usbMIDI.sendNoteOff((offset+kpd.key[i].kchar), vel, channel);
                break;
            case IDLE: ;
        }
      }
    }
  }
  digitalWrite(led, LOW);
  while (usbMIDI.read());
}
