# ![Sign Language Translator](https://undergrad.hxing.me/VE373/SLT/thumbnail.PNG?-source=github)

## Design
Hand gestures (all fingers on both hands with motion detection on one) are recognized into a pre-defined list of words. Results are displayed onto a LCD screen and (optionally) broadcasted from a 3.5 mm headphone jack.

![SLT Design Schematics](https://undergrad.hxing.me/VE373/SLT/design.png?-source=github)

## Demo
Gesture recognition (without voice broadcast) demo is [available on YouTube](https://youtu.be/YqCofvFmG_Q). 

We also have a report detailing the rationale behind the code available [here](https://undergrad.hxing.me/VE373/SLT/report.pdf?-source=github).

## How to Make One for Myself?
You’ll need hardware & software resources as follows:

1. PIC32 Ethernet Starter Kit
	- model [DM320004](http://www.microchip.com/Developmenttools/ProductDetails.aspx?PartNO=DM320004) from Microchip
	- Starter kit datasheet available [here](https://undergrad.hxing.me/VE373/SLT/ethernet+starter+kit+datasheet.pdf?-source=github)
	- datasheet for the chip (PIC32MX795F512L 32-bit micro-controller) is also [available](https://undergrad.hxing.me/VE373/SLT/pic32+datasheet.pdf?-source=github)
	- super expensive BTW
	- software IDE available [here](http://www.microchip.com/mplab/mplab-x-ide)
1. I/O Expansion Board
	- model [DM320002](http://www.microchip.com/DevelopmentTools/ProductDetails.aspx?PartNO=DM320002) from Microchip
	- allows access to starter kit signals with jumper wires
	- not full access though. See **Issues** section for details
1. Angle Displacement Sensors (flex sensors)
	- changes resistance when bent
	- [datasheet available here](https://undergrad.hxing.me/VE373/SLT/flex+datasheet.pdf?-source=github)
	- laddered with resistors to create analog voltage readings
	- five right hand signals feed into PIC32 I/O AN11 to AN15
	- five left hand signals feed into PIC32 I/O AN0 to AN4
	- PIC32 AN ports correspond to I/O Expansion Boards PINs like [this](https://undergrad.hxing.me/VE373/SLT/AN+IO.png?-source=github)
1. Accelerometer
	- outputs x-y-z analog signals in stasis or motion
	- [chip datasheet available here](https://undergrad.hxing.me/VE373/SLT/acc+bare+datasheet.pdf?-source=github)
	- [package datasheet available here](https://undergrad.hxing.me/VE373/SLT/acc+packaged+datasheet.pdf?-source=github)
	- X-Y-Z analog signals feed into PIC32 I/O AN8 to AN10, respectively. That translates to I/O PINs [like this](https://undergrad.hxing.me/VE373/SLT/AN+IO.png?-source=github).
	- fitted on the back side of the glove
		- ![glove fitted with flex and accelerometer looks like this:](https://undergrad.hxing.me/VE373/SLT/glove.png?-source=github)
1. LCD Display
	- displays a two-line message with 16 characters in each line
	- [datasheet available here](https://undergrad.hxing.me/VE373/SLT/lcd+datasheet.pdf?-source=github)
	- connects to PIC32 Starter Kit through I/O Expansion with [these ports](https://undergrad.hxing.me/VE373/SLT/LCD+IO.pdf?-source=github)
1. Voice Output
	- Arduino UNO
		- link to [product site](https://www.arduino.cc/en/Main/ArduinoBoardUno)
		- receives an integer value through UART (*Universal Asynchronous Receiver / Transmitter*). This value corresponds to a MP3 file stored in a SD card. Arduino feeds that file to a MP3 decoder (VS1003) through SPI (*Serial Peripheral Interface*). 
		- UART ports at 
			- Receiver: port 2 
			- Transmitter: port 3
	- SD Card Reader with SPI ports
		- holds the MP3 files
		- bought it [from here](https://detail.tmall.com/item.htm?spm=a230r.1.14.70.4S5Syf&id=43580108042&ns=1&abbucket=5)
		- and it looks like this
		- ![SD Reader](https://undergrad.hxing.me/VE373/SLT/SD.png?-source=github)
		- connect ports to Arduino as specified in file `Arduino/voice.ino`
	- VS1003 MP3 Decoder & DAC
		- reads the MP3 file fed from Arduino (through SPI)
		- decodes MP3 codec
		- convert digital signal to analog signal
		- output analog through a 3.5 mm stereo headphone jack
		- link to [product site](http://www.vlsi.fi/en/products/vs1003.html)
		- and [datasheet](https://undergrad.hxing.me/VE373/SLT/vs1003+datasheet.pdf?-source=github)
		- and it looks like this
		- ![VS1003](https://undergrad.hxing.me/VE373/SLT/vs1003.png?-source=github)
		- connect ports to Arduino as specified in file `Arduino/voice.ino`

## Compilation
Create a MPLAB project using files in folder `PIC32/`
- with a MPLAB X IDE [available here](http://www.microchip.com/mplab/mplab-x-ide)
- using a XC32 (v1.40) compiler
- connect ethernet starter kit to computer and program it

Open file `Arduino/voice.ino`
- with Arduino IDE 1.6.11, [available here](https://www.arduino.cc/en/Main/Software)

Connect hardware as mentioned in the previous section and you are good to go.

## Issues
1. As different batches of flex sensors could produce significantly different output, you might need to adjust default gesture values in file `PIC32/match.c`. Make a gesture with gloves on, check on ADC buffer values in MPLAB IDE and type in new values.
2. Arduino turned out to have a lower computing capacity than expected. As such we weren’t able to program a voice file for every gesture defined. For a fully-functioning voice module, try a higher-end chip instead.
3. And we didn’t include the MP3 files in here. Just put a short (1-2 seconds long) MP3 file in the SD card and it should work. Make sure to name it exactly as we did in `Arduino/voice.ino`

## Unfinished Business  
We were trying to implement a multi-language mode before the project got wrapped up. The idea was to press `SW3` on ethernet starter kit, and I/O port `RC1` would get a high or low signal. Feed that signal to Arduino and we will be able to play a different MP3 file accordingly. Record the other file in a different language and we will have multi-language functions. But limitations in Arduino’s computation capacity stopped this effort. Nevertheless the code on PIC32 stayed. 

## Credits
This project is completed with
- Xinyue Ou (Neil). Reach him [through GitHub](https://github.com/Neil-2013).
- Sandy Hsia. Reach her [through GitHub](https://github.com/sandyhsia).
- H.XING. Reach me [through here](http://goo.gl/JyGpYW).

Arduino sine test and SD card reading algorithms in file `Arduino/voice.ino` are adapted from Xiao Che’s code published [on her blog](http://goo.gl/4vHQEt). 

The amazing sign language art in the pictures are from Dr. Bill's [ASL Finger-spelling and Hand-shape Art](http://goo.gl/76faOQ), with © belonging to  Lifeprint.com
