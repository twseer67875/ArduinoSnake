

uint16_t ControllerXValue = 0;
uint16_t ControllerYValue = 0;

void setup() {
  Serial.begin(9600);
}

void loop() {
  Serial.println(getPS2Controller());
}


byte getPS2Controller(){
  ControllerXValue = analogRead(0);
  ControllerYValue = analogRead(1);

  if (ControllerXValue < 400) {
    //Serial.println("Left");
    return 1;
  }
  if (ControllerXValue > 600) {
    //Serial.println("Right");
    return 2;
  }

  if (ControllerYValue < 400) {
    //Serial.println("Up");
    return 3;
  }
  
  if (ControllerYValue > 600) {
    //Serial.println("Down");
    return 4;
  }
}