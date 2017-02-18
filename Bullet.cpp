//
// Bullet.cpp
//

// Engine includes.
#include "LogManager.h"
#include "WorldManager.h"
#include "ResourceManager.h"
#include "EventOut.h"

// Game includes.
#include "Bullet.h"
#include "Saucer.h"

Bullet::Bullet(df::Vector hero_pos) {

  // Link to "bullet" sprite.
  df::Sprite *p_temp_sprite = RM.getSprite("bullet");
  if (!p_temp_sprite) 
    LM.writeLog("Bullet::Bullet(): Warning! Sprite '%s' not found",  "bullet");
  else {
    setSprite(p_temp_sprite);
    setSpriteSlowdown(5);		
  }

  // Set other object properties.
  setType("Bullet");
  setVelocity(df::Vector(1,0)); // Move 1 space right every game loop.

  // Set starting location, based on hero's position passed in.
  df::Vector p(hero_pos.getX()+3, hero_pos.getY());
  setPosition(p);
}

// Handle event.
// Return 0 if ignored, else 1.
int Bullet::eventHandler(const df::Event *p_e) {

  if (p_e->getType() == df::OUT_EVENT) {
    out();
    return 1;
  }

  if (p_e->getType() == df::COLLISION_EVENT) {
    const df::EventCollision *p_collision_event = dynamic_cast <const df::EventCollision *> (p_e);
    hit(p_collision_event);
    return 1;
  }

  // If get here, have ignored this event.
  return 0;
}

// If Bullet moves outside world, mark self for deletion.
void Bullet::out() {
  WM.markForDelete(this);
}

// If Bullet hits Saucer, mark Saucer and Bullet for deletion.
void Bullet::hit(const df::EventCollision *p_collision_event) {
  WM.markForDelete(p_collision_event->getObject1());
  WM.markForDelete(p_collision_event->getObject2());
}
