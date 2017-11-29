#include <DHT.h>
//#include <LowPower.h>
#include <avr/sleep.h>
#include <avr/interrupt.h>
#include <avr/power.h>
#include <avr/wdt.h>
#include <EEPROM.h>
#include <OneWire.h>
#include <DallasTemperature.h>

#define DHTPIN 31
#define DHTTYPE DHT22
#define ONE_WIRE_BUS 33

volatile int f_wdt = 1;

DHT AirSensor(DHTPIN, DHTTYPE);
OneWire ds(ONE_WIRE_BUS); //created the soil temperature object, designed to be read at One Digital readout
DallasTemperature SoilSensor(&ds);
//SoftwareSerial mbedSerial(17,16);//RX, TX

//Variables
int chk;
int counter;
int output_value ;
float Atemp;
float Ahum;

// Watchdog Interrupt Service. This is executed when watchdog timed out.
ISR(WDT_vect)
{
  wdt_disable();  // disable watchdog
}

void myWatchdogEnable(const byte interval)
{
  MCUSR = 0;                          // reset various flags
  WDTCSR |= 0b00011000;               // see docs, set WDCE, WDE
  WDTCSR =  0b01000000 | interval;    // set WDIE, and appropriate delay

  wdt_reset();
  set_sleep_mode (SLEEP_MODE_PWR_DOWN);
  sleep_mode();            // now goes to Sleep and waits for the interrupt
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600); //serial for serial monitor
 // Serial2.begin(9600);
  AirSensor.begin();
  SoilSensor.begin();
 
  counter = 0;
}

//main actions
void loop() {
  delay(100); //small delay needed

  //sleeping 
  for( int i=0; i< 0; i++){//just repeatedly calling for it to sleep
  myWatchdogEnable (0b100001); //8 seconds (max)
  }

  delay(2000);
  String AirT = AirTemp();
  String AirH = AirHumidity();
  String SoilTempData = SoilTemp();
  // String SoilMoisture = SoilMoist();
 // String LightSensor = PAR();

  //creating the character arrays to store in EEPROM
  char AT[15]; //airtemp
  char AH[15]; //air humidity
  char ST[20]; //soil temperature
  char SM[20]; //soil moisture
  char LS[15]; //light sensing
  
  AirT.toCharArray(AT,15);
  AirH.toCharArray(AH,15);
  SoilTempData.toCharArray(ST,20);
 // SoilMoisture.toCharArray(SM,20);
 // LightSensor.toCharArray(LS,15);

  Serial.println(SoilTempData);
  Serial.println(AirT);
  Serial.println(AirH);
  //method to save 3 measurements of each value and then output it to LoRa later

  if (counter == 0) {
    EEPROM.put(0, AT); //saving air temp
    EEPROM.put(sizeof(AT), AH);//saving air temp
    EEPROM.put( (sizeof(AT) + sizeof(AH)) , ST); // saves soil temp
  //  EEPROM.put( (sizeof(AT) + sizeof(AH) + sizeof(ST) ) , SM); //saving soil moisture
  //  EEPROM.put( (sizeof(AT) + sizeof(AH) + sizeof(ST) + sizeof(SM) ) , LS); //saving light
    Serial.println(counter);

    counter++;
    
    }
    else if (counter == 1) {
    EEPROM.put( (sizeof(AT) + sizeof(AH) + sizeof(ST) + sizeof(SM) + sizeof(LS)) , AT); //saving air temp
    EEPROM.put( (sizeof(AT)*2 + sizeof(AH) + sizeof(ST) + sizeof(SM) + sizeof(LS)) , AH); //saving air humidity
    EEPROM.put( (sizeof(AT)*2 + sizeof(AH)*2 + sizeof(ST) + sizeof(SM) + sizeof(LS)) , ST); //saving soil temp
//    EEPROM.put( (sizeof(AT)*2 + sizeof(AH)*2 + sizeof(ST)*2 + sizeof(SM) + sizeof(LS)) , SM); //saving soil moisture
//    EEPROM.put( (sizeof(AT)*2 + sizeof(AH)*2 + sizeof(ST)*2 + sizeof(SM)*2 + sizeof(LS)) , LS); //saving light
    Serial.println(counter);

    counter++;

    }
    else if (counter == 2) {
    EEPROM.put( (sizeof(AT)*2 + sizeof(AH)*2 + sizeof(ST)*2 + sizeof(SM)*2 + sizeof(LS)*2 ) , AT); //saving air temp
    EEPROM.put( (sizeof(AT)*3 + sizeof(AH)*2 + sizeof(ST)*2 + sizeof(SM)*2 + sizeof(LS)*2) , AH); //saving air humidity
    EEPROM.put( (sizeof(AT)*3 + sizeof(AH)*3 + sizeof(ST)*2 + sizeof(SM)*2 + sizeof(LS)*2) , ST); //saving soil temp
//    EEPROM.put( (sizeof(AT)*(3/2) + sizeof(AH)*(3/2) + sizeof(ST)*(3/2) + sizeof(SM) + sizeof(LS))*2 , SM); //saving soil moisture
//    EEPROM.put( (sizeof(AT)*(3/2) + sizeof(AH)*(3/2) + sizeof(ST)*(3/2) + sizeof(SM)*(3/2) + sizeof(LS)*2 , LS); //saving light
    Serial.println(counter);

    counter++;

    }
    //reset and send data
    else if (counter == 3) {
    //retrieving the char[]
    //testing to see if it'll work
  Serial.println("Fetching Values")  ;
    
  char R1[15]; //airtemp
  char R2[15]; //air humidity
  char R3[20]; //soil temperature
  char R4[20]; //soil moisture
  char R5[15]; //light sensing
  EEPROM.get(0, R1);
  EEPROM.get(15, R2);
  EEPROM.get(30, R3);
//  EEPROM.get(50, R4);//soil moisture
//  EEPROM.get(70, R5);//light sensing
  
//transmission
 // Serial2.println(R1);
//  Serial2.write('A');

//  Serial.println(R1);
/*  Serial.println(R1);
  Serial.println(R2);
  Serial.println(R3);
  Serial.println("Returned First Stored Values");
*/
//second set of numbers
  EEPROM.get(85, R1);
  EEPROM.get(100, R2);
  EEPROM.get(115, R3);
//  EEPROM.get(135, R4);
//  EEPROM.get(155, R5);

/*  Serial.println(R1);
  Serial.println(R2);
  Serial.println(R3);
  Serial.println("Returned Second Stored Values");
*/
  EEPROM.get(170, R1);
  EEPROM.get(185, R2);
  EEPROM.get(200, R3);
//  EEPROM.get(220, R4);
//  EEPROM.get(240, R5);
/*
  Serial.println(R1);
  Serial.println(R2);
  Serial.println(R3);
  Serial.println("Returned Third Stored Values");
*/
    //code to send the data to the LoRa module
  


    Serial.println("Sent a message");
    counter = 0;
    }
}//ends main loop

String AirTemp() {
  Atemp = AirSensor.readTemperature();
  String s1 = "";
  
  if (isnan(Atemp)) {
    s1 = s1 + "No temp";
    return s1;
  }//restarts loop
 
  s1 = s1 + "ATemp: " + Atemp + "C" ;
  //returning a max of 20 characters
  return s1;
}

String AirHumidity(){
    Ahum = AirSensor.readHumidity();
  String s1 = "";
 if (isnan(Ahum)) {
    s1 = "No humidity";
    return;
 }
   s1 = "AHumid: " + (String)Ahum + "%";
  //returning a max of 20 characters
  return s1;
}

//measuring soil temperature
String SoilTemp() {
  String Stemp = "";
  SoilSensor.requestTemperatures();

  Stemp = Stemp + "Soil Temp: ";
  Stemp = Stemp + SoilSensor.getTempCByIndex(0) + "C" ;

  return Stemp;
}

//measuring soil moisture
//subject to change
String SoilMoist() {
  String Smoist = "";
  Serial.print("Sensor 1: ");
  Serial.println(analogRead(A0));
  Serial.print("Sensor 2: ");
  Serial.println(analogRead(A1));
  Serial.print("Sensor 3: ");
  Serial.println(analogRead(A2));
  Serial.print("Sensor 4: ");
  Serial.println(analogRead(A3));
  Serial.print(" Average: ");
  Serial.println((analogRead(A0) + analogRead(A1) + analogRead(A2) + analogRead(A3)) / 4);
  Serial.println(" ");
  return "Soil Moist: ";
}

String PAR() {
  String Light = "";

  return "WL: 400nm";
}

