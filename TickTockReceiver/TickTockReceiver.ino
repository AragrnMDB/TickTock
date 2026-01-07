#include <mdb_digitalIn.h>                      // Include my digitalIn library
#include <mdb_digitalOut.h>                     // Include my digitalOut library
#include <Wire.h>                               // Include the I2C library

uint8_t redLed               = 11;              // Define the red led pin (pinmode set in digitalOut)
unsigned long redFlashTime   = 50;              // Length of time for a red flash
uint8_t redCommand = 0;
uint8_t lastRedCommand = 0;
mdb_digitalOut redFlash(redLed, redFlashTime);  // Define a red flash
uint8_t grnLed               = 12;              // Define the green led pin (pinmode set in digitalOut)
unsigned long grnFlashTime   = 50;              // Length of time for a green flash
uint8_t grnCommand = 0;
uint8_t lastGrnCommand = 0;
mdb_digitalOut grnFlash(grnLed, grnFlashTime);  // Define a green flash
uint8_t commLed              = 13;              // Define the comm led pin (pinmode set in digitalOut)
unsigned long commFlashTime  = 25;              // Length of time a comm flash
mdb_digitalOut commFlash(commLed, commFlashTime); // Flasher for comm events

uint8_t oppositeButton       = 7;               // Define the opposit button pin (pinmode set in digitalIn)
bool oppositeFlag = false;                      // True if opposite leds
mdb_digitalIn oppositeToggle(oppositeButton,  INPUT_PULLUP, 20, RISINGEDGE); // Debounced toggle for oppositenButton

uint8_t flashButton          = 6;               // Define the off button pin (pinmode set in digitalIn)
bool flashFlag = false;                         // True if flashing
mdb_digitalIn flashToggle(flashButton, INPUT_PULLUP, 20, FALLINGEDGE); // Debounced toggle for flashButton

void setup() {                                  // Setup function runs once
  Wire.begin(4);                                // Begin the i2c bus with address #4
  Wire.onReceive(receiveEvent);                 // register event handler
  redFlash.begin();                             // Begin the red flashing led
  grnFlash.begin();                             // Begin the green flashing led
  commFlash.begin();                            // Begin the on-board flashing led
  oppositeToggle.begin();                       // Begin the opposite toggle button
  flashToggle.begin();                          // Begin the flash toggle button
}

void loop() {                                   // continuous loop
  commFlash.processFlash();                     // Process an ongoing comm flash
  oppositeFlag = oppositeToggle.toggle();       // Read the opposite toggle button
  flashFlag = flashToggle.toggle();             // Reas the flash toggle button
  if (flashFlag) {                              // If we're flashing
    redFlash.processFlash();                    // Process an ongoing red flash
    grnFlash.processFlash();                    // Process an ongoing green flash
  }
}

void receiveEvent(int howMany) {                // Process the receive event
  commFlash.startFlash();                       // Start a comm flash
  if (howMany == 2) {                           // We're expecting two bytes
    if (!oppositeFlag) {                        // If we're not opposite
      redCommand = Wire.read();                 // Read the red led status and set the red led
      grnCommand = Wire.read();                 // Read the green led status and set the green led
    } else {                                    // If we're opposite
      grnCommand = Wire.read();                 // Read the green led status and set the green led
      redCommand = Wire.read();                 // Read the red led status and set the red led
    }
    if (!flashFlag) {                           // If we're not flashing
      digitalWrite(redLed, redCommand);         // Set the red led
      digitalWrite(grnLed, grnCommand);         // Set the green led
    } else {
      digitalWrite(redLed, LOW);                // Clear the red led
      digitalWrite(grnLed, LOW);                // Clear the green led
      if (redCommand > 0 and redCommand != lastRedCommand) redFlash.startFlash(); // Flash the red led
      if (grnCommand > 0 and grnCommand != lastGrnCommand) grnFlash.startFlash(); // Flash the green led
    }
    lastRedCommand = redCommand;                // Save the red command for next time
    lastGrnCommand = grnCommand;                // Save the green command for next time
  }
}