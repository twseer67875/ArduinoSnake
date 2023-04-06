#include <U8g2lib.h>

U8G2_SH1106_128X64_NONAME_1_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE);

//DEBUG MODE
boolean DebugMod = false;

//Controller DEBUG String
const char* ControllerUp = "UP";
const char* ControllerDown = "DOWN";
const char* ControllerLeft = "LEFT";
const char* ControllerRight = "RIGHT";

//Controller Value
uint16_t ControllerXValue = 0;
uint16_t ControllerYValue = 0;

//Snake score & record
short score = 0;
short highest_record = 0;

void setup() {
  pinMode(7, OUTPUT);
  initOLEDDisplay();
}

void loop() {
  if(digitalRead(7) == 1){
    DebugMod = !DebugMod;
  }

  drawScoreboard();
}

void initOLEDDisplay(){
  u8g2.setColorIndex(2);
  u8g2.setFont(u8g2_font_helvB08_tr);
  u8g2.begin();
  u8g2.enableUTF8Print();
}

void drawScoreboard(){
  u8g2.firstPage();
  do {
    draw();
  } while (u8g2.nextPage());
}

void draw(){
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_helvB08_tr);
  drawDisplayFont(0, 10, "Score: ", String(score));
  drawDisplayFont(0, 20, "Record: : ", String(highest_record));

  if(DebugMod){
    drawDisplayFont(70, 10, "DEBUG_ON", "");
    drawDisplayFont(0, 30, "Controller: : ", getControllerDebugString(getPS2Controller()));
    drawDisplayFont(0, 40, "ControllerValueX: : ", String(ControllerXValue));
    drawDisplayFont(0, 50, "ControllerValueY: : ", String(ControllerYValue));
    return;
  }

  drawDisplayFont(66, 10, "DEBUG_OFF", "");
  u8g2.sendBuffer();
}

void drawDisplayFont(byte drawX, byte drawY, String message, String value){
  u8g2.setCursor(drawX, drawY);
  u8g2.print(message);
  u8g2.print(value);
}

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

const char* getControllerDebugString(byte ControllerState){
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
