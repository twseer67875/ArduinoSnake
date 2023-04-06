///////////////////////
//   tw_seer67875   //
//////////////////////
#include <U8g2lib.h>
#include <Adafruit_NeoPixel.h>

U8G2_SH1106_128X64_NONAME_1_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE);

//init strip obj
Adafruit_NeoPixel strip(8, 6, NEO_GRB + NEO_KHZ800);

//DEBUG MODE
boolean DebugMod = false;

//Controller DEBUG String
const char* ControllerDebugUp = "UP";
const char* ControllerDebugDown = "DOWN";
const char* ControllerDebugLeft = "LEFT";
const char* ControllerDebugRight = "RIGHT";

//Controller Play Icon
const char* ControllerUp = "\u0047";
const char* ControllerDown = "\u0044";
const char* ControllerLeft = "\u0046";
const char* ControllerRight = "\u0045";

//Controller Value
uint16_t ControllerXValue = 0;
uint16_t ControllerYValue = 0;

//Controller SW pin
short ControllerSWPin = 7;

//Debug led pin
short DebugLedPin = 2;

//Snake score & record
short score = 0;
short highest_record = 0;

void setup() {
  initPinMode();
  initOLEDDisplay();
  initRGBLed();
}

void loop() {
  if(digitalRead(7) == 1){
    DebugMod = !DebugMod;
    if(DebugMod){
      digitalWrite(DebugLedPin, HIGH);
    }
    else{
      digitalWrite(DebugLedPin, LOW);
    }
  }

  drawScoreboard();
}

//init all in & out pin
void initPinMode(){
  pinMode(ControllerSWPin, OUTPUT);
  pinMode(DebugLedPin, OUTPUT);
}

//init OLED display
void initOLEDDisplay(){
  u8g2.setColorIndex(1);
  u8g2.begin();
  u8g2.enableUTF8Print();
}

//init RGB Led
void initRGBLed(){
  strip.begin();
  strip.show();
}

//draw OLED display
void drawScoreboard(){
  u8g2.firstPage();
  do {
    draw();
  } while (u8g2.nextPage());
}

//draw OLED display logic part 
void draw(){
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_helvB08_tr);
  drawDisplayFont(0, 10, "Score: ", String(score));
  drawDisplayFont(0, 20, "Record: : ", String(highest_record));

  if(DebugMod){
    drawDisplayFont(70, 10, "DEBUG: ", "ON");
    drawDisplayFont(0, 30, "Controller Details", "");
    drawDisplayFont(0, 40, "Controller: : ", getControllerDebugString(getPS2Controller()));
    drawDisplayFont(0, 50, "ControllerValueX: ", String(ControllerXValue));
    drawDisplayFont(0, 60, "ControllerValueY: ", String(ControllerYValue));
    return;
  }

  drawDisplayFont(66, 10, "DEBUG: ", "OFF");
  u8g2.setFont(u8g2_font_open_iconic_arrow_4x_t);
  drawDisplayFont(45, 60, getControllerIconString(getPS2Controller()), "");

  u8g2.sendBuffer();
}

//draw display font function
void drawDisplayFont(byte drawX, byte drawY, String message, String value){
  u8g2.setCursor(drawX, drawY);
  u8g2.print(message);
  u8g2.print(value);
}

//return Controller state
byte getPS2Controller(){
  ControllerXValue = analogRead(0);
  ControllerYValue = analogRead(1);
  if(ControllerXValue < 400) {
    return 1;
  }
  if(ControllerXValue > 600) {
    return 2;
  }
  if(ControllerYValue < 400) {
    return 3;
  }
  if(ControllerYValue > 600) {
    return 4;
  }
}

//return Controller Debug direction message
const char* getControllerDebugString(byte ControllerState){
  switch(ControllerState){
    case 1:{
      return ControllerDebugRight;
      break;
    }
    case 2:{
      return ControllerDebugLeft;
      break;
    }
    case 3:{
      return ControllerDebugUp;
      break;
    }
    case 4:{
      return ControllerDebugDown;
      break;
    }
  }
}

//return Controller direction icon message
const char* getControllerIconString(byte ControllerState){
  switch(ControllerState){
    case 1:{
      return ControllerLeft;
      break;
    }
    case 2:{
      return ControllerRight;
      break;
    }
    case 3:{
      return ControllerUp;
      break;
    }
    case 4:{
      return ControllerDown;
      break;
    }
  }
}