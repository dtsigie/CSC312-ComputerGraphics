# Original by Chris & Zach
# Modified by Doug Hyde, 1/19/2009
# Modified by Stephen Schuh, 1/19/2011

# You may need to change the paths to GL and X11 to match your system's configuration

# For Mac OS 10.6:

CXX   = g++ -m32

FLTKINCDIR = ../include
FLTKLIBDIR = ../libs/darwin/

GLDIR = /System/Library/Frameworks/OpenGL.framework/Libraries/
X11INCDIR =  /usr/X11R6/include

OBJDIR = ../objs/

LIBS = -m32 -L$(FLTKLIBDIR) -L$(GLDIR) -lfltk -lfltk_gl -lfltk_images -lfltk_forms -lfltk_jpeg -lGLU -lGL -lpng
INCS =  -I$(FLTKINCDIR) -I$(X11INCDIR) -I. -I./vecmath

BIN = csc321shell
CXXFLAGS = $(INCS) -DLINUX -s -Wall -ansi -pedantic
LDFLAGS = $(INCS) -DLINUX -s -Wall -ansi -pedantic -framework Carbon -framework AGL
 

