####################################################################
# Makefile                                                         #
####################################################################

.SUFFIXES:				# ignore builtin rules
.PHONY: all debug release clean

####################################################################
# Definitions                                                      #
####################################################################

# uniq is a function which remove duplicate elements from a list
uniq = $(strip $(if $1,$(firstword $1) \
       $(call uniq,$(filter-out $(firstword $1),$1))))

DEVICE = EFM32LG990F256
PROJECTNAME = LG990_LSM6DS3

ifneq ($(filter $(MAKECMDGOALS),release),)
  OBJ_DIR = build/Release/build
  EXE_DIR = build/Release/exe
  LST_DIR = build/Release/lst
else
  OBJ_DIR = build/Debug/build
  EXE_DIR = build/Debug/exe
  LST_DIR = build/Debug/lst
endif

####################################################################
# Definitions of toolchain.                                        #
# You might need to do changes to match your system setup          #
####################################################################

# Change path to the tools according to your system configuration
# DO NOT add trailing whitespace chars, they do matter!
SIMPLICITY_DIR ?= $(HOME)/Downloads/SimplicityStudio_v4
TOOLDIR ?= $(SIMPLICITY_DIR)/developer/toolchains/gnu_arm/7.2_2017q4
SDK_DIR ?= $(SIMPLICITY_DIR)/developer/sdks/gecko_sdk_suite/v2.6

RMDIRS     := rm -rf
RMFILES    := rm -rf
MKDIR      := mkdir -p
ALLFILES   := /*.*
NULLDEVICE := /dev/null
SHELLNAMES := $(ComSpec)$(COMSPEC)
PATHSEP    := /

# Command output is hidden by default, it can be enabled by
# setting VERBOSE=true on the commandline.
ifeq ($(VERBOSE),)
  ECHO = @
endif

# Create directories and do a clean which is compatible with parallell make
$(shell $(MKDIR) $(subst /,$(PATHSEP),$(OBJ_DIR))>$(NULLDEVICE) 2>&1)
$(shell $(MKDIR) $(subst /,$(PATHSEP),$(EXE_DIR))>$(NULLDEVICE) 2>&1)
$(shell $(MKDIR) $(subst /,$(PATHSEP),$(LST_DIR))>$(NULLDEVICE) 2>&1)
ifeq (clean,$(findstring clean, $(MAKECMDGOALS)))
  ifneq ($(filter $(MAKECMDGOALS),all debug release),)
    $(shell $(RMFILES) $(subst /,$(PATHSEP),$(OBJ_DIR)$(ALLFILES))>$(NULLDEVICE) 2>&1)
    $(shell $(RMFILES) $(subst /,$(PATHSEP),$(EXE_DIR)$(ALLFILES))>$(NULLDEVICE) 2>&1)
    $(shell $(RMFILES) $(subst /,$(PATHSEP),$(LST_DIR)$(ALLFILES))>$(NULLDEVICE) 2>&1)
  endif
endif

CC      = $(QUOTE)$(TOOLDIR)/bin/arm-none-eabi-gcc$(QUOTE)
CXX     = $(QUOTE)$(TOOLDIR)/bin/arm-none-eabi-g++$(QUOTE)
LD      = $(QUOTE)$(TOOLDIR)/bin/arm-none-eabi-ld$(QUOTE)
AR      = $(QUOTE)$(TOOLDIR)/bin/arm-none-eabi-ar$(QUOTE)
OBJCOPY = $(QUOTE)$(TOOLDIR)/bin/arm-none-eabi-objcopy$(QUOTE)
DUMP    = $(QUOTE)$(TOOLDIR)/bin/arm-none-eabi-objdump$(QUOTE)

####################################################################
# Flags                                                            #
####################################################################

# -MMD : Don't generate dependencies on system header files.
# -MP  : Add phony targets, useful when a h-file is removed from a project.
# -MF  : Specify a file to write the dependencies to.
DEPFLAGS = -MMD -MP -MF $(@:.o=.d)

#
# Add -Wa,-ahld=$(LST_DIR)/$(@F:.o=.lst) to CFLAGS to produce assembly list files
#
override CFLAGS += -D$(DEVICE) -Wall -Wextra -mcpu=cortex-m3 -mthumb \
-mfix-cortex-m3-ldrd -ffunction-sections \
-fdata-sections -fomit-frame-pointer -std=c99 -DDEBUG_EFM  \
$(DEPFLAGS)

override CXXFLAGS += -D$(DEVICE) -Wall -Wextra -mcpu=cortex-m3 -mthumb \
-mfix-cortex-m3-ldrd -fno-builtin -fno-rtti -fno-exceptions -std=c++11 \
-ffunction-sections -fdata-sections -fomit-frame-pointer -DDEBUG_EFM  \
$(DEPFLAGS)

override ASMFLAGS += -x assembler-with-cpp -D$(DEVICE) -Wall -Wextra -mcpu=cortex-m3 -mthumb -DDEBUG_EFM  $(DEPFLAGS)

#
# NOTE: The -Wl,--gc-sections flag may interfere with debugging using gdb.
#
override LDFLAGS += -Xlinker -Map=$(LST_DIR)/$(PROJECTNAME).map -mcpu=cortex-m3 \
-mthumb -T$(SDK_DIR)/platform/Device/SiliconLabs/EFM32LG/Source/GCC/efm32lg.ld \
 --specs=nano.specs -Wl,--gc-sections 

LIBS = -Wl,--start-group -lgcc -lc -lnosys   -Wl,--end-group

INCLUDEPATHS += \
-I$(SDK_DIR)/platform/CMSIS/Include \
-I$(SDK_DIR)/platform/Device/SiliconLabs/EFM32LG/Include \
-I$(SDK_DIR)/platform/emlib/inc

####################################################################
# Files                                                            #
####################################################################

C_CXX_SRC += \
$(SDK_DIR)/platform/Device/SiliconLabs/EFM32LG/Source/system_efm32lg.c \
$(SDK_DIR)/platform/emlib/src/em_assert.c \
$(SDK_DIR)/platform/emlib/src/em_cmu.c \
$(SDK_DIR)/platform/emlib/src/em_core.c \
$(SDK_DIR)/platform/emlib/src/em_dma.c \
$(SDK_DIR)/platform/emlib/src/em_ebi.c \
$(SDK_DIR)/platform/emlib/src/em_emu.c \
$(SDK_DIR)/platform/emlib/src/em_gpio.c \
$(SDK_DIR)/platform/emlib/src/em_system.c \
$(SDK_DIR)/platform/emlib/src/em_usart.c \
src/main.c

S_SRC += $(SDK_DIR)/platform/Device/SiliconLabs/EFM32LG/Source/GCC/startup_efm32lg.S

C_SRC = $(filter %.c, $(C_CXX_SRC))
CXX_SRC = $(filter %.cpp, $(C_CXX_SRC))

####################################################################
# Rules                                                            #
####################################################################

C_FILES = $(notdir $(C_SRC) )
CXX_FILES = $(notdir $(CXX_SRC) )
S_FILES = $(notdir $(S_SRC) )
s_FILES = $(notdir $(s_SRC) )
#make list of source paths, uniq removes duplicate paths
C_PATHS = $(call uniq, $(dir $(C_SRC) ) )
CXX_PATHS = $(call uniq, $(dir $(CXX_SRC) ) )
S_PATHS = $(call uniq, $(dir $(S_SRC) $(s_SRC) ) )

C_OBJS = $(addprefix $(OBJ_DIR)/, $(C_FILES:.c=.o))
CXX_OBJS = $(addprefix $(OBJ_DIR)/, $(CXX_FILES:.cpp=.o))
S_OBJS = $(if $(S_SRC), $(addprefix $(OBJ_DIR)/, $(S_FILES:.S=.o)))
s_OBJS = $(if $(s_SRC), $(addprefix $(OBJ_DIR)/, $(s_FILES:.s=.o)))
C_DEPS = $(addprefix $(OBJ_DIR)/, $(C_FILES:.c=.d))
CXX_DEPS = $(addprefix $(OBJ_DIR)/, $(CXX_FILES:.cpp=.d))
S_DEPS = $(addprefix $(OBJ_DIR)/, $(S_FILES:.S=.d))
s_DEPS = $(addprefix $(OBJ_DIR)/, $(s_FILES:.s=.d))
OBJS = $(C_OBJS) $(CXX_OBJS) $(S_OBJS) $(s_OBJS)

vpath %.c $(C_PATHS)
vpath %.cpp $(CXX_PATHS)
vpath %.s $(S_PATHS)
vpath %.S $(S_PATHS)

# Default build is debug build
all:      debug

debug:    CFLAGS += -DDEBUG -O0 -g3 '-DEFM32LG990F256=1'
debug:    $(EXE_DIR)/$(PROJECTNAME).bin

release:  CFLAGS += -DNDEBUG -O2 -g3 '-DEFM32LG990F256=1'
release:  $(EXE_DIR)/$(PROJECTNAME).bin

# Create objects from C SRC files
$(OBJ_DIR)/%.o: %.c
	@echo "Building file: $<"
	$(ECHO)$(CC) $(CFLAGS) $(INCLUDEPATHS) -c -o $@ $<

# Create objects from CXX SRC files
$(OBJ_DIR)/%.o: %.cpp
	@echo "Building file: $<"
	$(ECHO)$(CXX) $(CXXFLAGS) $(INCLUDEPATHS) -c -o $@ $<

# Assemble .s/.S files
$(OBJ_DIR)/%.o: %.s
	@echo "Assembling $<"
	$(ECHO)$(CC) $(ASMFLAGS) $(INCLUDEPATHS) -c -o $@ $<

$(OBJ_DIR)/%.o: %.S
	@echo "Assembling $<"
	$(ECHO)$(CC) $(ASMFLAGS) $(INCLUDEPATHS) -c -o $@ $<

# Link
$(EXE_DIR)/$(PROJECTNAME).out: $(OBJS)
	@echo "Linking target: $@"
	$(ECHO)$(CC) $(LDFLAGS) $(OBJS) $(LIBS) -o $(EXE_DIR)/$(PROJECTNAME).out

# Create binary file
$(EXE_DIR)/$(PROJECTNAME).bin: $(EXE_DIR)/$(PROJECTNAME).out
	@echo "Creating binary file: $@"
	$(ECHO)$(OBJCOPY) -O binary $(EXE_DIR)/$(PROJECTNAME).out $(EXE_DIR)/$(PROJECTNAME).bin

# Calculate the size
	@echo '\n'
	@echo 'Running size tool'
	$(TOOLDIR)/bin/arm-none-eabi-size $(EXE_DIR)/$(PROJECTNAME).out
	@echo ' '

clean:
ifeq ($(filter $(MAKECMDGOALS),all debug release),)
	$(RMDIRS) build
endif

# include auto-generated dependency files (explicit rules)
ifneq (clean,$(findstring clean, $(MAKECMDGOALS)))
-include $(C_DEPS)
-include $(CXX_DEPS)
-include $(S_DEPS)
-include $(s_DEPS)
endif
