# vim:ts=4

first: all

# Override these variables in Makefile.local.

# Show verbose compilation commands?
VERBOSE			:= 0
# How many threads use for compilation?
THREADS			:= $(shell grep processor /proc/cpuinfo|wc -l)
# Architecture:
ARCH			:= $(shell uname -m)
# Enable profiling?
PROFILING		:= 0
# Debugging enabled?
DEBUG			:= 0
# Where Qt is installed?
QT_PREFIX		:= /usr
# Profile name:
PROFILE			:=
# Version:
VERSION			:= alpha0.1dev
# Release version? (no asserts, etc).
RELEASE			:= 0
# Set to 1 to disable some UB-generating optimizations:
UB_OPTS_DISABLE	:= 0

-include Makefile.local

#### Core (special) vars ####

DEFINES			+= -DMCU_TYPE=$(MCU) -DF_CPU=$(MCU_FREQUENCY)
C_CXX_FLAGS		+= -fPIE -O3 -g -c -std=c++14 -Wall -Wall -Wextra -Wunused -Wunused-function -Wunused-label -fstrict-aliasing $(foreach warning, $(CLANG_DISABLED_WARNINGS), -Wno-$(warning))
C_CXX_FLAGS		+= -pthread -I. -Ilib/libusbcc/src
C_CXX_FLAGS		+= -O3 -Ilib -Wall
C_CXX_OPT_FLAGS	+= -finline -funroll-loops -fomit-frame-pointer
LIBS			+= m boost_system pthread mhash
CXXFLAGS		+= -DCXXFLAGS='"$(CXXFLAGS_s)"' $(foreach feature, $(XEFIS_FEATURES), -D$(feature))
ifeq ($(DEBUG),1)
XEFIS_FEATURES	+= XEFIS_ENABLE_ASSERT
XEFIS_FEATURES	+= XEFIS_ENABLE_FATAL_ASSERT
CXXFLAGS		+= -O2 -ggdb3 -fno-inline -fno-eliminate-unused-debug-types -D_GLIBCXX_DEBUG
else
CXXFLAGS		+= -DQT_NO_DEBUG
endif
PKGCONFIGS		+= Qt5Core Qt5Gui Qt5Network Qt5Xml Qt5Widgets libusb-1.0

AS				:= as
ASFLAGS			+= $(DEFINES) -O3 -Ilib -Wall
C				:= gcc -c
CFLAGS			+= $(DEFINES) $(C_CXX_FLAGS) $(C_CXX_OPT_FLAGS)
CXX				:= g++ -c
CXXFLAGS		+= $(DEFINES) $(C_CXX_FLAGS) -std=c++14 $(C_CXX_OPT_FLAGS)
SO				:= $(CXX) -shared
SOFLAGS			+=
LD				:= g++
LDFLAGS			+= -rdynamic
AR				:= ar
ARFLAGS			+=
DEPGEN			:= $(CXX)
DEPFLAGS		+= $(CXXFLAGS) $(DEFINES)
MOC				:= $(QT_PREFIX)/bin/moc-qt5

#### Generic additional flags ####

ifeq ($(CXX),g++)
C_CXX_FLAGS		+= -fPIE -O0 -g -Wall -Wall -Wextra -Wunused -Wunused-function -Wunused-label -fstrict-aliasing -Wstrict-aliasing=3
C_CXX_FLAGS		+= -pthread -rdynamic -I. -Ilib/libusbcc/src
CXXFLAGS		+= -std=c++14 -fnothrow-opt -Wnoexcept
endif

ifeq ($(CXX),clang++)
CLANG_DISABLED_WARNINGS = mismatched-tags disabled-macro-expansion c++98-compat-pedantic documentation-unknown-command
C_CXX_FLAGS		+= -fPIE -O3 -g -std=c++14 -Wall -Wall -Wextra -Wunused -Wunused-function -Wunused-label -fstrict-aliasing $(foreach warning, $(CLANG_DISABLED_WARNINGS), -Wno-$(warning))
C_CXX_FLAGS		+= -pthread -I. -Ilib/libusbcc/src
endif

# Undefined-Behaviour handling:
ifeq ($(UB_OPTS_DISABLE),1)
C_CXX_FLAGS		+= -fno-strict-overflow -fno-delete-null-pointer-checks -fno-strict-aliasing
endif

ifeq ($(DEBUG),1)
C_CXX_FLAGS		+= -O2 -ggdb3 -fno-inline -fno-eliminate-unused-debug-types -D_GLIBCXX_DEBUG
else
C_CXX_FLAGS		+= -DQT_NO_DEBUG
endif

ifneq ($(LIBS),)
LDFLAGS			+= $(foreach lib, $(LIBS), -l$(lib))
endif

ifneq ($(PKGCONFIGS),)
LDFLAGS			+= $(shell pkg-config --libs $(PKGCONFIGS))
CFLAGS			+= $(shell pkg-config --cflags $(PKGCONFIGS))
CXXFLAGS		+= $(shell pkg-config --cflags $(PKGCONFIGS))
endif

ifneq ($(VERBOSE),1)
MAKEFLAGS += --silent
endif

ifneq ($(THREADS),)
MAKEFLAGS += -j$(THREADS)
endif

ifeq ($(PROFILING),1)
CXXFLAGS += -pg -fno-omit-frame-pointer
LDFLAGS += -pg
endif

ifeq ($(RELEASE),1)
CXXFLAGS += -DBOOST_DISABLE_ASSERTS -DNDEBUG
else
CXXFLAGS += -DXEFIS_ENABLE_ASSERT
endif

#### Sources and rules ####

HEADERS =
SOURCES =
OBJECTS =
NODEP_OBJECTS =
MOCSRCS =
TARGETS =

include Makefile.sources
include Makefile.core
include Makefile.rules

