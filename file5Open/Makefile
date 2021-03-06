###############################################################################
#                                                                             #
#                                  FIXME's                                    #
#                                                                             #
#   Fill in these variables with system & application-specific information.   #
#                                                                             #
###############################################################################

# FIXME directory of Popcorn compiler installation
POPCORN := /usr/local/popcorn

# FIXME directory of Popcorn compiler repository
# TODO remove
POPCORN_REPO := ~/popcorn-compiler-arm-x86

# FIXME directory of libgcc & libgcc_eh for aarch64 compiler
ARM64_LIBGCC := /usr/lib/gcc-cross/aarch64-linux-gnu/4.8

# FIXME directory of libgcc & libgcc_eh for x86-64 compiler
X86_64_LIBGCC := /usr/lib/gcc/x86_64-linux-gnu/4.8

# FIXME name of the generated executable
BIN := file

# FIXME specify source files
#SRC := $(shell ls *.c)
SRC := file.c

###############################################################################
#                                 Locations                                   #
###############################################################################

APP_DIR     := $(shell pwd)
ALIGN_CHECK := $(POPCORN_REPO)/util/scripts/check-align.py
SCRIPTS     := $(POPCORN)/lib/scripts

###############################################################################
#                  Compiler toolchain & command-line flags                    #
###############################################################################

ifeq ($(num_threads),)
num_threads := 1
endif

# Compiler
CC         := clang
CXX        := clang++
CFLAGS     := -O0 -Wall -g -gdwarf-aranges -c -fopenmp=libomp -DPOSIX -DBOMP 
	#\
              #-DTUSED=$(num_threads)
HET_CFLAGS := $(CFLAGS) -nostdinc -ffunction-sections -fdata-sections \
              -finstrument-functions -fno-common -ftls-model=initial-exec \
              -fno-vectorize -Wno-override-module -mllvm -optimize-regalloc

# Optimization Middle-End
OPT       := opt
OPT_FLAGS := -O3 -disable-loop-vectorization -disable-slp-vectorization \
             -load $(POPCORN)/lib/libAssociateLiteral.so -associate-literal \
             -load $(POPCORN)/lib/libSectionStatic.so -section-static \
             -load $(POPCORN)/lib/libLiveValues.so \
             -load $(POPCORN)/lib/libStackInfo.so -stack-info

# Linker
LD       := $(POPCORN)/bin/ld.gold
LDFLAGS  := -z relro --hash-style=gnu --build-id -static
LIBS     := /lib/crt1.o \
            /lib/libc.a \
            /lib/libmigrate.a \
            /lib/libstack-transform.a \
            /lib/libdwarf.a \
            /lib/libelf.a \
            /lib/libpthread.a \
            /lib/libbomp.a \
            /lib/libc.a \
            /lib/libm.a
LD_EXTRA := --start-group -lgcc -lgcc_eh --end-group

# Alignment
# TODO remove dependencies on external scripting
ALIGN         := align
ALIGN_BUILD   := align
ALIGN_SCRIPTS := $(ALIGN_BUILD)/readMyElfToFile.sh \
                 $(ALIGN_BUILD)/nmScript.sh \
                 $(ALIGN_BUILD)/mlink_armObjs.sh \
                 $(ALIGN_BUILD)/mlink_x86Objs.sh \
                 $(ALIGN_BUILD)/aarch64linux.x \
                 $(ALIGN_BUILD)/elf_x86_64.x
ALIGN_BINS    := $(ALIGN_BUILD)/armexe_musl $(ALIGN_BUILD)/x86exe_musl
ALIGN_MAPS    := $(ALIGN_BUILD)/map_aarch.txt $(ALIGN_BUILD)/map_x86.txt
ALIGN_OBJ     := $(ALIGN_BUILD)/obj_arm.txt $(ALIGN_BUILD)/obj_x86.txt
ALIGN_SYMBOLS := $(ALIGN_BUILD)/nm_symbols_aarch64.txt \
                 $(ALIGN_BUILD)/nm_symbols_x86.txt

# Post-processing
POST_PROCESS := $(POPCORN)/bin/gen-stackinfo

##########
# Common #
##########

IR_BUILD := ir
IR       := $(addprefix $(IR_BUILD)/,$(SRC:.c=.bc))

###########
# AArch64 #
###########

# Locations
ARM64_POPCORN := $(POPCORN)/aarch64
ARM64_BUILD   := build_aarch64

# Generated files
ARM64_ALIGNED   := $(BIN)_aarch64
ARM64_VANILLA   := $(ARM64_BUILD)/$(ARM64_ALIGNED)
ARM64_OBJ       := $(subst $(IR_BUILD),$(ARM64_BUILD),$(IR:.bc=.o))
ARM64_MAP       := $(ARM64_BUILD)/map.txt
ARM64_LD_SCRIPT := $(ALIGN_BUILD)/modified__aarch64.x

# Flags
ARM64_TARGET  := aarch64-linux-gnu
ARM64_INC     := -isystem $(ARM64_POPCORN)/include
ARM64_LDFLAGS := -m aarch64linux -L$(ARM64_POPCORN)/lib -L$(ARM64_LIBGCC) \
                 $(addprefix $(ARM64_POPCORN),$(LIBS)) $(LD_EXTRA)

##########
# x86-64 #
##########

# Locations
X86_64_POPCORN  := $(POPCORN)/x86_64
X86_64_BUILD    := build_x86-64
X86_64_SD_BUILD := sd_x86-64

# Generated files
X86_64_ALIGNED   := $(BIN)_x86-64
X86_64_VANILLA   := $(X86_64_BUILD)/$(X86_64_ALIGNED)
X86_64_OBJ       := $(subst $(IR_BUILD),$(X86_64_BUILD),$(IR:.bc=.o))
X86_64_MAP       := $(X86_64_BUILD)/map.txt
X86_64_LD_SCRIPT := $(ALIGN_BUILD)/modified__elf_x86_64.x
X86_64_SD        := $(X86_64_SD_BUILD)/$(X86_64_ALIGNED)
X86_64_SD_OBJ    := $(subst $(X86_64_BUILD),$(X86_64_SD_BUILD),$(X86_64_OBJ))

# Flags
X86_64_TARGET  := x86_64-linux-gnu
X86_64_INC     := -isystem $(X86_64_POPCORN)/include
X86_64_LDFLAGS := -m elf_x86_64 -L$(X86_64_POPCORN)/lib -L$(X86_64_LIBGCC) \
                  $(addprefix $(X86_64_POPCORN),$(LIBS)) $(LD_EXTRA)

###############################################################################
#                                 Recipes                                     #
###############################################################################

all: post_process

check: $(ARM64_ALIGNED) $(X86_64_ALIGNED)
	@echo " [CHECK] Checking alignment for $^"
	@$(ALIGN_CHECK) $(ARM64_ALIGNED) $(X86_64_ALIGNED)

post_process: $(ARM64_ALIGNED) $(X86_64_ALIGNED)
	@echo " [POST_PROCESS] $^"
	@$(POST_PROCESS) -f $(ARM64_ALIGNED)
	@$(POST_PROCESS) -f $(X86_64_ALIGNED)

stack-depth: $(X86_64_SD)

aligned: $(ARM64_ALIGNED) $(X86_64_ALIGNED)
aligned-aarch64: $(ARM64_ALIGNED)
aligned-x86-64: $(X86_64_ALIGNED)

vanilla: $(ARM64_VANILLA) $(X86_64_VANILLA)
vanilla-aarch64: $(ARM64_VANILLA)
vanilla-x86-64: $(X86_64_VANILLA)

clean:
	@echo " [CLEAN] $(ARM64_ALIGNED) $(ARM64_BUILD) $(X86_64_ALIGNED) $(X86_64_BUILD) $(X86_64_SD_BUILD) $(ALIGN_BUILD) $(IR_BUILD)"
	@rm -rf $(ARM64_ALIGNED) $(ARM64_BUILD) $(X86_64_ALIGNED) $(X86_64_BUILD) $(X86_64_SD_BUILD) $(ALIGN_BUILD) $(IR_BUILD)

%.dir:
	@echo " [MKDIR] $*"
	@mkdir -p $*
	@touch $@

# TODO remove dependencies on external scripting
$(ALIGN_BUILD)/readMyElfToFile.sh: $(SCRIPTS)/readMyElfToFile.sh $(ALIGN_BUILD)/.dir
	@cp $< $@
$(ALIGN_BUILD)/nmScript.sh: $(SCRIPTS)/nmScript.sh $(ALIGN_BUILD)/.dir
	@cp $< $@
$(ALIGN_BUILD)/mlink_armObjs.sh: $(SCRIPTS)/mlink_armObjs.sh $(ALIGN_BUILD)/.dir
	@cp $< $@
$(ALIGN_BUILD)/mlink_x86Objs.sh: $(SCRIPTS)/mlink_x86Objs.sh $(ALIGN_BUILD)/.dir
	@cp $< $@
$(ALIGN_BUILD)/aarch64linux.x: $(SCRIPTS)/aarch64linux.x $(ALIGN_BUILD)/.dir
	@cp $< $@
$(ALIGN_BUILD)/elf_x86_64.x: $(SCRIPTS)/elf_x86_64.x $(ALIGN_BUILD)/.dir
	@cp $< $@
$(ALIGN_BUILD)/armexe_musl: $(ARM64_VANILLA) $(ALIGN_BUILD)/.dir
	@cp $< $@
$(ALIGN_BUILD)/x86exe_musl: $(X86_64_VANILLA) $(ALIGN_BUILD)/.dir
	@cp $< $@
$(ALIGN_BUILD)/map_aarch.txt: $(ARM64_VANILLA) $(ALIGN_BUILD)/.dir
	@cp $(ARM64_MAP) $@
$(ALIGN_BUILD)/map_x86.txt: $(X86_64_VANILLA) $(ALIGN_BUILD)/.dir
	@cp $(X86_64_MAP) $@
$(ALIGN_BUILD)/obj_arm.txt: $(ARM64_OBJ) $(ALIGN_BUILD)/.dir
	@echo "$(addprefix $(APP_DIR)/,$(ARM64_OBJ))" > $@
$(ALIGN_BUILD)/obj_x86.txt: $(X86_64_OBJ) $(ALIGN_BUILD)/.dir
	@echo "$(addprefix $(APP_DIR)/,$(X86_64_OBJ))" > $@
$(ALIGN_BUILD)/nm_symbols_aarch64.txt: $(ARM64_OBJ) $(ALIGN_BUILD)/.dir
	@for f in $(ARM64_OBJ); do aarch64-linux-gnu-nm --numeric-sort $$f >> $@; done
$(ALIGN_BUILD)/nm_symbols_x86.txt: $(X86_64_OBJ) $(ALIGN_BUILD)/.dir
	@for f in $(X86_64_OBJ); do nm --numeric-sort $$f >> $@; done

##########
# Common #
##########

$(IR_BUILD)/%.bc: %.c
	@echo " [CC] $<"
	@$(CC) -target $(ARM64_TARGET) $(HET_CFLAGS) -emit-llvm $(ARM64_INC) -o $@ $<
	@$(OPT) $(OPT_FLAGS) $@ -o $@

###########
# AArch64 #
###########

$(ARM64_BUILD)/%.o: $(IR_BUILD)/%.bc
	@echo " [CC (aarch64)] $<"
	@$(CC) -target $(ARM64_TARGET) $(HET_CFLAGS) -o $@ $<

$(ARM64_VANILLA): $(IR_BUILD)/.dir $(ARM64_BUILD)/.dir $(ARM64_OBJ)
	@echo " [LD] $@ (vanilla)"
	@$(LD) -o $@ $(ARM64_OBJ) $(LDFLAGS) $(ARM64_LDFLAGS) -Map $(ARM64_MAP)

$(ARM64_LD_SCRIPT): $(ALIGN_BUILD)/.dir \
                    $(ALIGN_SCRIPTS) \
                    $(ALIGN_BINS) \
                    $(ALIGN_MAPS) \
                    $(ALIGN_OBJ) \
                    $(ALIGN_SYMBOLS)
	@echo " [ALIGN] $@"
	@$(ALIGN) 1 $(APP_DIR) -skip-vanilla > $(APP_DIR)/$(ALIGN_BUILD)/align.txt
	@if [ "`grep 'FAILURE' $(APP_DIR)/$(ALIGN_BUILD)/align.txt`" != "" ]; then \
		echo "Align failed -- see $(ALIGN_BUILD)/align.txt"; \
		false; \
	fi

$(ARM64_ALIGNED): $(ARM64_LD_SCRIPT)
	@echo " [LD] $@ (aligned)"
	@$(LD) -o $@ $(ARM64_OBJ) $(LDFLAGS) $(ARM64_LDFLAGS) -Map $(ARM64_MAP) -T $<

##########
# x86-64 #
##########

$(X86_64_BUILD)/%.o: $(IR_BUILD)/%.bc
	@echo " [CC (x86-64)] $<"
	@$(CC) -target $(X86_64_TARGET) $(HET_CFLAGS) -o $@ $<

$(X86_64_VANILLA): $(IR_BUILD)/.dir $(X86_64_BUILD)/.dir $(X86_64_OBJ)
	@echo " [LD] $@ (vanilla)"
	@$(LD) -o $@ $(X86_64_OBJ) $(LDFLAGS) $(X86_64_LDFLAGS) -Map $(X86_64_MAP)

$(X86_64_LD_SCRIPT): $(ALIGN_BUILD)/.dir \
                     $(ALIGN_SCRIPTS) \
                     $(ALIGN_BINS) \
                     $(ALIGN_MAPS) \
                     $(ALIGN_OBJ) \
                     $(ALIGN_SYMBOLS) \
                     $(ARM64_LD_SCRIPT)
	@echo " [ALIGN] $@"

$(X86_64_ALIGNED): $(X86_64_LD_SCRIPT)
	@echo " [LD] $@ (aligned)"
	@$(LD) -o $@ $(X86_64_OBJ) $(LDFLAGS) $(X86_64_LDFLAGS) -Map $(X86_64_MAP) -T $<

# Stack-depth builds
$(X86_64_SD_BUILD)/%.o: %.c
	@echo " [CC (x86-64)] $< (stack depth)"
	@$(CC) -target $(X86_64_TARGET) $(HET_CFLAGS) $(X86_64_INC) -o $@ $<

$(X86_64_SD): $(X86_64_SD_BUILD)/.dir $(X86_64_SD_OBJ)
	@echo " [LD] $@ (stack depth)"
	@$(CXX) -static -L$(POPCORN)/lib -o $@ $(X86_64_SD_OBJ) -lstack-depth -lbomp -lpthread

.PHONY: all post_process stack-depth clean \
        aligned aligned-aarch64 aligned-x86-64 \
        vanilla vanilla-aarch64 vanilla-x86-64
.SECONDARY: $(IR)
