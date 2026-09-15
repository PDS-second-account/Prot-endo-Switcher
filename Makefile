# Pret-arium Switcher
# Based on Roséverse Installer (MIT) by Project Rosé & contributors

ifeq ($(strip $(DEVKITPRO)),)
$(error "Please set DEVKITPRO in your environment")
endif
ifeq ($(strip $(DEVKITPPC)),)
$(error "Please set DEVKITPPC in your environment")
endif

WUT_ROOT := $(DEVKITPRO)/wut
PORTLIBS_PATH := $(DEVKITPRO)/portlibs
ifneq ($(strip $(V)), 1)
Q ?= @
endif

include $(WUT_ROOT)/share/wut_rules

TARGETNAME  = pretarium_switcher
TARGET      = inkay
BUILDDIR    = build
EG2DIR      = eg2
TARGETDIR   = $(BUILDDIR)/$(TARGET)
FSDIR       = fs
OBJDIR      = $(TARGETDIR)/obj
INCLUDE     = -I$(WUT_ROOT)/include -I$(PORTLIBS_PATH)/wiiu/include -I.
DEFINES     = -D__WUT__
LIBINC      = -L$(WUT_ROOT)/lib -L$(PORTLIBS_PATH)/wiiu/lib -L$(PORTLIBS_PATH)/ppc/lib
SOURCES     = $(subst ./,,$(shell find $(EG2DIR) $(TARGET) -name "*.cc"))

CC          = $(DEVKITPPC)/bin/powerpc-eabi-gcc
CXX         = $(DEVKITPPC)/bin/powerpc-eabi-g++
STRIP       = $(DEVKITPPC)/bin/powerpc-eabi-strip
OBJDUMP     = $(DEVKITPPC)/bin/powerpc-eabi-objdump
ELF2RPL     = $(DEVKITPRO)/tools/bin/elf2rpl
WUHBTOOL    = $(DEVKITPRO)/tools/bin/wuhbtool

CODEDIR     = $(FSDIR)/code
CONTENTDIR  = $(FSDIR)/content
METADIR     = $(FSDIR)/meta

CXXBASE     = -Os -Wall -Wextra $(INCLUDE) $(MACHDEP) $(DEFINES) -MMD -MP
CXXOPT      = -ffunction-sections -fdata-sections -fmerge-all-constants -fomit-frame-pointer -fvisibility=hidden
CXXOPT1     = -finline-small-functions -fno-rtti -fno-threadsafe-statics -fmerge-constants -fno-math-errno
CXXOPT2     = -fno-use-cxa-atexit -fno-thread-jumps -fno-rtti -flto -fno-common
CXXOPT3     = -fno-ident -fno-unwind-tables -fno-asynchronous-unwind-tables -fno-stack-protector -fno-builtin
CXXFLAGS    = $(CXXBASE) $(CXXOPT) $(CXXOPT1) $(CXXOPT2) $(CXXOPT3)
LIBS        = $(LIBINC) -lwut -lcurl -lmbedtls -lmbedcrypto -lmbedx509 -lz -lbrotlicommon -lbrotlidec
LDFLAGS     = -flto=auto -Wno-odr -Wl,--gc-sections -Wl,--build-id=none $(ARCH) $(RPXSPECS) $(LIBS)

WUHBFLAGS = \
    --name="Pret-arium Switcher" \
    --short-name="Pret-arium Switcher" \
    --author="Pret-arium Project / based on Project Rose" \
    --icon="$(METADIR)/iconTex.png" \
    --tv-image="$(METADIR)/tv-splash.png" \
    --drc-image="$(METADIR)/drc-splash.png" \
    --content="$(CONTENTDIR)"

OBJFILES = $(patsubst %,$(OBJDIR)/%,$(SOURCES:.cc=.o))
.DEFAULT_GOAL = all
.PHONY: all clean

all: $(OBJDIR) $(TARGETDIR)/$(TARGETNAME).elf $(CODEDIR)/$(TARGETNAME).rpx $(BUILDDIR)/$(TARGETNAME).wuhb

$(OBJDIR):
	@mkdir -p $@

$(OBJDIR)/%.o: %.cc
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

-include $(OBJFILES:.o=.d)

$(TARGETDIR)/$(TARGETNAME).elf: $(OBJFILES)
	$(CXX) $(OBJFILES) $(LDFLAGS) -o $@
	$(STRIP) --strip-unneeded $@

$(CODEDIR)/$(TARGETNAME).rpx: $(TARGETDIR)/$(TARGETNAME).elf
	@mkdir -p $(CODEDIR)
	$(ELF2RPL) $< $@

$(BUILDDIR)/$(TARGETNAME).wuhb: $(CODEDIR)/$(TARGETNAME).rpx
	$(WUHBTOOL) $< $@ $(WUHBFLAGS) > /dev/null

clean:
	rm -rf $(BUILDDIR)
	find . -name "*.rpx" -type f -delete
