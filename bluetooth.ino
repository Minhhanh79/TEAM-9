#include <ArduinoBLE.h>

#define SERVICE_UUID "19B10000-E8F2-537E-4F6C-D104768A1214" //uart service UUID
#define CHARACTERISTIC_UUID_TX "19B10001-E8F2-537E-4F6C-D104768A1214" //transfer

BLEService Dataservice(SERVICE_UUID);
BLECharacteristic Datachar (CHARACTERISTIC_UUID_TX, BLERead | BLEWrite, 1);

byte data0;
byte portvalue[1] = {0}; //port A0
int newInt;

byte hi, lo;

void setup(){
  
  Serial.begin(9600);
  
  while(!Serial);

  if(!BLE.begin()){
    Serial.println("starting BLE failed.");
    while(1);
  }
  
  BLE.setLocalName("Arduino Nano 33 IoT");
  BLE.setAdvertisedService(Dataservice);
  
  Dataservice.addCharacteristic(Datachar);
  
  BLE.addService(Dataservice);
  Datachar.writeValue(portvalue, 1);
  
  BLE.advertise();
  Serial.println("Bluetooth device active, waiting for connections...");
}

void loop(){
  BLEDevice central = BLE.central();
  if(central){
    Serial.println("Connected to central device!");
    Serial.println(central.address());
    Serial.println(" ");

    while(central.connected()){
        for (int i = 0; i < 1; i++) {
          if (i==0){
            portvalue[i] = byte(analogRead(i));
          }
          else{
            portvalue[i] = 0;
          }
        }
        /*
          hi = highByte(data0);
          lo = lowByte(data0);
        
          Serial.print("hi: ");
          Serial.println(hi, BIN);
          Serial.print("lo: ");
          Serial.println(lo, BIN);
          newInt = (hi << 8) + lo;
          Datachar.writeValue(hi);
          Datachar.writeValue(lo);
        */
        
        Datachar.writeValue(portvalue, 1);
        for (int i = 0; i < 1; i++) {
          Serial.print(portvalue[i]);
          Serial.print(" ");
        }
        Serial.println();
        delay((2000));
    }
    Serial.print("Disconnected from central: ");
    Serial.println(central.address());   
  }
}
