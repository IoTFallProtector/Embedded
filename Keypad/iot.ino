#include <SoftwareSerial.h>
#include "Keypad.h"

const byte ROWS = 4;
const byte COLS = 3;

char hexaKeys[ROWS][COLS] = {
  {'#', '0', '*'}, 
  {'9', '8', '7'}, 
  {'6', '5', '4'}, 
  {'3', '2', '1'}, 
};

byte rowPins[ROWS] = {4, 9, 8, 6};
byte colPins[COLS] = {5, 3, 7};

Keypad custom = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);

SoftwareSerial bluetooth(12, 13);

// 상황 발생 확인 LED 스트립 포트
const int initiateLED = 0;

// DC 모터 포트
const int motorCW = 2; // black
const int motorCCW = 1; // red

// 부저 포트
const int buzzer = 10; // PWM

// 입력 버퍼
String buffer = "";

// 수신한 임시 비밀번호
String password = "";

// 기존 비밀번호
String defaultPW = "1379";



void setup() {
  //Serial.begin(9600);
  bluetooth.begin(9600);
  while (!Serial) {};
  //Serial.println("Serial Connected");
  while (!bluetooth) {};
  //Serial.println("BT Connected");

  pinMode(initiateLED, OUTPUT);
  pinMode(motorCW, OUTPUT);
  pinMode(motorCCW, OUTPUT);
  pinMode(buzzer, OUTPUT);

  digitalWrite(initiateLED, 0);
  digitalWrite(motorCW, 0);
  digitalWrite(motorCCW, 0);
  digitalWrite(buzzer, 0);
}

void loop() {
  // 사고 발생 전

  while(1) {
    if (bluetooth.available()) {
      while(bluetooth.available()) {
        char tempC = bluetooth.read();
        password += tempC;
        delay(5);
      }

      password.trim();

      if (password.toInt() >= 0 && password.toInt() <= 9999) { // 사고 발생(임시 비밀번호 발급) 시 break
        digitalWrite(initiateLED, 1); // LED 스트립 on

        analogWrite(buzzer, 127);
        delay(70);
        analogWrite(buzzer, 0);
        delay(50);
        analogWrite(buzzer, 127);
        delay(70);
        analogWrite(buzzer, 0);

        break;
      }
    }

    

    char customKey = custom.getKey();

    if (customKey) {
      
      buffer += customKey;
      analogWrite(buzzer, 127);
      delay(80);
      analogWrite(buzzer, 0);
    }

    if (buffer.length() == 4) { // 비밀번호 4개를 모두 친 경우
      if ((buffer == defaultPW)) { // correct
        bluetooth.write("Open");

        digitalWrite(motorCW, 1);
        digitalWrite(motorCCW, 0);

        delay(750);

        digitalWrite(motorCW, 0);

        analogWrite(buzzer, 127);
        delay(70);
        analogWrite(buzzer, 0);
        delay(50);
        analogWrite(buzzer, 127);
        delay(70);
        analogWrite(buzzer, 0);



        delay(5000);

        digitalWrite(motorCW, 0);
        digitalWrite(motorCCW, 1);

        delay(750);

        digitalWrite(motorCCW, 0);
      }

      else { // failed
        analogWrite(buzzer, 127);
        delay(500);
        analogWrite(buzzer, 0);
      }

      buffer = "";
    }
  }

  

  // 사고 발생 이후
  
  

  while(1) {
    char customKey = custom.getKey();

    if (customKey) {
      buffer += customKey;
      analogWrite(buzzer, 127);
      delay(80);
      analogWrite(buzzer, 0);
    }

    if (buffer.length() == 4) { // 비밀번호 4개를 모두 친 경우
      if ((buffer == password) || (buffer == defaultPW)) {
        bluetooth.write("Open");

        digitalWrite(motorCW, 1);
        digitalWrite(motorCCW, 0);

        delay(750);

        digitalWrite(motorCW, 0);

        analogWrite(buzzer, 127);
        delay(70);
        analogWrite(buzzer, 0);
        delay(50);
        analogWrite(buzzer, 127);
        delay(70);
        analogWrite(buzzer, 0);



        delay(5000);

        digitalWrite(motorCW, 0);
        digitalWrite(motorCCW, 1);

        delay(750);

        digitalWrite(motorCCW, 0);

        buffer = "";
        break;
      }

      else {
        analogWrite(buzzer, 127);
        delay(500);
        analogWrite(buzzer, 0);
        buffer = "";
      }
    }
  }
}

