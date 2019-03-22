# EANAB Maker

## What It Does
This is a device that makes and mixes different combinations of EANABs to make your favorite drink! You start by filling in your drink prefereces on our EANAB WebApp™ hosted on our Concierge™ and swiping your Stanford ID card to store your preferences in our database. Then, walk over to our DrinkMaker™ station and swipe your Stanford ID card to retrieve your preferences, which have been magically transmitted over Wi-Fi. Put a cup on top of the conveyor belt and press a blue button when you're ready to be breathalyzed. Our device is an EANAB maker that makes EANABs and EANABs only, so our breathalyzer will make sure you've only had EANABs today! Then, once the breathalyzer confirms you're good to go, the conveyor belt will move your cup to our valve station, where our specially configured valves will dispense the correct combination of components for your EANABs. Finally, our conveyor belt will return your drink to you, with a fresh cup of EANABs.

## The Different Components
### EANAB WebApp
This is an online interface to enter in your preferences by first adjusting the amount of each ingredient you want through our draggable pie chart or adjusting the numbers, and then swiping your ID card to store your preferences.

### MagStripe
Both our concierge and our maker use a PS/2 protocol magstripe to read your Stanford ID card to store and receive your drink preferences.

### ESP32
We use an ESP-32 on both the concierge and the maker to communicate drink preferences from one to another, so the maker can retrieve the appropriate drink when you swipe the card.

### TAJ Protocol
The fittingly-named TAJ (Theo, Ankush, Jainil) protocol is used to communicate preferences from the ESP32 to the Raspberry Pi. It's not just typical UART!

### Button
We use a pretty simple pull-up resistor and button to register when you're ready to be breathalyzed.

### Breathalyzer
An arduino breathalyzer helps us make sure that you're good to drink our EANABs. We've calibrated the breathalyzer to get your BAC and reject you if you're over the legal limit of 0.08.

### Stepper Motor
We use a stepper motor to drive our conveyor belt, with a pretty sick library of different angles, settings, directions, and things it can do.

### Laser and Photoresistor
How do we stop our conveyor belt? We use a photoresistor to register the presence of a laser that emits directly across from it; if the laser is blocked by a cup, the photoresistor tells the conveyor belt to stop moving and the program moves to drink filling. 

### Analog to Digital Convertor
We use an MCP3008 ADC to convert analog signals from the breathalyzer and the photoresistor to be used by the Raspberry Pi.

### Valves and Relay
Our valves are solenoids that open when a 12V current is wired to them. We use a relay and GPIO pins to control when and for how long that current is present for, so that we can give you a nice drink with all of your desired components.

### Lights
Our config didn't look cool enough as is, so we decided to give it some flair with some sick lighting.

## To Run
For the concierge: run 'make install-c'  
For the maker: run 'make install-m'

## Roles and Responsibilities
We worked on a lot of the project together, but to break down some of the responsibilities.  
Theo: ESP32, TAJ protocol, magstripe, communication and storage of drink preferences  
Ankush: valve control, breathalyzer control, rand library, construction  
Jainil: stepper, photoresistor and laser, rand library, construction  
