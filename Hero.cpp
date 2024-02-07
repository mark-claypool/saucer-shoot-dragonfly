//
// Hero.cpp
//

// Engine includes.
#include "EventMouse.h"
#include "EventStep.h"
#include "EventView.h"
#include "GameManager.h"
#include "LogManager.h"
#include "ResourceManager.h"
#include "WorldManager.h"

// Game includes.
#include "Bullet.h"
#include "EventNuke.h"
#include "Explosion.h"
#include "GameOver.h"
#include "Hero.h"

Hero::Hero() {

  // Link to "ship" sprite.
  setSprite("ship");

  // Player controls hero, so register for input events.
#ifdef DF_REGISTER_INTEREST
  registerInterest(df::KEYBOARD_EVENT);
  registerInterest(df::MSE_EVENT);

  // Need to update rate control each step.
  registerInterest(df::STEP_EVENT);
#endif

  // Set object type.
  setType("Hero");

  // Set starting location.
  df::Vector p(7, WM.getBoundary().getVertical()/2);
  setPosition(p);

  // Create reticle for firing bullets.
  p_reticle = new Reticle();
  p_reticle->draw();

  // Set attributes that control actions.
  move_slowdown = 2;
  move_countdown = move_slowdown;
  fire_slowdown = 30;
  fire_countdown = fire_slowdown;
  nuke_count = 1;
}
  
Hero::~Hero() {

  // Create GameOver object.
  new GameOver;
  
  // Make big explosion.
  for (int i=-8; i<=8; i+=5) {
    for (int j=-5; j<=5; j+=3) {
      df::Vector temp_pos = this->getPosition();
      temp_pos.setX(this->getPosition().getX() + i);
      temp_pos.setY(this->getPosition().getY() + j);
      Explosion *p_explosion = new Explosion;
      p_explosion -> setPosition(temp_pos);
    }
  }
 
  // Mark Reticle for deletion.
  WM.markForDelete(p_reticle);
}
 
// Handle event.
// Return 0 if ignored, else 1.
int Hero::eventHandler(const df::Event *p_e) {

  if (p_e->getType() == df::KEYBOARD_EVENT) {
    const df::EventKeyboard *p_keyboard_event = dynamic_cast <const df::EventKeyboard *> (p_e);
    kbd(p_keyboard_event);
    return 1;
  }

  if (p_e->getType() == df::MSE_EVENT) {
    const df::EventMouse *p_mouse_event = dynamic_cast <const df::EventMouse *> (p_e);
    mouse(p_mouse_event);
    return 1;
  }

  if (p_e->getType() == df::STEP_EVENT) {
    step();
    return 1;
  }
 
  // If get here, have ignored this event.
  return 0;
}

// Take appropriate action according to mouse action.
void Hero::mouse(const df::EventMouse *p_mouse_event) {

  // Pressed button?
  if ((p_mouse_event->getMouseAction() == df::CLICKED) &&
      (p_mouse_event->getMouseButton() == df::Mouse::LEFT))
    fire(p_mouse_event->getMousePosition());
}

// Take appropriate action according to key pressed.
void Hero::kbd(const df::EventKeyboard *p_keyboard_event) {

  switch(p_keyboard_event->getKey()) {
  case df::Keyboard::W:       // up
    if (p_keyboard_event->getKeyboardAction() == df::KEY_PRESSED)
      move(-1);
    break;
  case df::Keyboard::S:       // down
    if (p_keyboard_event->getKeyboardAction() == df::KEY_PRESSED)
      move(+1);
    break;
  case df::Keyboard::SPACE:   // nuke!
    if (p_keyboard_event->getKeyboardAction() == df::KEY_PRESSED)
      nuke();
    break;
  case df::Keyboard::Q:        // quit
    if (p_keyboard_event->getKeyboardAction() == df::KEY_PRESSED)
      WM.markForDelete(this);
    break;
  default:
    break;
  };

  return;
}

// Move up or down.
void Hero::move(int dy) {

  // See if time to move.
  if (move_countdown > 0)
    return;
  move_countdown = move_slowdown;

  // If stays on window, allow move.
  df::Vector new_pos(getPosition().getX(), getPosition().getY() + dy);
  if ((new_pos.getY() > 3) && 
      (new_pos.getY() < WM.getBoundary().getVertical()-1))
    WM.moveObject(this, new_pos);
}

// Fire bullet towards target.
void Hero::fire(df::Vector target) {

  // See if time to fire.
  if (fire_countdown > 0)
    return;
  fire_countdown = fire_slowdown;

  // Fire Bullet towards target.
  Bullet *p = new Bullet(getPosition());
  p->setVelocity(df::Vector(p->getVelocity().getX(),
			    (target.getY() - getPosition().getY()) /
			    (target.getX() - getPosition().getX())));

  // Play "fire" sound.
  df::Sound *p_sound = RM.getSound("fire");
  p_sound->play();
}

// Decrease rate restriction counters.
void Hero::step() {

  // Move countdown.
  move_countdown--;
  if (move_countdown < 0)
    move_countdown = 0;

  // Fire countdown.
  fire_countdown--;
  if (fire_countdown < 0)
    fire_countdown = 0;
}

// Send "nuke" event to all objects.
void Hero::nuke() {

  // Check if nukes left.
  if (!nuke_count) 
    return;
  nuke_count--;

  // Create "nuke" event and send to interested Objects.
  EventNuke nuke;
  WM.onEvent(&nuke);
 
  // Send "view" event do decrease number of nukes to interested ViewObjects.
  df::EventView ev("Nukes", -1, true);
  WM.onEvent(&ev);

  // Play "nuke" sound.
  df::Sound *p_sound = RM.getSound("nuke");
  p_sound->play();
}
