# Sources
SRCS = main.c stm32f4xx_it.c system_stm32f4xx.c utils.c

# Project name

PROJ_NAME=main
OUTPATH=build

# OpenOCD conf file
OPENOCD_CONFIG_PATH=/home/francesco/openocd/stm32f4discovery.cfg
OPENOCD_LOGFILE_PATH=/dev/null

STM_COMMON=orig

###################################################

# Check for valid float argument
# NOTE that you have to run make clan after
# changing these as hardfloat and softfloat are not
# binary compatible
ifneq ($(FLOAT_TYPE), hard)
	ifneq ($(FLOAT_TYPE), soft)
	override FLOAT_TYPE = hard
	#override FLOAT_TYPE = soft
endif
endif

###################################################

BINPATH=/usr/bin
CC=$(BINPATH)/arm-none-eabi-gcc
OBJCOPY=$(BINPATH)/arm-none-eabi-objcopy
SIZE=$(BINPATH)/arm-none-eabi-size

CFLAGS  = -std=gnu99 -g -O2 -Wall -Tstm32_flash.ld 
CFLAGS += -mlittle-endian -mthumb -mthumb-interwork -nostartfiles -mcpu=cortex-m4

SEMIHOSTING_FLAGS = --specs=rdimon.specs -lc -lrdimon 

ifeq ($(FLOAT_TYPE), hard)
	CFLAGS += -fsingle-precision-constant -Wdouble-promotion
	CFLAGS += -mfpu=fpv4-sp-d16 -mfloat-abi=hard
	#CFLAGS += -mfpu=fpv4-sp-d16 -mfloat-abi=softfp
else
	CFLAGS += -msoft-float
endif

###################################################

vpath %.c src
vpath %.a lib

ROOT=$(shell pwd)

# custom libraries
CFLAGS += -Iinc -Ilib -Ilib/inc 
CFLAGS += -Ilib/inc/core -Ilib/inc/peripherals 

# Include files from original STM libraries
#CFLAGS += -I$(STM_COMMON)/CMSIS/Include 
#CFLAGS += -I$(STM_COMMON)/Utilities/STM32F4-Discovery
#CFLAGS += -I$(STM_COMMON)/CMSIS/ST/STM32F4xx/Include
#CFLAGS += -I$(STM_COMMON)/STM32F4xx_StdPeriph_Driver/inc 

SRCS += lib/startup_stm32f4xx.s # add startup file to build

OBJS = $(SRCS:.c=.o)

###################################################

.PHONY: lib proj

all: lib proj
	$(SIZE) $(OUTPATH)/$(PROJ_NAME).elf

lib:
	$(MAKE) -C lib FLOAT_TYPE=$(FLOAT_TYPE)

proj: 	$(OUTPATH)/$(PROJ_NAME).elf

$(OUTPATH)/$(PROJ_NAME).elf: $(SRCS)
	$(CC) $(SEMIHOSTING_FLAGS) $(CFLAGS) $^ -o $@ -Llib -lstm32f4 -lm
	$(OBJCOPY) -O ihex $(OUTPATH)/$(PROJ_NAME).elf $(OUTPATH)/$(PROJ_NAME).hex
	$(OBJCOPY) -O binary $(OUTPATH)/$(PROJ_NAME).elf $(OUTPATH)/$(PROJ_NAME).bin

clean:
	rm -f *.o
	rm -f $(OUTPATH)/$(PROJ_NAME).elf
	rm -f $(OUTPATH)/$(PROJ_NAME).hex
	rm -f $(OUTPATH)/$(PROJ_NAME).bin
	$(MAKE) clean -C lib # Remove this line if you don't want to clean the libs as well

upload: lib proj flash

flash:
	arm-none-eabi-gdb -silent --command=gdb_commands $(OUTPATH)/$(PROJ_NAME).elf

gdb: lib proj flash debug

debug:
	arm-none-eabi-gdb --command=gdb_debug_commands $(OUTPATH)/$(PROJ_NAME).elf

openocd:
	openocd -f $(OPENOCD_CONFIG_PATH) #--log_output $(OPENOCD_LOGFILE_PATH) > $(OPENOCD_LOGFILE_PATH)
