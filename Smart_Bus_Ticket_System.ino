#include <SPI.h>
#include <MFRC522.h>
#include <Servo.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);

Servo servo;
Servo servo1;
int servoPos = 0;

#define RST_PIN 9
#define SS_PIN 10

int card1Balance = 2000;
int card2Balance = 400;

#define num 4
char Data[num];
byte data_count = 0;

String num1, num2, card, card2;
int count=4;

const int startState=2;
const int stopState=6;
const int buzzerPin=4;
const int in1=14;
const int in2=15;
bool startFlag,stopFlag;

MFRC522 mfrc522(SS_PIN, RST_PIN);

int state = 0,state1=0;

void setup () {
 lcd.init();
  lcd.backlight();
  Serial.begin(9600);
  servo.attach(5);
  servo1.attach(3);
  servo.write(10);
  delay(100);
  servo1.write(10);
  delay(100);
  pinMode(startState,INPUT);
  pinMode(stopState,INPUT);
  digitalWrite(startState,HIGH);
  digitalWrite(stopState,HIGH);
  pinMode(buzzerPin, OUTPUT);
  pinMode(in1,OUTPUT);
  pinMode(in2,OUTPUT);
  digitalWrite(in1,LOW);
  digitalWrite(in2,LOW);
  SPI.begin();
  mfrc522.PCD_Init();
  lcd.setCursor(0, 0);
  lcd.print(" Automatic BUS");
  lcd.setCursor(0, 1);
  lcd.print("Ticket system");
  delay(3000);
  lcd.clear();
}
void loop()
{

    lcd.setCursor(0, 0);
    lcd.print("Seat Available");
    lcd.setCursor(0,1);
    lcd.print(count);
    if(count>0){
      rfid();
      }
    startFlag=digitalRead(startState);
    delay(100);
    stopFlag=digitalRead(stopState);
    delay(100);
    if(startFlag==LOW){
      Serial.println("Start");
      servoClose();
      startBus();
      delay(3000);
      }
    if(stopFlag==LOW ){
      Serial.println("Stop");
      stopBus();
      if(count>0){
          servoOpen();
          delay(3000);
        }
        else{
        servo1.write(90);
        servo.write(10);
        delay(3000);
          }
      }  
  }


void servoOpen()
{
    servo.write(90);
    delay(20);
    servo1.write(90);
    delay(20);
}

void servoClose()
{
    servo.write(10);
    delay(20);
    servo1.write(10);
    delay(20);
}



void rfid()
{
  if ( ! mfrc522.PICC_IsNewCardPresent())
  {
    return;
  }
  if ( ! mfrc522.PICC_ReadCardSerial())
  {
    return;
  }

  String content = "";
  for (byte i = 0; i < mfrc522.uid.size; i++)
  {
    content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
    content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  content.toUpperCase();

  if (content.substring(1) == "83 DC F2 1A")
  {
    if (card1Balance >= 10)
    {
      lcdPrint();
      card1Balance = card1Balance - 50;
      lcd.setCursor(9, 1);
      lcd.print(card1Balance);
      delay(2000);
      lcd.clear();
      if(count>0){
      count=count-1;
      }
      state = 1;
    }
  }
  else{
    state = 0;
    }
  if (content.substring(1) == "94 C9 1A 72")
  {
    if (card2Balance >= 10)
    {
      lcdPrint();
      card2Balance = card2Balance - 10;
      lcd.setCursor(9, 1);
      lcd.print(card2Balance);
      delay(2000);
      lcd.clear();
      if(count>0){
      count=count-1;
      }
      state1 = 1;
    }
  }

  else {
    state1 = 0;
  }
}



void lcdPrint()
{
  digitalWrite(buzzerPin, HIGH);
  delay(200);
  digitalWrite(buzzerPin, LOW);
  delay(100);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("  Successfully");
  lcd.setCursor(0, 1);
  lcd.print(" paid your bill");
  delay(1500);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Your Remaining");
  lcd.setCursor(0, 1);
  lcd.print("balance: ");
}

void startBus(){
  delay(2000);
  digitalWrite(in1,LOW);
  digitalWrite(in2,HIGH);  
  }

void stopBus(){
  delay(2000);
  digitalWrite(in1,LOW);
  digitalWrite(in2,LOW);
  }