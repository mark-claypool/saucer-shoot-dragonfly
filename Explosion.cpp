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
  df::ResourceManager &resource_manager = df::ResourceManager::getInstance();
  df::Sprite *p_temp_sprite = resource_manager.getSprite("explosion");
  if (!p_temp_sprite) {
    df::LogManager &log_manager = df::LogManager::getInstance();
    log_manager.writeLog("Explosion::Explosion(): Warning! Sprite '%s' not found", 
		"explosion");
    return;
  }
  setSprite(p_temp_sprite);

  setType("Explosion");

  time_to_live =  getSprite()->getFrameCount();
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
  if (time_to_live <= 0){
    df::WorldManager &world_manager = df::WorldManager::getInstance();
    world_manager.markForDelete(this);
  }
}
