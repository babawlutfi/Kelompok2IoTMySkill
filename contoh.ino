#include <LiquidCrystal_I2C.h>
#include <DHT.h>
#include <DHT_U.h>
// #include "DHT.h"
#include <Servo.h> 
LiquidCrystal_I2C lcd(0x27, 16, 4);

int sensorTanah = A0;

const int MQ135 = A1;
int maxudarabersih = 180;

const int TRIGPIN = 8;          
const int ECHOPIN = 9;
long timer;
int jarak;

DHT dht(2, DHT11);

Servo dataPosisiServo;
int dataPosisi = 00;
int pinServos=3;

int buzzer= 13;

int relayPin = 4;

int kolom = 16;
int baris = 2;

void setup(){
  Serial.begin(9600);
  lcd.begin();
  lcd.backlight();

  pinMode(MQ135, INPUT);

  pinMode(ECHOPIN, INPUT);
  pinMode(TRIGPIN, OUTPUT);

  dht.begin();

  dataPosisiServo.attach(pinServos);

  pinMode(buzzer, OUTPUT);

  pinMode(relayPin, OUTPUT);
  TulisanBergerak(0, "Sabar Baru Buka Coyyy :)", 300, kolom);

  digitalWrite(buzzer, LOW);
}


void TulisanBergerak(int row, String message, int delayTime, int kolom) {
  for (int i=0; i < kolom; i++) {
    message = " " + message;  
  } 
  message = message + " "; 
  for (int pos = 0; pos < message.length(); pos++) {
    lcd.setCursor(0, row);
    lcd.print(message.substring(pos, pos + kolom));
    delay(delayTime);
  }
}

void loop(){


  // start MQ-135
  int dataMQ135 = analogRead(MQ135);
  Serial.print(dataMQ135);
  Serial.print(" ");
  if (dataMQ135 > maxudarabersih){
    Serial.println("Kualitas Udara Tidak Baik");
    Serial.println("");
    digitalWrite(buzzer, HIGH);
    lcd.setCursor(0, 0);
    lcd.print("Udara Terdeteksi");
    lcd.setCursor(0, 1);
    lcd.print("Tidak Sehat");
    delay(1000);
    lcd.clear();
    }
  else {
    Serial.println("Kualitas Udara Baik");
    Serial.println("");
    digitalWrite(buzzer, LOW);
    lcd.setCursor(0, 0);
    lcd.print("Udara Terdeteksi");
    lcd.setCursor(0, 1);
    lcd.print("Baik");
    delay(1000);
    lcd.clear();
  }
  // end MQ-135

  // start PIR
    digitalWrite(TRIGPIN, LOW);                   
    delayMicroseconds(2);
    digitalWrite(TRIGPIN, HIGH);                  
    delayMicroseconds(10);
    digitalWrite(TRIGPIN, LOW);                   

    timer = pulseIn(ECHOPIN, HIGH);
    jarak = timer/58;
    delay(1000);

    Serial.print("Jarak = ");
    Serial.print(jarak);
    Serial.print(" cm");
    Serial.println();
    
  //   if (dataPIR == HIGH){                     
  //   Serial.println("Buka Portal !!!");
  //   for(dataPosisi = 80; dataPosisi>=1; dataPosisi-=1) 
  //   {                              
  //    dataPosisiServo.write(dataPosisi);
  //   }
  //   for(dataPosisi = 00; dataPosisi < 80; dataPosisi += 1) 
  //   {
  //    dataPosisiServo.write(dataPosisi);
  //    delay(30);                                         
  //   } 
  //   TulisanBergerak(1, "Anda Pendatang ke "+String(dataPIRs), 200, kolom);
  // }
  //   else{                 
  //   Serial.println("Tutup Portal !!!");   
  // }
  // end PIR

  // start sensor tanah
  float kelembabanTanah;
  int dataSensorTanah = analogRead(sensorTanah);
  kelembabanTanah = 100.0 - ((dataSensorTanah / 1023.0) * 100.0);

  Serial.print("Persentase Kelembaban Tanah = ");
  Serial.print(kelembabanTanah);
  Serial.println("%");

  if (kelembabanTanah < 9.00) {
    digitalWrite(relayPin, LOW);
    lcd.clear();
  } else {
    digitalWrite(relayPin, HIGH);
  }
  lcd.setCursor(0, 0);
  lcd.print("Kelembaban Tanah");
  lcd.setCursor(4, 1);
  lcd.print(String(kelembabanTanah)+"%");
  delay(1000);
  
  // end sensor tanah

  // start sensor dht 11
  
  float dataKelembabanDHT = dht.readHumidity();
  float dataSuhuDHT = dht.readTemperature();

  if (isnan(dataKelembabanDHT) || isnan(dataSuhuDHT)) {
    Serial.println(F("sensor DHT Error :)"));
    lcd.setCursor(0, 0);
    lcd.print("sensor DHT Error :)");
    delay(1000);
    lcd.clear();
    return;
  }

  Serial.print("Kelembaban: ");
  Serial.println(dataKelembabanDHT);
  Serial.print("Suhu: ");
  Serial.println(dataSuhuDHT);
  if(dataSuhuDHT>35.00){
    lcd.setCursor(0, 0);
    lcd.print("Panassss");
    lcd.setCursor(0, 1);
    lcd.print("Ati Ati Kebakaran");
    delay(3000);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Kelambaban U: "+String(dataKelembabanDHT)+"%");
    lcd.setCursor(0, 1);
    lcd.print("Suhu: "+ String(dataSuhuDHT) + " %");
    digitalWrite(buzzer, HIGH);
    delay(1000);
    lcd.clear();
  }else{
    lcd.setCursor(0, 0);
    lcd.print("Kelambaban U: "+String(dataKelembabanDHT)+"%");
    lcd.setCursor(0, 1);
    lcd.print("Suhu: "+ String(dataSuhuDHT) + " %");
    digitalWrite(buzzer, LOW);
    delay(1000);
    lcd.clear();
  }
  // end sensor dht 11

}
