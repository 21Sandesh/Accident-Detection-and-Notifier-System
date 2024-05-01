#include "MPU6050.h"
#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#include "I2Cdev.h"

// MPU6050 SDA - A5, SCL - A4

//-------------------//
#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
#include "Wire.h"
#endif
MPU6050 accelgyro;
//-------------------//

String Link = "https://www.google.com/maps/search/?api=1&query=";
String message = "IMPORTANT - A vechicle Accident has been detected, please try to contact the driver\n\nLocation - ";
String sandesh = "+917755966877";
String onkar = "+917666746731";
String yashraj = "+919524121111";
String vijaysir = "+918605619177";

//-------------------//
int16_t ax, ay, az;
int16_t gx, gy, gz;
//-------------------//

#define OUTPUT_READABLE_ACCELGYRO

static const int RXPin = 2, TXPin = 3;  // RX to Tx and Tx go Rx // gps
static const uint32_t Baud = 9600;


// The TinyGPS++ object
TinyGPSPlus GPS;

// arduino pin 2 to Tx of gps and arduino pin 3 to Rx of GPS
SoftwareSerial gps(RXPin, TXPin);
SoftwareSerial SIM900(9, 10);  //Blue - GSM(RXD) - Arduino(9), Purple - GSM(TXD) - Arduino(10)
boolean activate;

//--------------------------//

void setup() {
  //-------------------//
  /*pinMode(3, OUTPUT);
  digitalWrite(3, LOW);
  pinMode(4, OUTPUT);

  pinMode(5, OUTPUT);
  digitalWrite(5, LOW);
  pinMode(6, OUTPUT);*/

  pinMode(7, OUTPUT);
  digitalWrite(7, LOW);
  pinMode(8, OUTPUT);

  //-------------------//

  Serial.begin(Baud);
  gps.begin(Baud);
  SIM900.begin(Baud);

  //-------------------//

#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
  Wire.begin();
#elif I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE
  Fastwire::setup(400, true);
#endif

  //-------------------//

  Serial.println("Initializing I2C devices...");
  accelgyro.initialize();

  // verify connection
  Serial.println("Testing device connections...");
  Serial.println(accelgyro.testConnection() ? "MPU6050 connection successful" : "MPU6050 connection failed");

  //-------------------//
}

void loop() {
  accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
  delay(10);

  if ((ax > 15000 || ax < -15000)) {
    Serial.println(ax);
    Serial.println("\n");
    digitalWrite(8, HIGH);
    /*digitalWrite(6, HIGH);
    digitalWrite(4, LOW);*/
    SendMessage(onkar);
    if (SIM900.available() > 0) {
      Serial.write(SIM900.read());
    }
    /*SendMessage(yashraj);
    if (SIM900.available() > 0) {
      Serial.write(SIM900.read());
    }*/
    SendMessage(sandesh);
    if (SIM900.available() > 0) {
      Serial.write(SIM900.read());
    }
    /*SendMessage(sandesh);
    if (SIM900.available() > 0) {
      Serial.write(SIM900.read());
    }*/
    digitalWrite(8, LOW);
    /*digitalWrite(6, LOW);
    digitalWrite(4, HIGH);*/
  }
}

//--------------------------//

void SendMessage(String number) {

  if (gps.available() > 0) {
    GPS.encode(gps.read());
    if (GPS.location.isUpdated()) {
      Serial.print("GPS FIXED");
      Serial.print("Latitude= ");
      Serial.print(GPS.location.lat(), 6);
      Serial.print(" Longitude= ");
      Serial.println(GPS.location.lng(), 6);
    }
  }

  //-------------------//
  //-------------------//

  Link = Link + String("18.463653,73.871162");//(String(GPS.location.lat(), 6)) + "," + (String(GPS.location.lng(), 6));

  //-------------------//

  //-------------------//


  //-------------------//
  Serial.println("Sending Message");
  SIM900.println("AT+CMGF=1");  //Sets the GSM Module in Text Mode
  delay(1000);
  Serial.println("Set SMS Number " + number);
  SIM900.println("AT+CMGS=\"" + number + "\"\r");  //Write Mobile number to send message
  delay(1000);
  Serial.println("Set SMS Content");
  SIM900.print("IMPORTANT - A vechicle Accident has been detected,\n\nLocation - ");
  SIM900.print("https://www.google.com/maps/search/?api=1&query=");
  SIM900.print("18.463508,73.868090");  // Messsage content
  delay(100);
  Serial.println("Finish");
  SIM900.println((char)26);  // ASCII code of CTRL+Z
  delay(1000);
  SIM900.print("\r\n");
  Serial.println("Message has been sent");
  //-------------------//

}
