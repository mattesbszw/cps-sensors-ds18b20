#include <OneWire.h> // library for OneWire-Bus
#include <DallasTemperature.h> // sensor library for ds18b20

#define PIN_ONE_WIRE_BUS 13

// create bus object
OneWire oneWire(PIN_ONE_WIRE_BUS); 

// create sensors collection
DallasTemperature sensors(&oneWire);

// declare the sensors' physical addresses for the alternative way to get their values
// DeviceAddress sensor0 = { 0x28, 0x92, 0xE2, 0x49, 0xF6, 0xE1, 0x3C, 0x1B }; // yellow marker 
// DeviceAddress sensor1 = { 0x28, 0x23, 0xD5, 0x49, 0xF6, 0x45, 0x3C, 0x2B }; // red marker
// DeviceAddress sensor2 = { 0x28, 0xE2, 0x0D, 0x49, 0xF6, 0x7C, 0x3C, 0x7C }; // green marker
// DeviceAddress sensor3 = { 0x28, 0x8D, 0x7D, 0x49, 0xF6, 0x71, 0x3C, 0x7F }; // black/no marker

void setup(){

  // init serial monitor
  Serial.begin(115200);

  // scan OneWire bus for physical addresses; 
  // uncomment to get the phyiscal addresses for alternative way
  // scan_bus_for_addresses();

  // init sensors collection
  sensors.begin();

}

void loop(void){ 
  
  Serial.println("Requesting temperatures...");

  // send the command to get temperatures
  sensors.requestTemperatures(); 

  // DEVICE_DISCONNECTED_C = -127, as defined in DallasTemperature.h
  float t = DEVICE_DISCONNECTED_C;

  // get all ds18b20 sensor values from bus
  for (byte i=0; i<sensors.getDS18Count(); i++) {
    t = sensors.getTempCByIndex(i);
    if (t != DEVICE_DISCONNECTED_C) {
      Serial.println("Sensor " + String(i) + ": " + String(t));
    }
  }

  // if you have only 1 sensor, you don't need the loop
  // float t = sensors.getTempCByIndex(0);
  // if (t != DEVICE_DISCONNECTED_C) {
  //  Serial.print("Sensor 0: " + String(t));
  // }

  // alternatively get temperature from specific sensor via its address
  // float t = sensors.getTempC(sensor1); 

  delay(2000);
}

void scan_bus_for_addresses()
{
  int deviceCount = 0;

  byte address[8];
  memset(address, 0, sizeof(address));
  
  // scan for devices on OneWire bus
  while (oneWire.search(address)) {
    // check if address is valud
    if (OneWire::crc8(address, 7) != address[7]) {
      Serial.println("Error: Invalid hardware address (CRC)!");
      continue;
    }

    // Ausgabe der Geräteadresse
    Serial.print("Device ");
    Serial.print(deviceCount);
    Serial.print(": Address = ");

    deviceCount++;

    for (byte i = 0; i < 8; i++) {
      Serial.print(" ");
      if (address[i] < 16) {
        Serial.print("0"); // add leading 0 if necessary
      }
      Serial.print(address[i], HEX); // print every value as hex number
    }
    Serial.println();
  }

  // reset search for next time
  oneWire.reset_search();

  Serial.print("Found " + String(deviceCount) + " device(s) on OneWire bus.");

}