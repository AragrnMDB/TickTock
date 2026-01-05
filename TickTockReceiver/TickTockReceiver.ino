#include <mdb_digitalIn.h>                      // Include my digitalIn library
#include <mdb_digitalOut.h>                     // Include my digitalOut library
#include <Wire.h>                               // Include the I2C library

uint8_t redLed               = 11;              // Define the red led pin
unsigned long redFlashTime   = 20;              // Length of time for a red flash
uint8_t redCommand = 0;
mdb_digitalOut redFlash(redLed, redFlashTime);       // Define a red flash
uint8_t grnLed               = 12;              // Define the green led pin
unsigned long grnFlashTime   = 20;              // Length of time for a green flash
uint8_t grnCommand = 0;
mdb_digitalOut grnFlash(grnLed, grnFlashTime);       // Define a green flash
uint8_t commLed              = 13;              // Define the comm led pin (pinmode set in mdb_flash)
unsigned long commFlashTime  = 25;              // Length of time a comm flash
mdb_digitalOut commFlash(commLed, commFlashTime);    // Flasher for comm events

uint8_t oppositeButton       = 7;               // Define the opposit button pin (pinmode set in debounce)
bool oppositeFlag = false;
bool lastOpposite = false;
mdb_digitalIn debouncedOpposite(oppositeButton,  INPUT_PULLUP, 20, RISINGEDGE); // Debounced input for oppositenButton

uint8_t flashButton          = 6;               // Define the off button pin (pinmode set in debounce)
bool flashFlag = false;                         // True if flashing
bool lastFlash = false;                         // True if flash button has already been noted
mdb_digitalIn debouncedFlash(flashButton, INPUT_PULLUP, 20, FALLINGEDGE); // Debounced input for offButton

void setup() {                                  // Setup function runs once
  Wire.begin(4);                                // Join i2c bus with address #4
  Wire.onReceive(receiveEvent);                 // register event handler
  redFlash.begin();
  grnFlash.begin();
  commFlash.begin();
  debouncedOpposite.begin();
  debouncedFlash.begin();
}

void loop() {                                   // continuous loop
  commFlash.processFlash();                     // Process an ongoing comm flash
  oppositeFlag = debouncedOpposite.toggle();
  flashFlag = debouncedFlash.toggle();
  if (flashFlag) {                              // If we're flashing
    redFlash.processFlash();                    // Process an ongoing red flash
    grnFlash.processFlash();                    // Process an ongoing green flash
  }
}

void receiveEvent(int howMany) {                // Process the receive event
  commFlash.startFlash();                       // Start a comm flash
  if (howMany == 2) {                           // We're expecting two bytes
    if (!oppositeFlag) {
      redCommand = Wire.read();                 // Read the red led status and set the red led
      grnCommand = Wire.read();                 // Read the green led status and set the green led
    } else {
      grnCommand = Wire.read();                 // Read the green led status and set the green led
      redCommand = Wire.read();                 // Read the red led status and set the red led
    }
    if (!flashFlag) {
      digitalWrite(redLed, redCommand);
      digitalWrite(grnLed, grnCommand);
    } else {
      if (redCommand > 0) redFlash.startFlash();
      if (grnCommand > 0) grnFlash.startFlash();
    }
  }
}