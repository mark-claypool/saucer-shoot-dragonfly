//
// Star.cpp
//

// System includes.
#include <stdlib.h>		// for rand()

// Engine includes.
#include "DisplayManager.h"
#include "EventOut.h"
#include "WorldManager.h"

// Game includes.
#include "Star.h"

Star::Star() {
  setType("Star");
  setSolidness(df::SPECTRAL);
  setVelocity(df::Vector((float) -1.0 /(rand()%9 + 2), 0));
  setAltitude(0);	// Make Stars in the background.
  df::Vector p((float) (rand()%(int)WM.getBoundary().getHorizontal()),
	       (float) (rand()%(int)WM.getBoundary().getVertical()));
  setPosition(p);
}

// Draw star window.
void Star::draw() {
  DM.drawCh(getPosition(), STAR_CHAR, df::WHITE); 
}

// Handle event.
// Return 0 if ignored, else 1.
int Star::eventHandler(const df::Event *p_e) {

  if (p_e->getType() == df::OUT_EVENT) {
    out();
    return 1;
  }

  // If get here, have ignored this event.
  return 0;
}

// If Star moved off window, move back to far right.
void Star::out() {
  df::Vector p((float) (WM.getBoundary().getHorizontal() + rand()%20),
	       (float) (rand() % (int)WM.getBoundary().getVertical()));
  setPosition(p);
}
