///////////////////////
//   tw_seer67875   //
//////////////////////
#include <U8g2lib.h>
#include <Adafruit_NeoPixel.h>
#include <LedControl.h>
#include <ArduinoSTL.h>
#include <vector>

//init MAX7219 LedControl obj
LedControl lc = LedControl(12,11,10,4);

U8G2_SH1106_128X64_NONAME_1_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE);

//init strip obj
Adafruit_NeoPixel strip(8, 6, NEO_GRB + NEO_KHZ800);

//DEBUG MODE
boolean DebugMod = false;

//Controller DEBUG String
char ControllerDebugUp[] = "UP";
char ControllerDebugDown[] = "DOWN";
char ControllerDebugLeft[] = "LEFT";
char ControllerDebugRight[] = "RIGHT";

//Controller Play Icon
char ControllerUp[] = "\u0047";
char ControllerDown[] = "\u0044";
char ControllerLeft[] = "\u0046";
char ControllerRight[] = "\u0045";

int8_t ControllerLiveState = 0;

//Controller Value
int16_t ControllerXValue = 0;
int16_t ControllerYValue = 0;

//Controller SW pin
int8_t ControllerSWPin = 7;

//Debug led pin
int8_t DebugLedPin = 2;

//Snake score & record
short score = 0;
short highest_record = 0;

struct food {
  int8_t piece;
  int8_t x;
  int8_t z; 
} randomFood;

struct snake {
  int8_t piece;
  int8_t x;
  int8_t z; 
};

std::vector<snake> snakePathSet;

void setup() {
  Serial.begin(9600);
  initPinMode();
  initOLEDDisplay();
  initRGBLed();
  initSnakeGame();
  lc.setLed(randomFood.piece, randomFood.x, randomFood.z, true);
  ControllerLiveState = getPS2Controller();
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

  moveSnake();
  drawSnake();

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
    drawDisplayFont(0, 40, "Controller: : ", getControllerDebugString(ControllerLiveState));
    drawDisplayFont(0, 50, "ControllerValueX: ", String(ControllerXValue));
    drawDisplayFont(0, 60, "ControllerValueY: ", String(ControllerYValue));
    return;
  }

  drawDisplayFont(66, 10, "DEBUG: ", "OFF");
  u8g2.setFont(u8g2_font_open_iconic_arrow_4x_t);
  drawDisplayFont(45, 60, getControllerIconString(ControllerLiveState), "");

  u8g2.sendBuffer();
}

//draw display font function
void drawDisplayFont(byte drawX, byte drawY, String message, String value){
  u8g2.setCursor(drawX, drawY);
  u8g2.print(message);
  u8g2.print(value);
}

//return Controller state
int8_t getPS2Controller(){
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
const char* getControllerDebugString(int8_t ControllerState){
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
const char* getControllerIconString(int8_t ControllerState){
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

void initLEDMatrix(){
  for(int8_t i = 0; i != 4; i++){
    lc.shutdown(i, false);
    lc.setIntensity(i, 8);
    lc.clearDisplay(i);
  }
}

//init SnakeGame log
void initSnakeGame(){
  initLEDMatrix();
  
  delay(100);
  
  initRandomFood();

  struct snake snakePath1 = {1, 3, 5};
  struct snake snakePath2 = {1, 3, 4};
  struct snake snakePath3 = {1, 3, 3};

  snakePathSet.push_back(snakePath1);
  snakePathSet.push_back(snakePath2);
  snakePathSet.push_back(snakePath3);

}

void initRandomFood(){
  randomSeed(analogRead(A3));
  randomFood.piece = random(4);
  randomFood.x = random(8);
  randomFood.z = random(8);
}

void drawSnake(){
  for(int8_t i = 0; i != 4; i++){
    lc.clearDisplay(i);
  }
  for (auto const& snake_loc : snakePathSet) {
    lc.setLed(snake_loc.piece, snake_loc.x, snake_loc.z, true);
  }
  lc.setLed(randomFood.piece, randomFood.x, randomFood.z, true);
}

void moveSnake(){
  moveSnakeHead();
  snakePathSet.erase(snakePathSet.begin());
}

void moveSnakeHead(){
  int8_t piece = snakePathSet.back().piece;
  int8_t x = snakePathSet.back().x;
  int8_t z = snakePathSet.back().z;
  
  if(randomFood.piece == piece && randomFood.x == x && randomFood.z == z){
    initRandomFood();
    score = score + 1;
    initSankeTmp(piece, x ,z);
  }

  switch(ControllerLiveState){
    case 1:{
      x = x - 1;
      if(x == -1){
        x = 7;
      }
      initSankeTmp(piece, x, z);
      break;
    }
    case 2:{
      x = x + 1;
      if(x == 8){
        x = 0;
      }
      initSankeTmp(piece, x, z);
      break;
    }
    case 3:{
      z = z - 1;
      if(z == -1){
        piece = piece + 1;
        if(piece == 4){
            piece = 0;
        }
        z = 7;
      }
      initSankeTmp(piece, x, z);
      break;
    }
    case 4:{
      z = z + 1;
      if(z == 8){
        piece = piece - 1;
        if(piece == -1){
          piece = 3;
        }
        z = 0;
      }
      initSankeTmp(piece, x, z);
      break;
    }
  }
}

snake initSankeTmp(int piece, int x, int z){
    snake tmp;
    tmp.piece = piece;
    tmp.x = x;
    tmp.z = z;
    snakePathSet.push_back(tmp);
}