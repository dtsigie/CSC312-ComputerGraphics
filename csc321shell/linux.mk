# Original by Chris & Zach
# Modified by Doug Hyde, 1/19/2009
# Modified by Stephen Schuh, 1/19/2011

# For Fedora or Ubuntu linux:

CXX   = g++ -m32

FLTKINCDIR = ../include
FLTKLIBDIR = ../libs/linux/

OBJDIR = ../objs/

LIBS =  -m32 -L$(FLTKLIBDIR) -lX11 -lfltk -lfltk_gl -lfltk_images -lfltk_forms -lfltk_jpeg -lGLU -lGL -lpng
INCS =  -I$(FLTKINCDIR) -I. -I./vecmath

BIN = csc321shell
CXXFLAGS = $(INCS) -DLINUX -s -Wall -ansi -pedantic
LDFLAGS = $(INCS) -DLINUX -s -Wall -ansi -pedantic


