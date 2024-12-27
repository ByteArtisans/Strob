#include "IRremote.h"

decode_results results;

#define IR_RECEIVE_PIN 11


byte led1 = 6;
byte led2 = 5;
//int buttonPin = 2 NEXT MODE;
//int buttonPin = 4 OFF;
byte val = 0, stope = 1;
long previousMillis = 0;
byte buttonState = 0;
long interval = 300;

byte gl_previous_mode = 1;


void setup() {
  Serial.begin(9600); // // Establish serial communication

  
  delay(2);
  //IrReceiver.begin(IR_RECEIVE_PIN, ENABLE_LED_FEEDBACK); // Start the receiver
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(13, OUTPUT);
  //pinMode (buttonPin, INPUT);

  pinMode(4, INPUT);

  digitalWrite(3, LOW);

  attachInterrupt(0, buttonPin, FALLING);
  attachInterrupt(digitalPinToInterrupt(3), buttonPin2, RISING);

  // attachInterrupt(0, ser, FALLING);

  IrReceiver.begin(IR_RECEIVE_PIN, ENABLE_LED_FEEDBACK); // Start the receiver
  while (!Serial)
    ;
}

void one() {
  while (stope == 1) {
    // Serial.println(val);
    digitalWrite(led1, HIGH);
    digitalWrite(led2, LOW);
    delay(300);
    
    digitalWrite(led1, LOW);
    digitalWrite(led2, HIGH);
    delay(300);
    
    Irrec_func();
  }
  digitalWrite(led1, LOW);
  digitalWrite(led2, LOW);
}
void two() {
  while (stope == 1) {
    digitalWrite(led1, HIGH);
    delay(40);
    digitalWrite(led1, LOW);
    delay(40);
    digitalWrite(led1, HIGH);
    delay(40);
    digitalWrite(led1, LOW);
    delay(40);
    digitalWrite(led1, HIGH);
    delay(40);
    digitalWrite(led1, LOW);
    delay(40);
    digitalWrite(led1, HIGH);
    delay(40);
    digitalWrite(led1, LOW);
    delay(40);
    digitalWrite(led1, HIGH);
    delay(40);
    digitalWrite(led1, LOW);
    delay(40);
    digitalWrite(led2, HIGH);
    delay(40);
    digitalWrite(led2, LOW);
    delay(40);
    digitalWrite(led2, HIGH);
    delay(40);
    Irrec_func();
    if (stope != 1) {digitalWrite(led2, LOW); digitalWrite(led1, LOW); break;}
    digitalWrite(led2, LOW);
    delay(40);
    digitalWrite(led2, HIGH);
    delay(40);
    digitalWrite(led2, LOW);
    delay(40);
    digitalWrite(led2, HIGH);
    delay(40);
    digitalWrite(led2, LOW);
    delay(40);
    digitalWrite(led2, HIGH);
    delay(40);
    digitalWrite(led2, LOW);
    delay(40);
    
    Irrec_func();
    
  }
  digitalWrite(led1, LOW);
  digitalWrite(led2, LOW);
}
void three() {
  while (stope == 1) {
    digitalWrite(led1, HIGH);
    digitalWrite(led2, HIGH);
    delay(40);
    digitalWrite(led1, LOW);
    digitalWrite(led2, LOW);
    delay(40);
    digitalWrite(led1, HIGH);
    digitalWrite(led2, HIGH);
    delay(40);
    digitalWrite(led1, LOW);
    digitalWrite(led2, LOW);
    delay(40);
    digitalWrite(led1, HIGH);
    digitalWrite(led2, HIGH);
    delay(40);
    digitalWrite(led1, LOW);
    digitalWrite(led2, LOW);
    delay(40);
    digitalWrite(led1, HIGH);
    digitalWrite(led2, HIGH);
    delay(40);
    Irrec_func();
    if (stope != 1) {digitalWrite(led2, LOW); digitalWrite(led1, LOW); break;}
    digitalWrite(led1, LOW);
    digitalWrite(led2, LOW);
    delay(40);
    digitalWrite(led1, HIGH);
    digitalWrite(led2, HIGH);
    delay(40);
    digitalWrite(led1, LOW);
    digitalWrite(led2, LOW);
    delay(40);
    digitalWrite(led1, HIGH);
    digitalWrite(led2, HIGH);
    delay(40);
    digitalWrite(led1, LOW);
    digitalWrite(led2, LOW);
    Irrec_func();
    if (stope != 1) {digitalWrite(led2, LOW); digitalWrite(led1, LOW); break;}
    delay(500);
    
    Irrec_func();
  }
  digitalWrite(led1, LOW);
  digitalWrite(led2, LOW);
}
void smooth() {
  while (stope == 1) {
    for (int jl = 3; jl >= 0; jl--){
      if (jl != 0){

        for (int i = 0; i <= 255; i++) {
          analogWrite(led1, i);
          delay(jl);
        }
        for (int k = 255; k >= 0; k--) {
          analogWrite(led1, k);
          delay(jl);
          Irrec_func();
          if (stope != 1) {digitalWrite(led2, LOW); digitalWrite(led1, LOW); break;}
        }
        for (int i = 0; i <= 255; i++) {
          analogWrite(led2, i);
          delay(jl);
        }
        for (int k = 255; k >= 0; k--) {
          analogWrite(led2, k);
          delay(jl);
          Irrec_func();
          if (stope != 1) {digitalWrite(led2, LOW); digitalWrite(led1, LOW); break;}
        }
        
        Irrec_func();
        if (stope != 1) {digitalWrite(led2, LOW); digitalWrite(led1, LOW); break;}
      }else {
        if (stope != 1) {digitalWrite(led2, LOW); digitalWrite(led1, LOW); break;}
        for (byte f = 0; f <= 3; f++){

          for (int i = 0; i <= 255; i+=2) {
            analogWrite(led1, i);
            delay(1);
          }
          for (int k = 255; k >= 0; k-=2) {
            analogWrite(led1, k);
            delay(1);
            Irrec_func();
            if (stope != 1) {digitalWrite(led2, LOW); digitalWrite(led1, LOW); break;}
          }
          for (int i = 0; i <= 255; i+=2) {
            analogWrite(led2, i);
            delay(1);
          }
          for (int k = 255; k >= 0; k-=2) {
            analogWrite(led2, k);
            delay(1);
            Irrec_func();
            if (stope != 1) {digitalWrite(led2, LOW); digitalWrite(led1, LOW); break;}
          }
          
          Irrec_func();
          if (stope != 1) {digitalWrite(led2, LOW); digitalWrite(led1, LOW); break;}
        }
        for (byte n = 2; n <= 5; n++){


          for (int i = 0; i <= 255; i+=3) {
            analogWrite(led1, i);
            delay(1);
          }
          for (int k = 255; k >= 0; k-=3) {
            analogWrite(led1, k);
            delay(1);
          }
          for (int i = 0; i <= 255; i+=3) {
            analogWrite(led2, i);
            delay(1);
          }
          for (int k = 255; k >= 0; k-=3) {
            analogWrite(led2, k);
            delay(1);
          }
          
          Irrec_func();
          if (stope != 1) {digitalWrite(led2, LOW); digitalWrite(led1, LOW); break;}
        }
      }
    }
    
    
    Irrec_func();
  }
}
void smooth2() {
  while (stope == 1) {
    for (int i = 0; i <= 255; i++) {
      analogWrite(led1, i);
      delay(4);
      Irrec_func();
      if (stope != 1) {digitalWrite(led2, LOW); digitalWrite(led1, LOW); break;}
    }
    for (int i = 0; i <= 255; i++) {
      analogWrite(led2, i);
      delay(4);
    }
    for (int k = 255; k >= 0; k--) {
      analogWrite(led1, k);
      delay(4);
      Irrec_func();
      if (stope != 1) {digitalWrite(led2, LOW); digitalWrite(led1, LOW); break;}
    }
    for (int k = 255; k >= 0; k--) {
      analogWrite(led2, k);
      delay(4);
    }
    
    Irrec_func();
  }
}
void smooth3() {
  while (stope == 1) {
    for (int i = 0; i <= 255; i++) {
      analogWrite(led1, i);
      analogWrite(led2, i);
      delay(5);
    }
    Irrec_func();
    if (stope != 1) {digitalWrite(led2, LOW); digitalWrite(led1, LOW); break;}
    for (int k = 255; k >= 0; k--) {
      analogWrite(led1, k);
      analogWrite(led2, k);
      delay(5);
    }
    
    Irrec_func();
    if (stope != 1) {digitalWrite(led2, LOW); digitalWrite(led1, LOW); break;}
  }
}

void two2 () {
  while (stope == 1) {
    digitalWrite(led1, HIGH);
    Irrec_func();
    if (stope != 1) {digitalWrite(led2, LOW); digitalWrite(led1, LOW); break;}
    delay(40);
    digitalWrite(led1, LOW);

    delay(40);
    digitalWrite(led1, HIGH);

    delay(40);
    digitalWrite(led1, LOW);

    delay(40);
    digitalWrite(led1, HIGH);
    Irrec_func();
    if (stope != 1) {digitalWrite(led2, LOW); digitalWrite(led1, LOW); break;}
    delay(40);
    digitalWrite(led1, LOW);

    delay(40);
    digitalWrite(led1, HIGH);

    delay(40);
    digitalWrite(led1, LOW);

    delay(40);
    digitalWrite(led1, HIGH);
    delay(40);

    digitalWrite(led1, LOW);
    Irrec_func();
    if (stope != 1) {digitalWrite(led2, LOW); digitalWrite(led1, LOW); break;}

    for (int i = 0; i <= 560; i++){
      
      Irrec_func();
      if (stope != 1) {digitalWrite(led2, LOW); digitalWrite(led1, LOW); break;}
      delay(1);
    }


    
    Irrec_func();
  }
  digitalWrite(led1, LOW);
  digitalWrite(led2, LOW);
}


void funk3() {
  // 1. Початкове чергове миготіння
  while (stope == 1) {

  for (int j = 40; j >= 4; j-=4){

  for (int i = 0; i < 5; i++) {
    digitalWrite(led1, HIGH);
    delay(j);
    digitalWrite(led1, LOW);
    delay(j);
    Irrec_func();
    if (stope != 1) {digitalWrite(led2, LOW); digitalWrite(led1, LOW); break;}
  }
  if (stope != 1) {digitalWrite(led2, LOW); digitalWrite(led1, LOW); break;}

  for (int i = 0; i < 5; i++) {
    digitalWrite(led2, HIGH);
    delay(j);
    digitalWrite(led2, LOW);
    delay(j);
  }
  }
  for (int i = 0; i < 4; i++) {
    if (stope != 1) {digitalWrite(led2, LOW); digitalWrite(led1, LOW); break;}
    digitalWrite(led1, HIGH);
    digitalWrite(led2, HIGH);
    delay(40);
    digitalWrite(led1, LOW);
    digitalWrite(led2, LOW);
    delay(40);
    digitalWrite(led1, HIGH);
    digitalWrite(led2, HIGH);
    delay(40);
    digitalWrite(led1, LOW);
    digitalWrite(led2, LOW);
    delay(40);
    digitalWrite(led1, HIGH);
    digitalWrite(led2, HIGH);
    delay(40);
    digitalWrite(led1, LOW);
    digitalWrite(led2, LOW);
    delay(40);
    Irrec_func();
    if (stope != 1) {digitalWrite(led2, LOW); digitalWrite(led1, LOW); break;}
    digitalWrite(led1, HIGH);
    digitalWrite(led2, HIGH);
    delay(40);
    digitalWrite(led1, LOW);
    digitalWrite(led2, LOW);
    delay(40);
    digitalWrite(led1, HIGH);
    digitalWrite(led2, HIGH);
    delay(40);
    digitalWrite(led1, LOW);
    digitalWrite(led2, LOW);
    delay(40);
    digitalWrite(led1, HIGH);
    digitalWrite(led2, HIGH);
    delay(40);
    digitalWrite(led1, LOW);
    digitalWrite(led2, LOW);
    Irrec_func();
    if (stope != 1) {digitalWrite(led2, LOW); digitalWrite(led1, LOW); break;}
    
    delay(200);

    
    Irrec_func();
    
  }
   
    Irrec_func();
  }
  digitalWrite(led1, LOW);
  digitalWrite(led2, LOW);
}


void loop() {

  

  Irrec_func();
  
  
  if (Serial.available() ) { val = Serial.parseInt();
    Serial.println(val);
    // stope = 0;
    // Serial.println(Serial.read());
    
    
  }
  switch (val) {
    case 1:
      stope = 1;
      two2();
      break;
    case 2:
      stope = 1;
      two();
      break;
    case 3:
      stope = 1;
      three();
      break;
    case 4:
      stope = 1;
      smooth();
      break;
    case 5:
      stope = 1;
      smooth2();
      break;
    case 6:
      stope = 1;
      one();
      break;
    case 7:
      stope = 1;
      smooth3();
      break;
    case 8:
    stope = 1;
    funk3();
    break;
  }
}
void buttonPin() {
  static unsigned long millis_prev;
  if (millis() - 1000 > millis_prev) {
    stope = 1;
    delay(50);
    val++;
    delay(50);
    stope = 0;
    // Serial.println(val);
    if (val == 9) val = 0;
  }
  millis_prev = millis();
}

void buttonPin2() {
  static unsigned long millis_prev2;
  if (millis() - 1000 > millis_prev2) {
    stope = 1;
    delay(50);
    val = 0;
    delay(50);
    stope = 0;
    // Serial.println(val);
    if (val == 9) val = 0;
  }
  millis_prev2 = millis();
}

void Irrec_func () {

  
  
  if (Serial.available() > 0) stope = Serial.parseInt();
  if (IrReceiver.decode()) {
      //Serial.println(IrReceiver.decodedIRData.decodedRawData); // Print "old" raw data
      
      // Serial.println(IrReceiver.decodedIRData.decodedRawData);
      

      if ((IrReceiver.decodedIRData.decodedRawData) == 3141861120 || (IrReceiver.decodedIRData.decodedRawData) == 2907881216){
          Serial.print(1);
          val = 1;
          stope = 0;
      }else if ((IrReceiver.decodedIRData.decodedRawData) == 3158572800 || (IrReceiver.decodedIRData.decodedRawData) == 2941304576){
        Serial.print(2);
        val = 2;
        stope = 0;
      }else if ((IrReceiver.decodedIRData.decodedRawData) == 4161273600 || (IrReceiver.decodedIRData.decodedRawData) == 4010852096){
        Serial.print(3);
        val = 3;
        stope = 0;
      }else if ((IrReceiver.decodedIRData.decodedRawData) == 4127850240 || (IrReceiver.decodedIRData.decodedRawData) == 2841034496){
        Serial.println(4);
        val = 4;
        stope = 0;
      }else if ((IrReceiver.decodedIRData.decodedRawData) == 3910598400 || (IrReceiver.decodedIRData.decodedRawData) == 2874457856){
        Serial.println(5);
        val = 5;
        stope = 0;
      }else if ((IrReceiver.decodedIRData.decodedRawData) == 4061003520 || (IrReceiver.decodedIRData.decodedRawData) == 3944005376){
        Serial.println(6);
        val = 6;
        stope = 0;
      }else if ((IrReceiver.decodedIRData.decodedRawData) == 4077715200 || (IrReceiver.decodedIRData.decodedRawData) == 2974727936){
        Serial.println(7);
        val = 7;
        stope = 0;
      }else if ((IrReceiver.decodedIRData.decodedRawData) == 2707357440 || (IrReceiver.decodedIRData.decodedRawData) == 3008151296){
        Serial.println(8);
        val = 8;
        stope = 0;
      }
      
      
      else if ((IrReceiver.decodedIRData.decodedRawData) == 3091726080 || (IrReceiver.decodedIRData.decodedRawData) == 2790899456){
        if (val != 9){
        gl_previous_mode = val;
        }else if(val == 0){}
        Serial.println(9);
        val = 9;
        stope = 0;
      }else if ((IrReceiver.decodedIRData.decodedRawData) == 3125149440 || (IrReceiver.decodedIRData.decodedRawData) == 250724096){
        
        delay(50);
        Serial.println(gl_previous_mode);
        val = gl_previous_mode;
        stope = 0;
      }


      IrReceiver.resume();
      // delay(10);
      
  }
}