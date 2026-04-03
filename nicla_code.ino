#include <ArduinoBLE.h>
#include "Nicla_System.h"

// This creates a "Service" and a "Characteristic" (the mailbox where the phone drops data)
BLEService ledService("19B10000-E8F2-537E-4F6C-D104768A1214"); 
BLEByteCharacteristic switchChar("19B10001-E8F2-537E-4F6C-D104768A1214", BLERead | BLEWrite);

void setup() {
  Serial.begin(115200);
  nicla::begin();
  nicla::leds.begin();

  if (!BLE.begin()) {
    while (1); // Stop if BLE fails
  }

  BLE.setLocalName("Nicla_Remote");
  BLE.setAdvertisedService(ledService);
  ledService.addCharacteristic(switchChar);
  BLE.addService(ledService);
  
  // Start with a White LED to show we are ready
  nicla::leds.setColor(white);
  BLE.advertise();

  Serial.println("Ready! Connect via nRF Connect...");
}

void loop() {
  BLEDevice central = BLE.central();

  if (central) {
    Serial.print("Connected to: ");
    Serial.println(central.address());

    while (central.connected()) {
      if (switchChar.written()) {
        uint8_t value = switchChar.value();
        Serial.print("Phone sent: ");
        Serial.println(value);

        if (value == 1) {
          nicla::leds.setColor(red);
        } else if (value == 2) {
          nicla::leds.setColor(green);
        } else if (value == 3) {
          nicla::leds.setColor(blue);
        } else {
          nicla::leds.setColor(off);
        }
      }
    }
    Serial.println("Disconnected");
    nicla::leds.setColor(white); // Back to white when phone leaves
  }
}
