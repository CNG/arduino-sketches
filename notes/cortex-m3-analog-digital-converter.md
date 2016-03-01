# SAM3X8E ARM Cortex-M3 analog to digital converter

To use the code from [http://forum.arduino.cc/index.php/topic,140107.0.html](http://forum.arduino.cc/index.php/topic,140107.0.html):

		➜  ~ cd ~/Library/Arduino15
		➜  Arduino15 find . -name "libarm_cortexM3l_math.a"
		./packages/arduino/hardware/sam/1.6.6/system/CMSIS/CMSIS/Lib/GCC/libarm_cortexM3l_math.a
		➜  Arduino15 cd ./packages/arduino/hardware/sam/1.6.6/
		➜  1.6.6 find . -name "libarm_cortexM3l_math.a"
		./system/CMSIS/CMSIS/Lib/GCC/libarm_cortexM3l_math.a
		➜  1.6.6 cp ./system/CMSIS/CMSIS/Lib/GCC/libarm_cortexM3l_math.a variants/arduino_due_x

Then make the edit adding `{build.variant.path}/libarm_cortexM3l_math.a` to `platform.txt` per comment from cmaglie.

## Resources

* [project using peripheral DMA](https://github.com/stimmer/DueVGA/blob/master/extras/Waterfall/waterfall/waterfall.ino)
* [SAM3S4 datasheet](http://www.atmel.com/Images/Atmel-6500-32-bit-Cortex-M3-Microcontroller-SAM3S4-SAM3S2-SAM3S1_Datasheet.pdf)
* [Analog-to-Digital Converter in the SAM3S4](http://www.atmel.com/Images/doc11106.pdf)
