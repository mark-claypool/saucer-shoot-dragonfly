//
// Saucer.cpp
//
 
// System includes.
#include <stdlib.h>		// for rand()

// Engine includes.
#include "EventCollision.h"
#include "EventNuke.h"
#include "EventOut.h"
#include "EventView.h"
#include "LogManager.h"
#include "ResourceManager.h"
#include "WorldManager.h"
 
// Game includes.
#include "Explosion.h"
#include "Points.h"
#include "Saucer.h"

// Define registerInterest in case engine does not.
static void registerInterest(std::string s) {};

Saucer::Saucer() {
  df::LogManager &log_manager = df::LogManager::getInstance();
  df::ResourceManager &resource_manager = df::ResourceManager::getInstance();

  // Setup "saucer" sprite.
  df::Sprite *p_temp_sprite = resource_manager.getSprite("saucer");
  if (!p_temp_sprite) {
    log_manager.writeLog("Saucer::Saucer(): Warning! Sprite '%s' not found",
			 "saucer");
  } else {
    setSprite(p_temp_sprite);
    setSpriteSlowdown(4);		
  }

  // Set object type.
  setType("Saucer");

  // Set speed in horizontal direction.
  setXVelocity(-0.25); // 1 space left every 4 frames

  // Move Saucer to start location.
  moveToStart();

  // Register interest in "nuke" event.
  registerInterest(NUKE_EVENT);
}

Saucer::~Saucer() {
  // Send "view" event with points to interested ViewObjects.
  // Add 10 points.
  df::EventView ev(POINTS_STRING, 10, true);
  df::WorldManager &world_manager = df::WorldManager::getInstance();
  world_manager.onEvent(&ev);
}

// Handle event.
// Return 0 if ignored, else 1.
int Saucer::eventHandler(const df::Event *p_e) {

  if (p_e->getType() == df::OUT_EVENT) {
    out();
    return 1;
  }

  if (p_e->getType() == df::COLLISION_EVENT) {
    const df::EventCollision *p_collision_event = dynamic_cast <df::EventCollision const *> (p_e);
    hit(p_collision_event);
    return 1;
  }

  if (p_e->getType() == NUKE_EVENT) {
 
    // Create explosion.
    Explosion *p_explosion = new Explosion;
    p_explosion -> setPosition(this -> getPosition());
 
    // Delete self.
    df::WorldManager &world_manager = df::WorldManager::getInstance();
    world_manager.markForDelete(this);
 
    // Saucers appear stay around perpetually
    new Saucer;
  }
 
  // If get here, have ignored this event.
  return 0;
}

// If moved off left edge, move back to far right.
void Saucer::out() {

  // If haven't moved off left edge, then nothing to do.
  if (getPosition().getX() >= 0)
    return;

  // Otherwise, move back to far right.
  moveToStart();

  // Spawn new Saucer to make the game get harder.
  new Saucer;

}

// Called with Saucer collides.
void Saucer::hit(const df::EventCollision *p_collision_event) {

  // If Saucer on Saucer, ignore.
  if ((p_collision_event -> getObject1() -> getType() == "Saucer") &&
      (p_collision_event -> getObject2() -> getType() == "Saucer"))
    return;

  // If Bullet, create explosion and make new Saucer.
  if ((p_collision_event -> getObject1() -> getType() == "Bullet") ||
      (p_collision_event -> getObject2() -> getType() == "Bullet")) {

    // Create an explosion.
    Explosion *p_explosion = new Explosion;
    p_explosion -> setPosition(this -> getPosition());

    // Play "explode" sound
    df::Sound *p_sound = df::ResourceManager::getInstance().getSound("explode");
    p_sound->play();

    // Saucers appear stay around perpetually.
    new Saucer;
  }

  // If Hero, mark both objects for destruction.
  if (((p_collision_event -> getObject1() -> getType()) == "Hero") || 
      ((p_collision_event -> getObject2() -> getType()) == "Hero")) {
    df::WorldManager &world_manager = df::WorldManager::getInstance();
    world_manager.markForDelete(p_collision_event -> getObject1());
    world_manager.markForDelete(p_collision_event -> getObject2());
  }

}

// Move Saucer to starting location on right side of window.
void Saucer::moveToStart() {
  df::WorldManager &world_manager = df::WorldManager::getInstance();
  df::Position temp_pos;

  // Get world boundaries.
  int world_horiz = world_manager.getBoundary().getHorizontal();
  int world_vert = world_manager.getBoundary().getVertical();

  // x is off right side of window.
  temp_pos.setX(world_horiz + rand()%world_horiz + 3);

  // y is in vertical range.
  temp_pos.setY(rand()%(world_vert-4) + 4);

  // If collision, move right slightly until empty space.
  df::ObjectList collision_list = world_manager.isCollision(this, temp_pos);
  while (!collision_list.isEmpty()) {
    temp_pos.setX(temp_pos.getX()+1);
    collision_list = world_manager.isCollision(this, temp_pos);
  }

  world_manager.moveObject(this, temp_pos);
}
