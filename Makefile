#---------------------------------------------------------------------------------
# Clear the implicit built in rules
#---------------------------------------------------------------------------------
.SUFFIXES:
#---------------------------------------------------------------------------------
ifeq ($(strip $(DEVKITPPC)),)
$(error "Please set DEVKITPPC in your environment. export DEVKITPPC=<path to>devkitPPC")
endif

export	LIBOGC_INC	:=	$(DEVKITPRO)/libogc/include
export	LIBOGC_LIB	:=	$(DEVKITPRO)/libogc/lib/cube

include $(DEVKITPPC)/base_rules

MACHDEP =  -DGEKKO -mvgc -mcpu=750 -meabi -mhard-float

#---------------------------------------------------------------------------------
%.dol: %.elf
	@echo output ... $(notdir $@)
	@elf2dol $< $@

#---------------------------------------------------------------------------------
%.tpl : %.scf
	@echo $(notdir $<)
	@gxtexconv -s $< -d $(DEPSDIR)/$*.d -o $@

#---------------------------------------------------------------------------------
%.tpl.o	:	%.tpl
	@echo $(notdir $<)
	@$(bin2o)
	
#---------------------------------------------------------------------------------
%.elf:
	@echo linking ... $(notdir $@)
	@$(LD)  $^ $(LDFLAGS) $(LIBPATHS) $(LIBS) -o $@

	
#---------------------------------------------------------------------------------
# TARGET is the name of the output
# BUILD is the directory where object files & intermediate files will be placed
# SOURCES is a list of directories containing source code
# INCLUDES is a list of directories containing extra header files
#---------------------------------------------------------------------------------
TARGET		:=	megaloader
BUILD		:=	build
SOURCES		:=	source \
				source/aram \
				source/images \
				source/images/buttons
DATA		:=	data  
TEXTURES	:=	source/images \
				source/images/buttons
INCLUDES	:= 	include $(SOURCES)
#VERSION = _v1.3
#---------------------------------------------------------------------------------
# options for code generation
#---------------------------------------------------------------------------------

CFLAGS		= -g -O3 -Wall -fno-strict-aliasing $(MACHDEP) $(INCLUDE)
CXXFLAGS	= $(CFLAGS)

LDFLAGS		= -g $(MACHDEP) -Wl,-Map,$(notdir $@).map -Wl,--section-start,.init=0x80003100 -T../linker-script/vgcogc.ld

#---------------------------------------------------------------------------------
# any extra libraries we wish to link with the project
#---------------------------------------------------------------------------------
#LIBS	:=	-lbba -logc -lm -lfat -lmad -lasnd -lm -logc -ldb 
LIBS	:=  -logc -lm -lfat -lmad -lasnd -lm -logc -ldb -liso9660 

#---------------------------------------------------------------------------------
# list of directories containing libraries, this must be the top level containing
# include and lib
#---------------------------------------------------------------------------------
LIBDIRS	:=

#---------------------------------------------------------------------------------
# no real need to edit anything past this point unless you need to add additional
# rules for different file extensions
#---------------------------------------------------------------------------------
ifneq ($(BUILD),$(notdir $(CURDIR)))
#---------------------------------------------------------------------------------

export OUTPUT	:=	$(CURDIR)/$(TARGET)

export VPATH	:=	$(foreach dir,$(SOURCES),$(CURDIR)/$(dir)) \
					$(foreach dir,$(DATA),$(CURDIR)/$(dir)) \
					$(foreach dir,$(TEXTURES),$(CURDIR)/$(dir))

export DEPSDIR	:=	$(CURDIR)/$(BUILD)

#---------------------------------------------------------------------------------
# automatically build a list of object files for our project
#---------------------------------------------------------------------------------
CFILES		:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.c)))
CPPFILES	:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.cpp)))
sFILES		:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.s)))
SFILES		:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.S)))
BINFILES	:=	$(foreach dir,$(DATA),$(notdir $(wildcard $(dir)/*.*)))
SCFFILES	:=	$(foreach dir,$(TEXTURES),$(notdir $(wildcard $(dir)/*.scf)))
TPLFILES	:=	$(SCFFILES:.scf=.tpl)

#---------------------------------------------------------------------------------
# use CXX for linking C++ projects, CC for standard C
#---------------------------------------------------------------------------------
ifeq ($(strip $(CPPFILES)),)
	export LD	:=	$(CC)
else
	export LD	:=	$(CXX)
endif

export OFILES	:=	$(addsuffix .o,$(BINFILES)) \
					$(addsuffix .o,$(TPLFILES)) \
					$(CPPFILES:.cpp=.o) $(CFILES:.c=.o) \
					$(sFILES:.s=.o) $(SFILES:.S=.o)

#---------------------------------------------------------------------------------
# build a list of include paths
#---------------------------------------------------------------------------------
export INCLUDE	:=	$(foreach dir,$(INCLUDES),-I$(CURDIR)/$(dir)) \
			$(foreach dir,$(LIBDIRS),-I$(dir)/include) \
			-I$(CURDIR)/$(BUILD) \
			-I$(LIBOGC_INC)

#---------------------------------------------------------------------------------
# build a list of library paths
#---------------------------------------------------------------------------------
export LIBPATHS	:=	$(foreach dir,$(LIBDIRS),-L$(dir)/lib) \
			-L$(LIBOGC_LIB)

export OUTPUT	:=	$(CURDIR)/$(TARGET)
.PHONY: $(BUILD) clean

#---------------------------------------------------------------------------------
$(BUILD):
	@[ -d $@ ] || mkdir -p $@
	@$(MAKE) --no-print-directory -C $(BUILD) -f $(CURDIR)/Makefile

#---------------------------------------------------------------------------------
clean:
	@echo clean ...
	@rm -fr $(BUILD) $(OUTPUT).elf $(OUTPUT).dol MegaLoader MegaLoader*.rar megaloader_homebrew.iso

#---------------------------------------------------------------------------------
rar:
## to make distribution package...
## usage "make rar VERSION=x.x"
	@rm -rf MegaLoader MegaLoader*.rar iso/disc_temp/
	@mkdir MegaLoader
	@mkdir iso/disc_temp/
	@cp megaloader.dol MegaLoader
	@iso/dollz3 megaloader.dol MegaLoader/megaloader-lz.dol -m
	@iso/dollz3 megaloader.dol MegaLoader/megaloader-lz-viper.dol -m -v
	@iso/dollz3 megaloader.dol iso/disc_temp/megaloader.dol
	@cp megaloader.dol iso/disc_temp/
	@iso/mkisofs -R -J -G iso/gbi.hdr -no-emul-boot -b megaloader.dol -o MegaLoader/megaloader.iso iso/disc_temp/
	@rar a MegaLoader$(VERSION).rar MegaLoader/megaloader.dol MegaLoader/megaloader-lz.dol MegaLoader/megaloader-lz-viper.dol MegaLoader/megaloader.iso
	@rm -rf iso/disc_temp/
#---------------------------------------------------------------------------------
iso9660:
## to make homebrew iso...
## load emulators and rom files into directory iso/disc
## then run command to build iso disc called "megaloader_homebrew.iso"
## usage "make iso"
	@cp megaloader.dol iso/disc/
	@iso/mkisofs -R -J -G iso/gbi.hdr -no-emul-boot -b megaloader.dol -o megaloader_homebrew.iso iso/disc/
	@rm -rf iso/disc/megaloader.dol

#---------------------------------------------------------------------------------


run:	
	$(DEVKITPRO)/emulators/gcube/gcube $(OUTPUT).dol

#---------------------------------------------------------------------------------
else

DEPENDS	:=	$(OFILES:.o=.d)

#---------------------------------------------------------------------------------
# main targets
#---------------------------------------------------------------------------------
$(OUTPUT).dol: $(OUTPUT).elf
$(OUTPUT).elf: $(OFILES)

#---------------------------------------------------------------------------------
# This rule links in binary data with the .jpg extension
#---------------------------------------------------------------------------------
%.jpg.o	:	%.jpg
#---------------------------------------------------------------------------------
	@echo $(notdir $<)
	$(bin2o)

-include $(DEPENDS)

#---------------------------------------------------------------------------------
endif
#---------------------------------------------------------------------------------
