#
# Makefile for saucer shoot game using Dragonfly
#
# Copyright Mark Claypool and WPI, 2015
#
# 'make depend' to generate new dependency list
# 'make clean' to remove all constructed files
# 'make' to build executable
#
# Variables of interest:
#   GAMESRC is the source code files for the game
#   GAME is the game main() source
#

CC= g++ 
# Uncomment below for Mac
#LINK= -I/usr/local/Cellar/sfml/2.3_1/include -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio 
# Uncomment below for Linux
#LINK= -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio -lrt
INCLUDE= -I../dragonfly/ # path to dragonfly includes
ENGINE= ../dragonfly/libdragonfly.a # location of dragonfly engine
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

all: $(EXECUTABLE) Makefile

$(EXECUTABLE): $(ENGINE) $(OBJECTS) $(GAME) $(GAMESRC) 
	$(CC) $(GAME) $(OBJECTS) $(ENGINE) -o $@ $(INCLUDE) $(LINK) 

.cpp.o: 
	$(CC) -c $(INCLUDE) $< -o $@

clean:
	rm -rf $(OBJECTS) $(EXECUTABLE) core dragonfly.log Makefile.bak *~

depend: 
	makedepend *.cpp 2> /dev/null

# DO NOT DELETE
