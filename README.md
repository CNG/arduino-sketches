# Arduino sketches

Now that a few years of working with [Arduino](https://www.arduino.cc/) have flown by, it is time to get organized. As I learn and try new techniques, I will gather working examples in the [**techniques**](techniques) folder to use as building blocks for future projects. Similarly, my various projects live in the [**projects**](projects) folder. Of course I have not done this consistently from the beginning, but what is life for if not self improvement?

Most of my work so far has involved LEDs and signal processing for sound reactivity. I am indebted to [Burning Man 2013](https://votecharlie.com/blog/2013/09/burning-man-night.html) for opening my eyes to the accessibility of working with electronics to make art and to [Adafruit](https://learn.adafruit.com/) for some great tutorials that helped unleash the monster and drain my bank account.

## Projects

The below animations link to their code. The projects evolve, but I will try to create new ones if they diverge too much from the images. See the [**projects**](projects) folder for the complete list. Sometimes I also write about and post videos of these projects [on my blog](https://votecharlie.com/blog/technology/arduino/).

| [![ceiling](/../files/images/ceiling-444.gif?raw=true "ceiling")](projects/ceiling) | [![coat](/../files/images/coat-444.gif?raw=true "coat")](projects/coat) |
| - | - |
| [![hood](/../files/images/hood-444.gif?raw=true "hood")](projects/hood5th_hat_cool_test) | [![bike 2016](/../files/images/bike-444.gif?raw=true "bike 2016")](projects/bike2016) |
| [![cube](/../files/images/cube-444.gif?raw=true "cube")](projects/cube) | [![hat](/../files/images/hat-444.gif?raw=true "hat")](projects/hat_cool) |
| [![umbrella](/../files/images/umbrella-444.gif?raw=true "umbrella")](projects/canopy) | [![random patterns](/../files/images/boa-444.gif?raw=true "random patterns")](projects/random_patterns) |

## Techniques

Proofs of concept or small examples that can be incorporated into other projects, organized by chip type.

**ATmega32u4** ([datasheet](http://www.atmel.com/devices/atmega32u4.aspx))

* [using both external interrupt and potentiometer](techniques/external_interrupt_and_potentiometer)
* [recording amplitudes to file for analyzing microphone performance](techniques/sound_readings)
* [testing predicability of interrupt based button controls](techniques/testing_interrupt)
* [quick rainbow test on matrix](techniques/matrix_rainbow)

**SAM3X8E ARM Cortex-M3 CPU** ([datasheet](http://www.atmel.com/Images/Atmel-11057-32-bit-Cortex-M3-Microcontroller-SAM3X-SAM3A_Datasheet.pdf))

* [testing speed of serial writes](techniques/serial_speed_test_arm)
* [recording amplitudes to file for analyzing microphone performance](techniques/sound_readings_arm)

    | Visualizing data quality | ([spreadsheet](/../files/spreadsheets/sound_readings_arm.xlsx?raw=true)) |
    | - | - |
    | [![sound readings on ARM](/../files/screenshots/sound_readings_arm_1.png?raw=true "sound readings on ARM")](techniques/fht_readings_arm) | [![sound readings on ARM](/../files/screenshots/sound_readings_arm_2.png?raw=true "sound readings on ARM")](techniques/fht_readings_arm) |

* [beat detection: calculate RMS value for short and long window](techniques/calc_short_long_window_rms)
* [beat detection: separating sound into frequencies using fast Hartley transform](techniques/fht_readings_arm)

    | Adapted to 32x8 grid | Raw values ([spreadsheet](/../files/spreadsheets/fht_readings_arm.xlsx?raw=true)) |
    | - | - |
    | [![fast Hartley transform](/../files/images/fft-444.gif?raw=true "fast Hartley transform")](techniques/fht_readings_arm) | <img src="/../files/screenshots/fht_readings_arm.png?raw=true" width="350" title="fast Hartley transform"> |

* [fast Hartley transform display on matrix](techniques/fht_matrix_display)
