# Sound reactive room lighting

[![Sound reactive room lighting](/../files/images/ceiling-888.gif?raw=true "Sound reactive room lighting")](#sound-reactive-room-lighting)

This code powers my bedroom lights that were [written about in Atmel blog](https://atmelcorporation.wordpress.com/category/cool-things/page/4/). I created the original incarnation for my Tokyo apartment in 2014 using 12 meters of [Adafruit NeoPixels](https://www.adafruit.com/category/168) strips and code based on Adafruit’s [LED Ampli-Tie](https://learn.adafruit.com/led-ampli-tie/overview). Two bedrooms later, I am now using about 15 meters of similar LED strips inside white pool noodles for diffusion. I began breaking the monolithic project file into reusable components since I was doing similar things across projects, but this is still a work in progress.

There are multiple modes, including the sound reactive one pictured as well as different types of rainbows and a solid color fade to dark. Brightness can also be controlled by a potetiometer.

## Components

**2015 Haight Street setup**

See my blog for more installation documentation: [Sound reactive room lighting using Arduino and LED strips](https://votecharlie.com/blog/2016/02/sound-reactive-room-lighting-using-arduino-and-led-strips.html)

* 1x [FLORA - Wearable electronic platform: Arduino-compatible - v2][659], $19.95
* 12x [Adafruit NeoPixel Digital RGB LED Strip - White 60 LED][1138], $23.95/m for 4–39 meters
* 1x [Electret Microphone Amplifier - MAX4466 with Adjustable Gain][1063], $6.95
* 1x [Panel Mount 10K potentiometer (Breadboard Friendly) - 10K Linear][562], $0.95 
* 1x [Potentiometer Knob - Soft Touch T18 - Blue][2048], $0.50
* <del>1x [Tactile On/Off Switch with Leads][1092], $3.95</del>
* 1x [Tactile Button switch (6mm) x 20 pack][367], $2.50
* 3x [5V 10A switching power supply][658], $25.00 each
* 3x [Female DC Power adapter - 2.1mm jack to screw terminal block][368], $2.00 each
* 2x [4700uF 10v Electrolytic Capacitor][1589], $1.95 each
* 1x [JST 2-pin cable][261], $0.75
* [jumper wires][wires] and [header pins][header] to connect things, if you want to do it my way
* old CAT5 or other cable with at least three wires to connect FLORA to lights

![room lights diagram](https://votecharlie.com/rsc/t/h.mb?src=%2Fblog%2F2016%2F02%2Froom_lights_diagram.png&w=1000&h=1800&hash=d95f3fe2e954eeaf5155d5f7cd850b50)

**2017 Santiago Street setup**

I am now using a beefier [Arduino Mega 2560](https://www.arduino.cc/en/Main/arduinoBoardMega2560) R3 and two 5V 40W power supplies, and thus I have redone most of the electronics. I will provide more details once it is more finalized.

* 15m [WS2813 or WS2812B LED strips, 300 LEDs/5m](http://amzn.to/2r2SkCE), ~$30 per 5 meters
* 2x [Switching Power Supply for LED Strip Light 200W 176-242VAC 5V 40A](http://amzn.to/2qcK7dK), ~$25 each
* 1x [Ieik Mega 2560 R3 ATmega2560-16AU + ATMEGA16U2](http://amzn.to/2rbEknj), $12.49
* 1x [Eleduino Arduino Mega 2560 Case Enclosure Box](http://amzn.to/2pHIemW), $9.99
* 1x [Electronics-Salon 10x Prototype PCB for Arduino Mega 2560 R3 Shield Board DIY](http://amzn.to/2qCbqjq), $16.80
* 1x [ZJchao 9V 1A Power Adapter](http://amzn.to/2qcF8dm), $5.49
* 1x [DIKAVS 10PCS Breadboard Trim Potentiometer With Knob 10K for Arduino](http://amzn.to/2qC3tut), $11.39
* 2x [Electret Microphone Amplifier - MAX4466 with Adjustable Gain][1063], $6.95 each
* 1x [Command Mini Hooks, Clear, 6-Hook, 6-Pack](http://amzn.to/2qCcPpX), $11.99
* 1x [18AWG Low Voltage LED Cable 3 Conductor Jacketed In-Wall Speaker Wire UL/cUL Class 2 (200 ft reel)](http://amzn.to/2pHGslD), $42.27
* 1x [25 Pack of White 52 Inch Oodles of Noodles Foam Swimming Pool Floatie](http://amzn.to/2qC79wm), $69.99
* 1x [Spiderwire Ultracast Ultimate-Monofilament Superline](http://amzn.to/2r34Dix), $6.98
* 1x [Clear Polycarbonate Tubing, 7/8" ID, 1" OD, 1/16" Wall, 6' Length](http://amzn.to/2r2WWso), $18.48
* 3x [4700uF 10v Electrolytic Capacitor][1589], $1.95 each
* 1x [Uxcell a12013100ux0116 High Knob 3P 2 Position 1P2T SPDT Vertical Slide Switch, 0.5 Amp, 50V DC, 50 Piece, 3 mm](http://amzn.to/2qC3bni), $3.67
* 1x [100 Pcs 6x6x4.5mm Panel PCB Momentary Tactile Tact Push Button Switch 4 Pin DIP](http://amzn.to/2r38hsz), $4.86
* [jumper wires][wires] and [header pins][header] to connect things, if you want to do it my way
* 1x [VIMVIP 3Pin 30cm Servo Extension Cable 22AWG 60 Cores Wire Male to Male Futaba JR for DJI Phantom 4 3 2 DJI Inspire 2 1 and Other Remote Control Aircraft](http://amzn.to/2pHL8Hn), $12.99

## Media

* Video: [Sound reactive room lighting](https://vimeo.com/154562012)


[blog]: https://votecharlie.com/blog/2016/02/sound-reactivity-restored-to-room-lighting.html
[pins]: http://amzn.to/2rbveXI
[fritlib]: https://github.com/adafruit/Fritzing-Library
[frit]: http://fritzing.org/home/
[header]: http://amzn.to/2r3ercm
[wires]: https://www.adafruit.com/categories/306
[bm]: https://votecharlie.com/blog/adventures/burning-man/
[guide]: https://learn.adafruit.com/adafruit-neopixel-uberguide/overview
[1092]: https://www.adafruit.com/products/1092
[2048]: https://www.adafruit.com/products/2048
[562]: https://www.adafruit.com/products/562
[1138]: https://www.adafruit.com/products/1138
[368]: https://www.adafruit.com/products/368
[261]: https://www.adafruit.com/products/261
[658]: https://www.adafruit.com/products/658
[659]: https://www.adafruit.com/products/659
[1063]: https://www.adafruit.com/products/1063
[1589]: https://www.adafruit.com/products/1589
[amplitie]: https://learn.adafruit.com/led-ampli-tie
[367]: https://www.adafruit.com/products/367
