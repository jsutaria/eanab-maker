NAME = main
OBJECTS = src/common/communicator.o src/common/magstripe.o src/common/storage.o src/common/stepper_driver.o src/common/valve.o src/common/mcp3008.o src/common/button.o src/common/breathalyzer.o


CFLAGS  = -Iinclude -I$(CS107E)/include -g -Wall -Wpointer-arith
CFLAGS += -Og -std=c99 -ffreestanding
CFLAGS += -mapcs-frame -fno-omit-frame-pointer -mpoke-function-name
LDFLAGS = -nostdlib -T memmap -L. -L$(CS107E)/lib
LDLIBS  = -lpi -lpiextra -lgcc

all : $(NAME).bin

%.bin: %.elf
	arm-none-eabi-objcopy $< -O binary $@

%.elf: %.o $(OBJECTS) start.o cstart.o
	arm-none-eabi-gcc $(LDFLAGS) $^ $(LDLIBS) -o $@

%.o: %.c
	arm-none-eabi-gcc $(CFLAGS) -c $< -o $@

%.o: %.s
	arm-none-eabi-as $(ASFLAGS) $< -o $@

%.list: %.o
	arm-none-eabi-objdump  --no-show-raw-insn -d $< > $@

install-m: src/maker/$(NAME).bin
	rpi-install.py -p $<

install-c: src/concierge/$(NAME).bin
	rpi-install.py -p $<

install-t: src/test/$(NAME).bin
	rpi-install.py -p $<

clean:
	rm -f *.o *.bin *.elf *.list *~
	rm -f src/common/*.o src/common/*.bin src/common/*.elf src/common/*.list src/common/*~

.PHONY: all clean install-m install-c install-t

.PRECIOUS: %.o %.elf

# empty recipe used to disable built-in rules for native build
%:%.c
%:%.o

define CS107E_ERROR_MESSAGE
ERROR - CS107E environment variable is not set.

Please set it to point to the `cs107e.github.io/cs107e` directory using the
command `export CS107E=<replace with path to your cs107e.github.io directory>/cs107e`.

To have this done automatically, add the above command to your shell
environment configuration file (e.g. ~/.bashrc)
endef

ifndef CS107E
$(error $(CS107E_ERROR_MESSAGE))
endif
