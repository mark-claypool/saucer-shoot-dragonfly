//
// Points.cpp
//

// Engine includes
#include "DisplayManager.h"
#include "Event.h"
#include "EventStep.h"
#include "LogManager.h"

// Game includes.
#include "Points.h"

Points::Points() {
  setLocation(df::TOP_RIGHT);
  setViewString(POINTS_STRING);
  setColor(df::YELLOW);
  // Need to update score each second, so count "step" events.
#ifdef DF_REGISTER_INTEREST
  registerInterest(df::STEP_EVENT);
#endif
}

// Handle event.
// Return 0 if ignored, else 1.
int Points::eventHandler(const df::Event *p_e) {

  // Parent handles event if score update.
  if (df::ViewObject::eventHandler(p_e)) {
    return 1;
  }

  // If step, increment score every second (30 steps).
  if (p_e->getType() == df::STEP_EVENT) {
    if (dynamic_cast <const df::EventStep *> (p_e) -> getStepCount() % 30 == 0)
      setValue(getValue() + 1);
    return 1;
  }

  // If get here, have ignored this event.
  return 0;
}
