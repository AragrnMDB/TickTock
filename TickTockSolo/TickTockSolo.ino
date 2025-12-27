#include <mdb_debounce.h>                   // Include my debounce library
#include <mdb_blink.h>                          // Include my blink library

uint8_t redLed               = 11;              // Define the red led pin
unsigned long redOnTime      = 500;             // Amount of time the red led is on in a cycle
unsigned long redOffTime     = 500;             // Amount of time the red led is off in a cycle
uint8_t redLedState          = LOW;             // Red led state
uint8_t redLedSend           = redLedState;     // Red led state to send
mdb_blink redBlinker(redLed, redOnTime, redOffTime, redLedState);

uint8_t grnLed               = 12;              // Define the green led pin
unsigned long grnOnTime      = 500;             // Amount of time the green led is on in a cycle
unsigned long grnOffTime     = 500;             // Amount of time the green led is off in a cycle
uint8_t grnLedState          = HIGH;            // Green led state
uint8_t grnLedSend           = grnLedState;     // Green led state to send
mdb_blink grnBlinker(grnLed, grnOnTime, grnOffTime, grnLedState);

uint8_t onButton             = 7;               // Define the on button pin (pinmode set in debounce)
mdb_debounce debouncedOn(onButton,  INPUT_PULLUP, 20); // Debounced input for onButton

uint8_t offButton            = 6;               // Define the off button pin (pinmode set in debounce)
mdb_debounce debouncedOff(offButton, INPUT_PULLUP, 20); // Debounced input for offButton

void setup() {                                  // Setup function runs once
  redBlinker.startBlink();                      // Start the red blinker
  grnBlinker.startBlink();                      // Start the green blinker
}

void loop() {
  redLedState = redBlinker.processBlink();      // Process the blinking of the red led
  grnLedState = grnBlinker.processBlink();      // Process the blinking of the green led
  if (debouncedOn.inputState()) {               // If the ON button is pressed
    redLedSend = HIGH;                          // Send the red led as on
    grnLedSend = HIGH;                          // Send the green led as on
    redBlinker.holdBlink();                     // Reset the red timer to keep it from elapsing
    grnBlinker.holdBlink();                     // Reset the red timer to keep it from elapsing
  } else if (debouncedOff.inputState()) {       // Else If the OFF button is pressed
    redLedSend = LOW;                           // Send the red led as off
    grnLedSend = LOW;                           // Send the green led as off
    redBlinker.holdBlink();                     // Reset the red timer to keep it from elapsing
    grnBlinker.holdBlink();                     // Reset the red timer to keep it from elapsing
  } else {                                      // Else (neither button is pressed)
    redLedSend = redLedState;                   // send the red led as the red led state
    grnLedSend = grnLedState;                   // send the green led as the green led state
  }
  digitalWrite(redLed, redLedSend);             // Update the red led to the red send
  digitalWrite(grnLed, grnLedSend);             // Update the green led to the green send
}