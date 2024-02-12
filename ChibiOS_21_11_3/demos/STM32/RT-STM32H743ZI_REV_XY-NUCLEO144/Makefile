##############################################################################
# Build global options
# NOTE: Can be overridden externally.
#

# Compiler options here.
ifeq ($(USE_OPT),)
  USE_OPT = -O2 -ggdb -fomit-frame-pointer -falign-functions=16
endif

# C specific options here (added to USE_OPT).
ifeq ($(USE_COPT),)
  USE_COPT = 
endif

# C++ specific options here (added to USE_OPT).
ifeq ($(USE_CPPOPT),)
  USE_CPPOPT = -fno-rtti
endif

# Enable this if you want the linker to remove unused code and data.
ifeq ($(USE_LINK_GC),)
  USE_LINK_GC = yes
endif

# Linker extra options here.
ifeq ($(USE_LDOPT),)
  USE_LDOPT = 
endif

# Enable this if you want link time optimizations (LTO).
ifeq ($(USE_LTO),)
  USE_LTO = yes
endif

# Enable this if you want to see the full log while compiling.
ifeq ($(USE_VERBOSE_COMPILE),)
  USE_VERBOSE_COMPILE = no
endif

# If enabled, this option makes the build process faster by not compiling
# modules not used in the current configuration.
ifeq ($(USE_SMART_BUILD),)
  USE_SMART_BUILD = yes
endif

#
# Build global options
##############################################################################

##############################################################################
# Architecture or project specific options
#

# Stack size to be allocated to the Cortex-M process stack. This stack is
# the stack used by the main() thread.
ifeq ($(USE_PROCESS_STACKSIZE),)
  USE_PROCESS_STACKSIZE = 0x800
endif

# Stack size to the allocated to the Cortex-M main/exceptions stack. This
# stack is used for processing interrupts and exceptions.
ifeq ($(USE_EXCEPTIONS_STACKSIZE),)
  USE_EXCEPTIONS_STACKSIZE = 0x800
endif

# Enables the use of FPU (no, softfp, hard).
ifeq ($(USE_FPU),)
  USE_FPU = hard
endif

# FPU-related options.
ifeq ($(USE_FPU_OPT),)
  USE_FPU_OPT = -mfloat-abi=$(USE_FPU) -mfpu=fpv5-sp-d16
endif

#
# Architecture or project specific options
##############################################################################

##############################################################################
# Project, target, sources and paths
#

# Define project name here
PROJECT = vesc

# Target settings.
MCU  = cortex-m7

# Imported source files and paths.
CHIBIOS  := ../../..
CONFDIR  := ./cfg
BUILDDIR := ./build
DEPDIR   := ./.dep

# Licensing files.
include $(CHIBIOS)/os/license/license.mk
# Startup files.
include $(CHIBIOS)/os/common/startup/ARMCMx/compilers/GCC/mk/startup_stm32h7xx.mk
# HAL-OSAL files (optional).
include $(CHIBIOS)/os/hal/hal.mk
include $(CHIBIOS)/os/hal/ports/STM32/STM32H7xx/platform.mk
include $(CHIBIOS)/os/hal/boards/ST_NUCLEO144_H743ZI/board.mk
include $(CHIBIOS)/os/hal/osal/rt-nil/osal.mk
# RTOS files (optional).
include $(CHIBIOS)/os/rt/rt.mk
include $(CHIBIOS)/os/common/ports/ARMv7-M/compilers/GCC/mk/port.mk
# Auto-build files in ./source recursively.
include $(CHIBIOS)/tools/mk/autobuild.mk
# Other files (optional).
include $(CHIBIOS)/os/test/test.mk
include $(CHIBIOS)/test/rt/rt_test.mk
include $(CHIBIOS)/test/oslib/oslib_test.mk
include hwconf/hwconf.mk
include applications/applications.mk
include nrf/nrf.mk
include libcanard/canard.mk
include imu/imu.mk
include lora/lora.mk
include lzo/lzo.mk
include blackmagic/blackmagic.mk
include encoder/encoder.mk

ifeq ($(USE_LISPBM),1)
  include lispBM/lispbm.mk
  USE_OPT += -DUSE_LISPBM
endif

# Define linker script file here
LDSCRIPT= $(STARTUPLD)/STM32H743xI.ld

# C sources that can be compiled in ARM or THUMB mode depending on the global
# setting.
CSRC = $(ALLCSRC) \
       $(CHIBIOS)/os/hal/lib/streams/chprintf.c \
       $(CHIBIOS)/os/various/syscalls.c \
       board.c \
       main.c \
       comm_usb_serial.c \
       irq_handlers.c \
       buffer.c \
       comm_usb.c \
       crc.c \
       digital_filter.c \
       ledpwm.c \
       mcpwm.c \
       servo_dec.c \
       utils_math.c \
       utils_sys.c \
       servo_simple.c \
       packet.c \
       terminal.c \
       conf_general.c \
       eeprom.c \
       commands.c \
       timeout.c \
       comm_can.c \
       flash_helper.c \
       mc_interface.c \
       mcpwm_foc.c \
       gpdrive.c \
       confgenerator.c \
       timer.c \
       i2c_bb.c \
       spi_bb.c \
       virtual_motor.c \
       shutdown.c \
       mempools.c \
       worker.c \
       bms.c \
       events.c \
       $(HWSRC) \
       $(APPSRC) \
       $(NRFSRC) \
       $(CANARDSRC) \
       $(IMUSRC) \
       $(LORASRC) \
       $(LZOSRC) \
       $(BLACKMAGICSRC) \
       qmlui/qmlui.c \
       $(ENCSRC) \
       foc_math.c
	   
ifeq ($(USE_LISPBM),1)
  CSRC += $(LISPBMSRC)
endif

# C++ sources that can be compiled in ARM or THUMB mode depending on the global
# setting.
CPPSRC = $(ALLCPPSRC)

# List ASM source files here.
ASMSRC = $(ALLASMSRC)

# List ASM with preprocessor source files here.
ASMXSRC = $(ALLXASMSRC)

# Inclusion directories.
INCDIR = $(ALLINC)$(CONFDIR) \
         $(CHIBIOS)/os/various \
         $(CHIBIOS)/os/hal/lib/streams \
         mcconf \
         appconf \
         $(HWINC) \
         $(APPINC) \
         $(NRFINC) \
         $(CANARDINC) \
         $(IMUINC) \
         $(LORAINC) \
         $(LZOINC) \
         $(BLACKMAGICINC) \
         qmlui \
         qmlui/hw \
         qmlui/app \
         $(ENCINC)
		 
ifeq ($(USE_LISPBM),1)
  INCDIR += $(LISPBMINC)
endif

ifdef app_custom_mkfile
include $(app_custom_mkfile)
endif

# Define C warning options here.
CWARN = -Wall -Wextra -Wundef -Wstrict-prototypes -Wshadow

# Define C++ warning options here.
CPPWARN = -Wall -Wextra -Wundef

#
# Project, target, sources and paths
##############################################################################

##############################################################################
# Start of user section
#

# List all user C define here, like -D_DEBUG=1
UDEFS = 

# Define ASM defines here
UADEFS =

# List all user directories here
UINCDIR =

# List the user directory to look for the libraries here
ULIBDIR =

# List all user libraries here
ULIBS = -lm

ifeq ($(USE_FWLIB),yes)
  include $(CHIBIOS)/ext/stdperiph_stm32f4/stm32lib.mk
  CSRC += $(STM32SRC)
  INCDIR += $(STM32INC)
  USE_OPT += -DUSE_STDPERIPH_DRIVER
endif

#
# End of user section
##############################################################################

##############################################################################
# Common rules
#

RULESPATH = $(CHIBIOS)/os/common/startup/ARMCMx/compilers/GCC/mk
include $(RULESPATH)/arm-none-eabi.mk
include $(RULESPATH)/rules.mk

#
# Common rules
##############################################################################

##############################################################################
# Custom rules
#

build/$(PROJECT)/$(PROJECT).bin: build/$(PROJECT)/$(PROJECT).elf
	$(BIN) build/$(PROJECT)/$(PROJECT).elf build/$(PROJECT)/$(PROJECT).bin --gap-fill 0xFF

#
# Custom rules
##############################################################################
