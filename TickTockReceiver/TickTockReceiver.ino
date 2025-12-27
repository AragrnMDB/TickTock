#include <mdb_debounce.h>                       // Include my debounce library
#include <mdb_flash.h>                          // Include my flash library
#include <Wire.h>                               // Include the I2C library

uint8_t redLed               = 11;              // Define the red led pin
unsigned long redFlashTime   = 20;              // Length of time for a red flash
uint8_t redCommand = 0;
mdb_flash redFlash(redLed, redFlashTime);       // Define a red flash
uint8_t grnLed               = 12;              // Define the green led pin
unsigned long grnFlashTime   = 20;              // Length of time for a green flash
uint8_t grnCommand = 0;
mdb_flash grnFlash(grnLed, grnFlashTime);       // Define a green flash
uint8_t commLed              = 13;              // Define the comm led pin (pinmode set in mdb_flash)
unsigned long commFlashTime  = 25;              // Length of time a comm flash
mdb_flash commFlash(commLed, commFlashTime);    // Flasher for comm events

uint8_t oppositeButton       = 7;               // Define the opposit button pin (pinmode set in debounce)
bool oppositeFlag = false;
bool lastOpposite = false;
mdb_debounce debouncedOpposite(oppositeButton,  INPUT_PULLUP, 20); // Debounced input for oppositenButton

uint8_t flashButton          = 6;               // Define the off button pin (pinmode set in debounce)
bool flashFlag = false;                         // True if flashing
bool lastFlash = false;                         // True if flash button has already been noted
mdb_debounce debouncedFlash(flashButton, INPUT_PULLUP, 20); // Debounced input for offButton

void setup() {                                  // Setup function runs once
  Wire.begin(4);                                // Join i2c bus with address #4
  Wire.onReceive(receiveEvent);                 // register event handler
  digitalWrite(redLed, LOW);                    // Initialize the red led to off
  digitalWrite(grnLed, LOW);                    // Initialize the green led to off
}

void loop() {                                   // continuous loop
  commFlash.processFlash();                     // Process an ongoing comm flash
  if (debouncedOpposite.inputState() and ! lastOpposite) { // If the opposite button is pressed and was not last time
    oppositeFlag = !oppositeFlag;               // Flip the opposite flag
    lastOpposite = true;                        // Remember that the opposite button was pressed
  } else {                                      // Otherwise
    if (!debouncedOpposite.inputState()) {      // If the opposite button is not pressed
      lastOpposite = false;                     // Clear the last opposite flag
    }
  }
  if (debouncedFlash.inputState() and ! lastFlash) { // If the flash button is pressed and was not last time
    flashFlag = !flashFlag;                     // Flip the flash flag
    lastFlash = true;                           // Remember that the flash button was pressed
  } else {                                      // Otherwise
      if (!debouncedFlash.inputState()) {       // If the flash button is not pressed
        lastFlash = false;                      // Clear the flash flag
      }
  }
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