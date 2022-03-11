#
# Makefile for saucer shoot game using Dragonfly
#
# Mark Claypool. Dragonfly - Program a Game Engine from Scratch,
# Interactive Media and Game Development, Worcester Polytechnic
# Institute, 2014. Online at: http://dragonfly.wpi.edu/book/
#
# Note!  This uses the core engine (no optional elements implemented)
# as specified in the book, version 6.
#
# Copyright Mark Claypool and WPI, 2016-2019
#
# 'make' to build executable
# 'make depend' to generate new dependency list
# 'make clean' to remove all constructed files
#
# Variables of interest:
#   GAMESRC is the source code files for the game
#   GAME is the game main() source
#   EXECUTABLE is the name of the runnable game
#

# Compiler.
CC= g++

# Libraries and includes.
LINKDIR= -L../dragonfly/lib # path to dragonfly library
INCDIR= -I../dragonfly/include # path to dragonfly includes
#LINKDIR= -L ../../dragonfly # path to dragonfly library
#INCDIR= -I ../../dragonfly # path to dragonfly includes

## Uncomment and update below if using local SFML installation.
LOCALSFML= $(HOME)/src/SFML
LINKDIR:= $(LINKDIR) -L $(LOCALSFML)/lib
INCDIR:= $(INCDIR) -I $(LOCALSFML)/include

### Uncomment only 1 of the below! ###

# 1) Uncomment below for Linux (64-bit).
CFLAGS=
LINKLIB= -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio -lrt
LINKLIB= -ldragonfly-linux64 $(LINKLIB) 
#LINKLIB:= -ldragonfly $(LINKLIB) 

# 2) Uncomment below for Mac (64-bit).
# Note: if homebrew install sfml, may be in:
#   /usr/local/Cellar/sfml
#CFLAGS=
#LINKLIB= -ldragonfly-mac64 -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio 

######

GAMESRC= \
         Bullet.cpp \
         EventNuke.cpp \
         Explosion.cpp \
         GameOver.cpp \
         GameStart.cpp \
         Hero.cpp \
         Points.cpp \
         Reticle.cpp \
         Saucer.cpp \
         Star.cpp \

GAME= game.cpp
EXECUTABLE= game
OBJECTS= $(GAMESRC:.cpp=.o)
# Uncomment if using STL
#CFLAGS:= $(CFLAGS) -DSTL

all: $(EXECUTABLE) Makefile

$(EXECUTABLE): $(OBJECTS) $(GAME) $(GAMESRC) 
	$(CC) $(CFLAGS) $(GAME) $(OBJECTS) -o $@ $(INCDIR) $(LINKDIR) $(LINKLIB) 

.cpp.o: 
	$(CC) $(CFLAGS) -c $(INCDIR) $< -o $@

clean:
	rm -rf $(OBJECTS) $(EXECUTABLE) core dragonfly.log Makefile.bak *~

