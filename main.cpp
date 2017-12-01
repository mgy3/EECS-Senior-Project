#include "mbed.h"
#include "DHT.h"
#include <string>
#include <stdint.h>
#define DHTTYPE DHT22
#include "DS1820.h"

Serial pc(USBTX, USBRX);
DHT AirSensor(D4, DHT22);
DS1820  ds1820(A1);    // substitute PA_9 with actual mbed pin name connected to the DS1820 data pin   

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
    
    //DS18B20 Variables and checks
    
    float   temp = 0;
    int     error = 0;
    ds1820.begin();                       
    ds1820.setResolution(12);  

    //permanent loop  
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

    wait(5);
    
        if(ds1820.isPresent()) {
            ds1820.startConversion();  // start temperature conversion
            wait(1.0);                 // let DS1820 complete the temperature conversion
            ds1820.read();
            error = ds1820.read(temp); // read temperature from DS1820 and perform cyclic redundancy check (CRC)
            switch(error) {
            case 0:    // no errors -> 'temp' contains the value of measured temperature
                pc.printf("temp = %3.1f\r\n", temp);
                break;
            case 1:    // no sensor present -> 'temp' is not updated
                pc.printf("no sensor present\n\r");
                break;
            case 2:    // CRC error -> 'temp' is not updated
                pc.printf("CRC error\r\n");
             
        }
    } else
        pc.printf("No DS1820 sensor found!\r\n");
    }//ends while
    
    }//ends main
   