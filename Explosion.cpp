//
// Explosion.cpp
//

// Engine includes.
#include "EventStep.h"
#include "GameManager.h"
#include "LogManager.h"
#include "ResourceManager.h"
#include "WorldManager.h"

// Game includes.
#include "Explosion.h"

// Define registerInterest in case engine does not.
static void registerInterest(std::string s) {};

Explosion::Explosion() {
  registerInterest(df::STEP_EVENT);

  // Link to "explosion" sprite.
  if (setSprite("explosion") == 0)
    time_to_live = getAnimation().getSprite()->getFrameCount();
  else
    time_to_live = 0;

  setType("Explosion");

  setSolidness(df::SPECTRAL);
}

// Handle event.
// Return 0 if ignored, else 1.
int Explosion::eventHandler(const df::Event *p_e) {

  if (p_e->getType() == df::STEP_EVENT) {
    step();
    return 1;
  }

  // If get here, have ignored this event.
  return 0;
}

// Count down until explosion finished.
void Explosion::step() {
  time_to_live--;
  if (time_to_live <= 0)
    WM.markForDelete(this);
}
