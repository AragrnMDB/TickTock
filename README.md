# TickTock sample programs for Arduino

## TickTockSolo

This program runs on a solo arduino and blinks two leds. As stored here it alternates the two leds (one on and one off) in a
one second cycle (on for 500ms, off for 500ms). Hence the name TickTock.

There are two buttons that are monitored. If one is pressed and held, the tick tocking stops and both leds are lit.
If the other button is pressed and held, the tick tocking stops with both leds off.

Uses:  
- [mdb_blink](https://github.com/AragrnMDB/mdb_blink.git "mdb_blink") 
- [mdb_debounce](https://github.com/AragrnMDB/mdb_debounce.git "mdb_debounce")
- These libraries also use: [mdb_timer](https://github.com/AragrnMDB/mdb_timer.git "mdb_timer") 



## TickTockSender

This program is the same as TickTock solo with the exception that whenever one or more leds changes state, the state of the
two leds is sent over I2C to address 4.

Uses:  
- [mdb_blink](https://github.com/AragrnMDB/mdb_blink.git "mdb_blink") 
- [mdb_debounce](https://github.com/AragrnMDB/mdb_debounce.git "mdb_debounce") 
- [mdb_flash](https://github.com/AragrnMDB/mdb_flash.git "mdb_flash") 
- These libraries also use: [mdb_timer](https://github.com/AragrnMDB/mdb_timer.git "mdb_timer") 

## TickTockReceiver

This program monitors I2C address 4. When it receives a transmission, the first byte is the state of the first led, the second
byte is the state of the second led.

There are two buttons that are monitored. If the first button is clicked, the leds swap (if led 1 is on at the sender, led 2 is
on at the receover). If the second button is clicked, the leds flash (on for 20ms) instead of remaining on.

Uses:  
- [mdb_debounce](https://github.com/AragrnMDB/mdb_debounce.git "mdb_debounce") 
- [mdb_flash](https://github.com/AragrnMDB/mdb_flash.git "mdb_flash") 
- These libraries also use: [mdb_timer](https://github.com/AragrnMDB/mdb_timer.git "mdb_timer") 

