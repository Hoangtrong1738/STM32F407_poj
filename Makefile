All:
	arm-none-eabi-gcc -c Src\test_timer.c -mcpu=cortex-m4 -mthumb -std=gnu11 -Idriver\inc -Idriver\Mylib\Inc -IInc -o build\test_timer.o
	arm-none-eabi-gcc -c driver\scr\clock.c -mcpu=cortex-m4 -mthumb -std=gnu11 -Idriver\inc -Idriver\Mylib\Inc -IInc -o build\clock.o
	arm-none-eabi-gcc -c driver\scr\gpio.c -mcpu=cortex-m4 -mthumb -std=gnu11 -Idriver\inc -Idriver\Mylib\Inc -IInc -o build\gpio.o
	arm-none-eabi-gcc -c driver\scr\i2c.c -mcpu=cortex-m4 -mthumb -std=gnu11 -Idriver\inc -Idriver\Mylib\Inc -IInc -o build\i2c.o
	arm-none-eabi-gcc -c driver\scr\spi.c -mcpu=cortex-m4 -mthumb -std=gnu11 -Idriver\inc -Idriver\Mylib\Inc -IInc -o build\spi.o
	arm-none-eabi-gcc -c driver\Mylib\Src\motor.c -mcpu=cortex-m4 -mthumb -std=gnu11 -Idriver\inc -Idriver\Mylib\Inc -IInc -o build\motor.o
	arm-none-eabi-gcc -c driver\Mylib\Src\delay.c -mcpu=cortex-m4 -mthumb -std=gnu11 -Idriver\inc -Idriver\Mylib\Inc -IInc -o build\delay.o
	arm-none-eabi-gcc -c driver\scr\uart.c -mcpu=cortex-m4 -mthumb -std=gnu11 -Idriver\inc -Idriver\inc -IInc -o build\uart.o
	arm-none-eabi-gcc -c driver\Mylib\Src\encoder.c -mcpu=cortex-m4 -mthumb -std=gnu11 -Idriver\inc -Idriver\Mylib\Inc -IInc -o build\encoder.o
	arm-none-eabi-gcc -c driver\Mylib\Src\pid.c -mcpu=cortex-m4 -mthumb -std=gnu11 -Idriver\inc -Idriver\Mylib\Inc -IInc -o build\pid.o
	arm-none-eabi-gcc -c driver\Mylib\Src\control.c -mcpu=cortex-m4 -mthumb -std=gnu11 -Idriver\inc -Idriver\Mylib\Inc -IInc -o build\control.o
	arm-none-eabi-gcc -c driver\Mylib\Src\kinematics.c -mcpu=cortex-m4 -mthumb -std=gnu11 -Idriver\inc -Idriver\Mylib\Inc -IInc -o build\kinematics.o
	arm-none-eabi-gcc -x assembler-with-cpp -c Startup\startup_stm32f407vetx.s -mcpu=cortex-m4 -mthumb -o build\startup.o
	
	arm-none-eabi-gcc build/test_timer.o build/clock.o build/gpio.o build/i2c.o build/spi.o build/delay.o build\kinematics.o build/uart.o build\control.o build/motor.o build\encoder.o build\pid.o build/startup.o -mcpu=cortex-m4 -mthumb -T"STM32F407VETX_FLASH.ld" -Wl,-Map="file.map" -Wl,--gc-sections -static --specs=nosys.specs -o build/blink_led.elf
	arm-none-eabi-objcopy -O ihex build/blink_led.elf build/blink_led.hex
	arm-none-eabi-objcopy -O binary build/blink_led.elf build/blink_led.bin

clean:
	del /Q build\*.o
	del /Q build\*.elf
	del /Q build\*.hex
	del /Q build\*.bin
	del /Q build\*.map

Flash:
	STM32_Programmer_CLI -c port=SWD mode=UR -w build\blink_led.hex -v -rst
	
	 