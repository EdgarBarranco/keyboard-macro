#include <Keypad.h>
#include <EEPROM.h>

byte btn,stSize,led=13;
int addr;
char ans,mtx[16][128];
String cmnd;
const byte ROWS = 4; //four rows
const byte COLS = 4; //four columns
byte keys [ROWS][COLS] = {
  {3,2,1,'0'},
  {7,6,5,4},
  {11,10,9,8},
  {15,14,13,12}
};

byte rowPins[ROWS] = {23, 22, 21, 20}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {19, 18, 17, 16}; //connect to the column pinouts of the keypad

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

void setup(){
  Serial.begin(9600);
  Serial.setTimeout(100);
  if (EEPROM.read(0) == 255)
    firstBoot();
  printMenu();
  loadMTX();
  pinMode(led, OUTPUT);
}

void loop(){
  byte key = keypad.getKey();
  if (key){
    digitalWrite(led, HIGH);
    if (key == 48) key = 0;
    Serial.print("Pressed key: ");
    Serial.println(key,HEX);
//    Serial.print("Macro: ");
//    Serial.println(mtx[key]);
//    Keyboard.print(mtx[key]);
    int count = 0;
    for (int i = 0; i < strlen(mtx[key]); i++) {
      Keyboard.print(mtx[key][count++]);
      delay(100);
    }
    digitalWrite(led, LOW);
  }
  else if (Serial.available()){
    printMenu();
    ans = Serial.read();
    Serial.println(ans);
    if (ans == 'p' || ans == 'P') printEEPROM(16);
    else if (ans == 'w' || ans == 'W') writeEEPROM();
    else if (ans == 'e' || ans == 'E') eraseEEPROM();
  }
}

void firstBoot(){
  for ( unsigned int i = 0 ; i < EEPROM.length() ; i++ )
    EEPROM.write(i, 0);
}

void loadMTX(){
  int i,j;
  for (i = 0;i < 16; i++){
    addr = i*128;
    stSize = EEPROM.read(addr);
    for (j = 0; j<=stSize;j++){
      mtx[i][j]=EEPROM.read(1+addr+j);
    }
    mtx[i][j]=0;
  }
}

void printMenu(){
  Serial.println("\r\nKeyboard macros menu");
  Serial.println("p: Print EEPROM");
  Serial.println("w: Write EEPROM");
  Serial.println("e: Erase EEPROM");
  Serial.print("Input: ");
}

void eraseEEPROM(){
  Serial.print("\n\rErase the eeprom? (y/n): ");
  while (!Serial.available());
  ans = Serial.read();
  Serial.print(ans);
  if (ans == 'y' || ans == 'Y') 
  { 
    Serial.println("\r\nErasing..");
    digitalWrite(led, HIGH);
    for ( unsigned int i = 0 ; i < EEPROM.length() ; i++ )
      EEPROM.write(i, 0);
    digitalWrite(led, LOW);
    Serial.println("Done.");
  }
  else Serial.println("\r\nAborting.");
}

void printEEPROM(byte btn){
  if (btn == 16){ 
    char indx[4];
    for (int i = 0;i < btn ; i++){ 
      Serial.print("Key ");
      sprintf(indx, "%1X:",i);
      Serial.print(indx);
      printEEPROM(i);
      Serial.println();
    }
    Serial.println();
  } else {
    digitalWrite(led, HIGH);
    addr = (btn*128);
    byte stSize = EEPROM.read(addr);
    for (int j=0;j<=stSize;j++){
      if (EEPROM.read(1+addr+j))
        Serial.print((char)EEPROM.read(1+addr+j));
    digitalWrite(led, LOW);
    }
  } 
}

void writeEEPROM(){
  btn = 0;
  byte i=0;
  digitalWrite(led, HIGH);
  Serial.print("Press the button you want to program. ");
  while (!btn){
     btn = keypad.getKey();
  }
  if (btn == 48) btn = 0;
  
  Serial.print(btn,HEX);Serial.print(": [");printEEPROM(btn); Serial.println("]");
  Serial.print("Enter macro (126 characters max): ");
  while (!Serial.available());
  while ((ans != '\n') && (ans != '\r')){
   while (!Serial.available());
   ans = Serial.read();
    if (ans == 127) {
     ans = 8;
    }
    if (ans == 8){
       if (i) {
        cmnd.remove(--i);
        Serial.print((char)ans);
        Serial.print((char)' ');
        Serial.print((char)ans);
       }
    }
    else if ((ans >= 32) && (ans <= 255)) {
         i++;
         Serial.print((char)ans);
         if (cmnd.length() > 125) {
            Serial.println("\r\nSorry you have reached the character limit for this macro.");
            ans = '\r';
         }
         if ((ans != '\n') && (ans != '\r')) cmnd += ans;
    }
  }
  
  Serial.print("\r\nReplacing:\r\n [");printEEPROM(btn);Serial.print("]\r\nWith:\r\n [");Serial.print(cmnd);Serial.println("]");
  Serial.print("Correct? (y/n): ");
  while (!Serial.available());
  ans = Serial.read();
  Serial.println(ans);  
  if (ans == 'y' || ans == 'Y'){ 
   stSize = cmnd.length();
   Serial.print("\n\rLength: ");
   Serial.println(stSize); 
   addr = (btn * 128);
   EEPROM.write(addr,stSize);
   for (i=0;i<=stSize;i++){
    EEPROM.write((1+addr+i),cmnd[i]);
   }
   EEPROM.write((addr + i++),0);
   while (i<128){
    EEPROM.write(addr + i, 0);
    i++;
   }
   Serial.println("Done writing.");
  }
  else Serial.println("Aborting.");
  cmnd="";
  loadMTX();
  digitalWrite(led, LOW);
}

