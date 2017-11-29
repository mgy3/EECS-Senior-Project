#include "mbed.h"
#include "DHT.h"
#include <string>
#include <stdint.h>
#define DHTTYPE DHT22

Serial pc(USBTX, USBRX);
DHT AirSensor(D4, DHT22);
//DigitalInOut sensor(D3); Soil temp is an analog in this case
//don't use pin D5 on the mbed

int main(){
    //sensor.mode(PullUp);
    
    pc.baud(9600);
    pc.printf("Starting:\n");
      
    float AirTemp;
    float AirHumidity;
    float SoilTemp;
    char ST[30];
    char AT[25];
    char AH[30]; //may need to limit size
    int err;  
    bool go = true;  
   
    while(1){
    go=true;
       wait(2);
        
    //read until there isn't an error 
        while(go){
    //  pc.printf("Number is: %d \n",err);
       err = AirSensor.readData();
        if(err == 0){
            wait(5);
            //should convert the values in code to floats, not ints
            AirTemp = AirSensor.ReadTemperature(CELCIUS);
            AirHumidity = AirSensor.ReadHumidity();
            sprintf(AT, "Air Temp: %.2fC",AirTemp); 
            pc.printf("%s\n",AT);
           // pc.printf("Humidity : %f \n",AirHumidity);
            sprintf(AH, "Air Humid: %.2f",AirHumidity);
            pc.printf("%s\n",AH);
            go=false;
        }//ends if
        
        }//ends air sensing while

    wait(10);
    }//ends while
    
    }//ends main
   