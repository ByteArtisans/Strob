#include "IRremote.h"

decode_results results;

#define IR_RECEIVE_PIN A1



// PWM PIN Led!!!
byte led_red_left = 5;
byte led_blue_left = 0;
byte led_green_left = 0;

byte led_red_right = 11;
byte led_blue_right = 10;
byte led_green_right = 9;



//int buttonPin = 2 NEXT MODE;
//int buttonPin = 4 OFF;
byte val = 0, stope = 1;
long previousMillis = 0;
byte buttonState = 0;
long interval = 300;

byte gl_previous_mode = 1;


byte off_led_num = 11;

int values_l[] = {led_red_left}; // Масив із 4 значень
int values_r[] = {led_blue_right, led_red_right, led_green_right}; // Масив із 4 значень


const int arraySize1 = sizeof(values_l) / sizeof(values_l[0]); // Розрахунок розміру масиву
const int arraySize2 = sizeof(values_r) / sizeof(values_r[0]); // Розрахунок розміру масиву



// Функція, яка повертає випадкове значення з масиву
int getRandomValue_l() {
  int randomIndex = random(0, arraySize1); // Випадковий індекс
  return values_l[randomIndex]; // Повертає значення з масиву
}
int getRandomValue_r() {
  int randomIndex = random(0, arraySize2); // Випадковий індекс
  return values_r[randomIndex]; // Повертає значення з масиву
}


void setup() {
  Serial.begin(9600); // // Establish serial communication

  
  delay(2);
  //IrReceiver.begin(IR_RECEIVE_PIN, ENABLE_LED_FEEDBACK); // Start the receiver
  pinMode(led_red_left, OUTPUT);
  pinMode(led_blue_right, OUTPUT);
  pinMode(led_green_right, OUTPUT);
  pinMode(led_red_right, OUTPUT);
  // pinMode(13, OUTPUT);
  //pinMode (buttonPin, INPUT);

  //pinMode(3, INPUT);

  //digitalWrite(3, LOW);

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
    digitalWrite(led_blue_right, HIGH);
    digitalWrite(led_red_left, LOW);
    delay(300);
    
    digitalWrite(led_blue_right, LOW);
    digitalWrite(led_red_left, HIGH);
    delay(300);
    
    Irrec_func();
  }
  digitalWrite(led_blue_right, LOW);
  digitalWrite(led_red_left, LOW);
}
void two() {
  while (stope == 1) {
    digitalWrite(led_blue_right, HIGH);
    delay(40);
    digitalWrite(led_blue_right, LOW);
    delay(40);
    digitalWrite(led_blue_right, HIGH);
    delay(40);
    digitalWrite(led_blue_right, LOW);
    delay(40);
    digitalWrite(led_blue_right, HIGH);
    delay(40);
    digitalWrite(led_blue_right, LOW);
    delay(40);
    digitalWrite(led_blue_right, HIGH);
    delay(40);
    digitalWrite(led_blue_right, LOW);
    delay(40);
    digitalWrite(led_blue_right, HIGH);
    delay(40);
    digitalWrite(led_blue_right, LOW);
    delay(40);
    digitalWrite(led_red_left, HIGH);
    delay(40);
    digitalWrite(led_red_left, LOW);
    delay(40);
    digitalWrite(led_red_left, HIGH);
    delay(40);
    Irrec_func();
    if (stope != 1) {digitalWrite(led_red_left, LOW); digitalWrite(led_blue_right, LOW); break;}
    digitalWrite(led_red_left, LOW);
    delay(40);
    digitalWrite(led_red_left, HIGH);
    delay(40);
    digitalWrite(led_red_left, LOW);
    delay(40);
    digitalWrite(led_red_left, HIGH);
    delay(40);
    digitalWrite(led_red_left, LOW);
    delay(40);
    digitalWrite(led_red_left, HIGH);
    delay(40);
    digitalWrite(led_red_left, LOW);
    delay(40);
    
    Irrec_func();
    
  }
  digitalWrite(led_blue_right, LOW);
  digitalWrite(led_red_left, LOW);
}
void three() {
  while (stope == 1) {
    digitalWrite(led_blue_right, HIGH);
    digitalWrite(led_red_left, HIGH);
    delay(40);
    digitalWrite(led_blue_right, LOW);
    digitalWrite(led_red_left, LOW);
    delay(40);
    digitalWrite(led_blue_right, HIGH);
    digitalWrite(led_red_left, HIGH);
    delay(40);
    digitalWrite(led_blue_right, LOW);
    digitalWrite(led_red_left, LOW);
    delay(40);
    digitalWrite(led_blue_right, HIGH);
    digitalWrite(led_red_left, HIGH);
    delay(40);
    digitalWrite(led_blue_right, LOW);
    digitalWrite(led_red_left, LOW);
    delay(40);
    digitalWrite(led_blue_right, HIGH);
    digitalWrite(led_red_left, HIGH);
    delay(40);
    Irrec_func();
    if (stope != 1) {digitalWrite(led_red_left, LOW); digitalWrite(led_blue_right, LOW); break;}
    digitalWrite(led_blue_right, LOW);
    digitalWrite(led_red_left, LOW);
    delay(40);
    digitalWrite(led_blue_right, HIGH);
    digitalWrite(led_red_left, HIGH);
    delay(40);
    digitalWrite(led_blue_right, LOW);
    digitalWrite(led_red_left, LOW);
    delay(40);
    digitalWrite(led_blue_right, HIGH);
    digitalWrite(led_red_left, HIGH);
    delay(40);
    digitalWrite(led_blue_right, LOW);
    digitalWrite(led_red_left, LOW);
    Irrec_func();
    if (stope != 1) {digitalWrite(led_red_left, LOW); digitalWrite(led_blue_right, LOW); break;}
    delay(500);
    
    Irrec_func();
  }
  digitalWrite(led_blue_right, LOW);
  digitalWrite(led_red_left, LOW);
}
void smooth() {
  while (stope == 1) {
    for (int jl = 3; jl >= 0; jl--){
      if (jl != 0){

        for (int i = 0; i <= 255; i++) {
          analogWrite(led_blue_right, i);
          delay(jl);
        }
        for (int k = 255; k >= 0; k--) {
          analogWrite(led_blue_right, k);
          delay(jl);
          Irrec_func();
          if (stope != 1) {digitalWrite(led_red_left, LOW); digitalWrite(led_blue_right, LOW); break;}
        }
        for (int i = 0; i <= 255; i++) {
          analogWrite(led_red_left, i);
          delay(jl);
        }
        for (int k = 255; k >= 0; k--) {
          analogWrite(led_red_left, k);
          delay(jl);
          Irrec_func();
          if (stope != 1) {digitalWrite(led_red_left, LOW); digitalWrite(led_blue_right, LOW); break;}
        }
        
        Irrec_func();
        if (stope != 1) {digitalWrite(led_red_left, LOW); digitalWrite(led_blue_right, LOW); break;}
      }else {
        if (stope != 1) {digitalWrite(led_red_left, LOW); digitalWrite(led_blue_right, LOW); break;}
        for (byte f = 0; f <= 3; f++){

          for (int i = 0; i <= 255; i+=2) {
            analogWrite(led_blue_right, i);
            delay(1);
          }
          for (int k = 255; k >= 0; k-=2) {
            analogWrite(led_blue_right, k);
            delay(1);
            Irrec_func();
            if (stope != 1) {digitalWrite(led_red_left, LOW); digitalWrite(led_blue_right, LOW); break;}
          }
          for (int i = 0; i <= 255; i+=2) {
            analogWrite(led_red_left, i);
            delay(1);
          }
          for (int k = 255; k >= 0; k-=2) {
            analogWrite(led_red_left, k);
            delay(1);
            Irrec_func();
            if (stope != 1) {digitalWrite(led_red_left, LOW); digitalWrite(led_blue_right, LOW); break;}
          }
          
          Irrec_func();
          if (stope != 1) {digitalWrite(led_red_left, LOW); digitalWrite(led_blue_right, LOW); break;}
        }
        for (byte n = 2; n <= 5; n++){


          for (int i = 0; i <= 255; i+=3) {
            analogWrite(led_blue_right, i);
            delay(1);
          }
          for (int k = 255; k >= 0; k-=3) {
            analogWrite(led_blue_right, k);
            delay(1);
          }
          for (int i = 0; i <= 255; i+=3) {
            analogWrite(led_red_left, i);
            delay(1);
          }
          for (int k = 255; k >= 0; k-=3) {
            analogWrite(led_red_left, k);
            delay(1);
          }
          
          Irrec_func();
          if (stope != 1) {digitalWrite(led_red_left, LOW); digitalWrite(led_blue_right, LOW); break;}
        }
      }
    }
    
    
    Irrec_func();
  }
}
void smooth2() {
  while (stope == 1) {
    int r = getRandomValue_l();
    int ra = getRandomValue_r();
    for (int i = 0; i <= 255; i++) {
      analogWrite(r, i);
      delay(4);
      Irrec_func();
      if (stope != 1) {digitalWrite(led_red_left, LOW); digitalWrite(led_blue_right, LOW); break;}
    }
    for (int i = 0; i <= 255; i++) {
      analogWrite(ra, i);
      delay(4);
    }
    for (int k = 255; k >= 0; k--) {
      analogWrite(r, k);
      delay(4);
      Irrec_func();
      if (stope != 1) {digitalWrite(led_red_left, LOW); digitalWrite(led_blue_right, LOW); break;}
    }
    for (int k = 255; k >= 0; k--) {
      analogWrite(ra, k);
      delay(4);
    }
    
    Irrec_func();
  }
}
void smooth3() {
  while (stope == 1) {
    for (int i = 0; i <= 255; i++) {
      analogWrite(led_blue_right, i);
      analogWrite(led_red_left, i);
      delay(5);
    }
    Irrec_func();
    if (stope != 1) {digitalWrite(led_red_left, LOW); digitalWrite(led_blue_right, LOW); break;}
    for (int k = 255; k >= 0; k--) {
      analogWrite(led_blue_right, k);
      analogWrite(led_red_left, k);
      delay(5);
    }
    
    Irrec_func();
    if (stope != 1) {digitalWrite(led_red_left, LOW); digitalWrite(led_blue_right, LOW); break;}
  }
}

void two2 () {
  while (stope == 1) {
    digitalWrite(led_blue_right, HIGH);
    Irrec_func();
    if (stope != 1) {digitalWrite(led_red_left, LOW); digitalWrite(led_blue_right, LOW); break;}
    delay(45);
    digitalWrite(led_blue_right, LOW);

    delay(40);
    digitalWrite(led_blue_right, HIGH);

    delay(45);
    digitalWrite(led_blue_right, LOW);

    delay(40);
    digitalWrite(led_blue_right, HIGH);
    Irrec_func();
    if (stope != 1) {digitalWrite(led_red_left, LOW); digitalWrite(led_blue_right, LOW); break;}
    delay(45);
    digitalWrite(led_blue_right, LOW);

    delay(40);
    digitalWrite(led_blue_right, HIGH);

    delay(45);
    digitalWrite(led_blue_right, LOW);

    // delay(40);
    // digitalWrite(led_blue_right, HIGH);
    // delay(40);

    // digitalWrite(led_blue_right, LOW);
    Irrec_func();
    if (stope != 1) {digitalWrite(led_red_left, LOW); digitalWrite(led_blue_right, LOW); break;}

    for (int i = 0; i <= 700; i++){
      
      Irrec_func();
      if (stope != 1) {digitalWrite(led_red_left, LOW); digitalWrite(led_blue_right, LOW); break;}
      delay(1);
    }


    
    Irrec_func();
  }
  digitalWrite(led_blue_right, LOW);
  digitalWrite(led_red_left, LOW);
}


void funk3() {
  // 1. Початкове чергове миготіння
  while (stope == 1) {

  for (int j = 40; j >= 4; j-=4){

  for (int i = 0; i < 5; i++) {
    digitalWrite(led_blue_right, HIGH);
    delay(j);
    digitalWrite(led_blue_right, LOW);
    delay(j);
    Irrec_func();
    if (stope != 1) {digitalWrite(led_red_left, LOW); digitalWrite(led_blue_right, LOW); break;}
  }
  if (stope != 1) {digitalWrite(led_red_left, LOW); digitalWrite(led_blue_right, LOW); break;}

  for (int i = 0; i < 5; i++) {
    digitalWrite(led_red_left, HIGH);
    delay(j);
    digitalWrite(led_red_left, LOW);
    delay(j);
  }
  }
  for (int i = 0; i < 4; i++) {
    if (stope != 1) {digitalWrite(led_red_left, LOW); digitalWrite(led_blue_right, LOW); break;}
    digitalWrite(led_blue_right, HIGH);
    digitalWrite(led_red_left, HIGH);
    delay(40);
    digitalWrite(led_blue_right, LOW);
    digitalWrite(led_red_left, LOW);
    delay(40);
    digitalWrite(led_blue_right, HIGH);
    digitalWrite(led_red_left, HIGH);
    delay(40);
    digitalWrite(led_blue_right, LOW);
    digitalWrite(led_red_left, LOW);
    delay(40);
    digitalWrite(led_blue_right, HIGH);
    digitalWrite(led_red_left, HIGH);
    delay(40);
    digitalWrite(led_blue_right, LOW);
    digitalWrite(led_red_left, LOW);
    delay(40);
    Irrec_func();
    if (stope != 1) {digitalWrite(led_red_left, LOW); digitalWrite(led_blue_right, LOW); break;}
    digitalWrite(led_blue_right, HIGH);
    digitalWrite(led_red_left, HIGH);
    delay(40);
    digitalWrite(led_blue_right, LOW);
    digitalWrite(led_red_left, LOW);
    delay(40);
    digitalWrite(led_blue_right, HIGH);
    digitalWrite(led_red_left, HIGH);
    delay(40);
    digitalWrite(led_blue_right, LOW);
    digitalWrite(led_red_left, LOW);
    delay(40);
    digitalWrite(led_blue_right, HIGH);
    digitalWrite(led_red_left, HIGH);
    delay(40);
    digitalWrite(led_blue_right, LOW);
    digitalWrite(led_red_left, LOW);
    Irrec_func();
    if (stope != 1) {digitalWrite(led_red_left, LOW); digitalWrite(led_blue_right, LOW); break;}
    
    delay(200);

    
    Irrec_func();
    
  }
   
    Irrec_func();
  }
  digitalWrite(led_blue_right, LOW);
  digitalWrite(led_red_left, LOW);
}

void led_on(){
  while (stope == 1) {
      digitalWrite(led_blue_right, HIGH);
      digitalWrite(led_red_left, HIGH);
      digitalWrite(led_green_right, HIGH);

    Irrec_func();
  }
  digitalWrite(led_blue_right, LOW);
  digitalWrite(led_red_left, LOW);
  digitalWrite(led_green_right, LOW);
}

void check_stop() {
  while (stope == 1) {
    for (int i = 0; i <= 255; i+=9) {
      analogWrite(led_red_left, i);
      analogWrite(led_green_right, i);
      analogWrite(led_red_right, i);
      delay(3);
      Irrec_func();
      if (stope != 1) {digitalWrite(led_red_left, LOW); digitalWrite(led_blue_right, LOW); break;}
    }
    digitalWrite(led_red_left, HIGH);
    digitalWrite(led_green_right, HIGH);
    digitalWrite(led_red_right, HIGH);
    Irrec_func();
    if (stope != 1) {digitalWrite(led_red_left, LOW); digitalWrite(led_blue_right, LOW); break;}
    delay(500);
    Irrec_func();
    if (stope != 1) {digitalWrite(led_red_left, LOW); digitalWrite(led_blue_right, LOW); break;}
    for (int k = 255; k >= 0; k-=9) {
      analogWrite(led_red_left, k);
      analogWrite(led_green_right, k);
      analogWrite(led_red_right, k);
      delay(3);
      Irrec_func();
      if (stope != 1) {digitalWrite(led_red_left, LOW); digitalWrite(led_blue_right, LOW); break;}
    }

      
      digitalWrite(led_blue_right, LOW);
      digitalWrite(led_red_left, LOW);
      digitalWrite(led_green_right, LOW);
      digitalWrite(led_red_right, LOW);
      Irrec_func();
      if (stope != 1) {digitalWrite(led_red_left, LOW); digitalWrite(led_blue_right, LOW); break;}



      
      for (int c = 0; c <= 850; c++){
        delay(1);
        Irrec_func();
      if (stope != 1) {digitalWrite(led_red_left, LOW); digitalWrite(led_blue_right, LOW); break;}
      }



    Irrec_func();
    if (stope != 1) {digitalWrite(led_red_left, LOW); digitalWrite(led_blue_right, LOW); break;}
  }
  digitalWrite(led_blue_right, LOW);
  digitalWrite(led_red_left, LOW);
  digitalWrite(led_green_right, LOW);
  digitalWrite(led_red_right, LOW);
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
      funk3();
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
      two();
      break;
    case 9:
      stope = 1;
      led_on();
      break;
    case 10:
      stope = 1;
      check_stop();
      break;
  }
  }

void buttonPin() {
  static unsigned long millis_prev;
  if (millis() - 1000 > millis_prev) {
    stope = 1;
    if (val == 0) val = (gl_previous_mode - 1);
    delay(50);
    val++;
    delay(50);
    stope = 0;
    // Serial.println(val);
    if (val == off_led_num) val = 0;
  }
  millis_prev = millis();
}

void buttonPin2() {
  static unsigned long millis_prev2;
  if (millis() - 1000 > millis_prev2) {
    if (val != 0) gl_previous_mode = val;
    stope = 1;
    delay(50);
    val = 0;
    delay(50);
    stope = 0;
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
      }else if ((IrReceiver.decodedIRData.decodedRawData) == 3108437760){
        if (val != 9){
          Serial.println(9);
          val = 9;
          stope = 0;
        }else{
          if (val != off_led_num){
            gl_previous_mode = val;
          }else if(val == 0){}
          Serial.println(off_led_num);
          val = off_led_num;
          stope = 0;
        }

      }else if ((IrReceiver.decodedIRData.decodedRawData) == 4144561920){
        Serial.println(8);
        val = 10;
        stope = 0;
      }
      else if ((IrReceiver.decodedIRData.decodedRawData) == 3091726080 || (IrReceiver.decodedIRData.decodedRawData) == 2790899456){
        if (val != off_led_num){
        gl_previous_mode = val;
        }else if(val == 0){}
        Serial.println(off_led_num);
        val = off_led_num;
        stope = 0;

        // OFF
      }else if ((IrReceiver.decodedIRData.decodedRawData) == 3125149440 || (IrReceiver.decodedIRData.decodedRawData) == 250724096){
        
        delay(50);
        Serial.println(gl_previous_mode);
        val = gl_previous_mode;

        stope = 0;

        // ON
      }


      IrReceiver.resume();
      // delay(10);
      
  }
}