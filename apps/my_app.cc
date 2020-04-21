// Copyright (c) 2020 [Your Name]. All rights reserved.

#include "my_app.h"
#include <cinder/gl/draw.h>
#include <cinder/gl/gl.h>
#include <iostream>

#include <cinder/app/App.h>

using cinder::Rectf;

// Have to use a huge number since Box2D uses forces in terms of meters, but
// Cinder uses pixels, pixel:meter ratio is not 1:1
const long kForce = 100;
const int kBlockSize = 50;
const float kTimeStep = 1.0f / 60.0f;
const size_t kWallWidth = 20;

namespace myapp {

using cinder::app::KeyEvent;

MyApp::MyApp() {}

void MyApp::setup() {
  // Initialize world, blocks, and walls
  world = new b2World(b2Vec2(0, 0));
  world->SetAllowSleeping(false);
  blocks_.init(world, getWindowCenter().x - kBlockSize,
      getWindowCenter().y, getWindowCenter().x, getWindowCenter().y);
  CreateWalls();
}

void MyApp::update() {
  // In order to actually move the objects
  world->Step(kTimeStep, 0, 0);
  // Keys
  HandleKeyPressed();
}

void MyApp::draw() {
  DrawBlocks();
  DrawWalls();
}


void MyApp::DrawBlocks() const {
  // Get the current positioning
  cinder::gl::clear();
  cinder::gl::enableAlphaBlending();

  b2Vec2 block_one_pos = blocks_.GetBlockOnePos();
  b2Vec2 block_two_pos = blocks_.GetBlockTwoPos();
  cinder::gl::color(.5, 0, .5);
  cinder::gl::drawSolidRect(Rectf(block_one_pos.x,
                                  block_one_pos.y,
                                  block_one_pos.x + kBlockSize,
                                  block_one_pos.y + kBlockSize));
  cinder::gl::color(0, .5, .5);

  cinder::gl::drawSolidRect(Rectf(block_two_pos.x,
                                  block_two_pos.y,
                                  block_two_pos.x + kBlockSize,
                                  block_two_pos.y + kBlockSize));

}

void MyApp::DrawWalls() const {
    cinder::gl::color(0, .5, 0);
    cinder::gl::drawSolidRect(Rectf(wall_one_->GetPosition().x,
                                    0,
                                    kWallWidth,
                                    getWindowHeight()));
    cinder::gl::drawSolidRect(Rectf(wall_two_->GetPosition().x,0,
                                       getWindowWidth() - kWallWidth,
                                       getWindowHeight()));
}

void MyApp::CreateWalls() {
  b2BodyDef wall_body_def_one;
  wall_body_def_one.position.Set(0.0f, getWindowHeight() / 2);
  wall_one_ = world->CreateBody(&wall_body_def_one);
  b2PolygonShape wall_box;
  wall_box.SetAsBox(50.0f, 10.0f);
  wall_one_->CreateFixture(&wall_box, 0.0f);

  b2BodyDef wall_body_def_two;
  wall_body_def_two.position.Set(getWindowWidth(), getWindowHeight()/2);
  wall_two_ = world->CreateBody(&wall_body_def_two);
  wall_two_->CreateFixture(&wall_box, 0.0f);
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
void MyApp::HandleKeyPressed() {
  if (held_keys_.find(KeyEvent::KEY_LEFT) != held_keys_.end() &&
      held_keys_.find(KeyEvent::KEY_RIGHT) != held_keys_.end()) {
    if (blocks_.GetBlockOnePos().x <= kWallWidth &&
    blocks_.GetBlockTwoPos().x >= getWindowWidth() - kWallWidth - kBlockSize) {
      OneRight();
      TwoLeft();
    } else if (blocks_.GetBlockOnePos().x <= kWallWidth) {
      OneRight();
      TwoRight();
    } else if (blocks_.GetBlockTwoPos().x >= getWindowWidth()
               - kWallWidth - kBlockSize) {
      OneLeft();
      TwoRight();
    } else {
      OneLeft();
      TwoRight();
    }
  } else if (held_keys_.find(KeyEvent::KEY_LEFT) != held_keys_.end()) {
    if (blocks_.GetBlockTwoPos().x <= (kWallWidth + kBlockSize) &&
        blocks_.GetBlockOnePos().x <= kWallWidth) {
      OneRight();
      TwoRight();
    } else if (blocks_.GetBlockOnePos().x <= kWallWidth) {
      OneRight();
      TwoLeft();
    } else {
      OneLeft();
      TwoLeft();
    }
  } else if (held_keys_.find(KeyEvent::KEY_RIGHT) != held_keys_.end()) {
    if (blocks_.GetBlockOnePos().x >= (getWindowWidth() - 2*kBlockSize
        - kWallWidth) && blocks_.GetBlockTwoPos().x >= (getWindowWidth()
        - kBlockSize - kWallWidth)) {
      OneLeft();
      TwoLeft();
    } else if (blocks_.GetBlockTwoPos().x >= (getWindowWidth()
                                              - kBlockSize - kWallWidth)) {
      OneRight();
      TwoLeft();
    } else {
      OneRight();
      TwoRight();
    }
  } else if (held_keys_.find(KeyEvent::KEY_LEFT) == held_keys_.end() &&
             held_keys_.find(KeyEvent::KEY_RIGHT) == held_keys_.end()) {
    // Move first block to center
    if (blocks_.GetBlockOnePos().x < getWindowCenter().x - kBlockSize) {
      OneRight();
    } else if (blocks_.GetBlockOnePos().x > getWindowCenter().x - kBlockSize) {
      OneLeft();
    } else if (blocks_.GetBlockOnePos().x == getWindowCenter().x - kBlockSize) {
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

}  // namespace myapp
