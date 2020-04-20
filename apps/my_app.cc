// Copyright (c) 2020 [Your Name]. All rights reserved.

#include "my_app.h"
#include <cinder/gl/draw.h>
#include <cinder/gl/gl.h>
#include <iostream>

#include <cinder/app/App.h>

using cinder::Rectf;

// Have to use a huge number since Box2D uses forces in terms of meters, but
// Cinder uses pixels, pixel:meter ratio is not 1:1
const long kForce = 10000000000;

namespace myapp {

using cinder::app::KeyEvent;

MyApp::MyApp() {}

void MyApp::setup() {
  // Initialize world and blocks
  world = new b2World(b2Vec2(0, 0));
  world->SetAllowSleeping(false);
  blocks_.init(world, getWindowCenter().x - 50, getWindowCenter().y,
      getWindowCenter().x, getWindowCenter().y);
}

void MyApp::update() {
  // What is step??
  world->Step(1.0f / 60.0f, 0, 0);
  // If both keys are held down
  if (held_keys_.find(KeyEvent::KEY_LEFT) != held_keys_.end() &&
      held_keys_.find(KeyEvent::KEY_RIGHT) != held_keys_.end()) {
    if (blocks_.GetBlockOnePos().x <= 0 && blocks_.GetBlockTwoPos().x >= 750) {
      OneStill();
      TwoStill();
    } else if (blocks_.GetBlockOnePos().x <= 0) {
      OneStill();
      TwoRight();
    } else if (blocks_.GetBlockTwoPos().x >= 750) {
      OneLeft();
      TwoStill();
    } else {
      OneLeft();
      TwoRight();
    }
  } else if (held_keys_.find(KeyEvent::KEY_LEFT) != held_keys_.end()) {
    if (blocks_.GetBlockOnePos().x <= 0 &&
        blocks_.GetBlockTwoPos().x <= 50) {
      OneStill();
      TwoStill();
    } else if (blocks_.GetBlockOnePos().x <= 0) {
      OneStill();
      TwoLeft();
    } else {
      OneLeft();
      TwoLeft();
    }
  } else if (held_keys_.find(KeyEvent::KEY_RIGHT) != held_keys_.end()) {
    if (blocks_.GetBlockOnePos().x >= 700 && blocks_.GetBlockTwoPos().x >= 750) {
      OneStill();
      TwoStill();
    } else if (blocks_.GetBlockTwoPos().x >= 750) {
      OneRight();
      TwoStill();
    } else {
      OneRight();
      TwoRight();
    }
  } else if (held_keys_.find(KeyEvent::KEY_LEFT) == held_keys_.end() &&
             held_keys_.find(KeyEvent::KEY_RIGHT) == held_keys_.end()) {
    // Move first block to center
    if (blocks_.GetBlockOnePos().x < getWindowCenter().x - 50) {
      OneRight();
    } else if (blocks_.GetBlockOnePos().x > getWindowCenter().x - 50) {
      OneLeft();
    } else if (blocks_.GetBlockOnePos().x == getWindowCenter().x - 50) {
      OneStill();
    }
    // Move second block to center
    if (blocks_.GetBlockTwoPos().x >= getWindowCenter().x) {
      TwoLeft();
    } else if (blocks_.GetBlockTwoPos().x <= getWindowCenter().x) {
      TwoRight();
    } else if (blocks_.GetBlockTwoPos().x == getWindowCenter().x) {
      TwoStill();
    }
  }
}

void MyApp::draw() {
  DrawBlocks();
}


void MyApp::DrawBlocks() const {
  // Get the current positioning
  cinder::gl::clear();
  b2Vec2 block_one_pos = blocks_.GetBlockOnePos();
  b2Vec2 block_two_pos = blocks_.GetBlockTwoPos();
  cinder::gl::color(.5, 0, .5);
  cinder::gl::drawSolidRect(Rectf(block_one_pos.x,
                                  block_one_pos.y,
                                  block_one_pos.x + 50,
                                  block_one_pos.y + 50));
  cinder::gl::color(0, .5, .5);

  cinder::gl::drawSolidRect(Rectf(block_two_pos.x,
                                  block_two_pos.y,
                                  block_two_pos.x + 50,
                                  block_two_pos.y + 50));

}

void MyApp::keyDown(KeyEvent event) {
  held_keys_.insert(event.getCode());
}

void MyApp::keyUp(KeyEvent event) {
  held_keys_.erase(event.getCode());
}

void MyApp::DrawSpikes() const {}
void MyApp::ResetGame() const {}
void MyApp::DrawGameOver() const {}


void MyApp::OneLeft() {
  blocks_.ApplyForceToOne(b2Vec2(-kForce, 0));
}
void MyApp::OneRight() {
  blocks_.ApplyForceToOne(b2Vec2(kForce, 0));
}
void MyApp::OneStill() {
  blocks_.ApplyForceToOne(b2Vec2(0, 0));
}
void MyApp::TwoLeft() {
  blocks_.ApplyForceToTwo(b2Vec2(-kForce, 0));
}
void MyApp::TwoRight() {
  blocks_.ApplyForceToTwo(b2Vec2(kForce, 0));
}
void MyApp::TwoStill() {
  blocks_.ApplyForceToTwo(b2Vec2(0, 0));
}

}  // namespace myapp
