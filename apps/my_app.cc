// Copyright (c) 2020 [Your Name]. All rights reserved.

#include "my_app.h"
#include <cinder/gl/draw.h>
#include <cinder/gl/gl.h>
#include <iostream>
#include "cinder/params/Params.h"
#include <cinder/app/App.h>
#include "cinder/app/App.h"

using cinder::Rectf;
using namespace ci::app;
using namespace ci;

const double kForce = 10;
const float kBlockSize = 50;
const float kTimeStep = 1.0f / 60.0f;
const size_t kWallWidth = 20;
const float kPPM = 50.0f; // Convert from pixels to meters

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
  cinder::gl::drawSolidRect(Rectf(block_one_pos.x * kPPM,
                                  block_one_pos.y * kPPM,
                                  block_one_pos.x * kPPM + kBlockSize,
                                  block_one_pos.y * kPPM + kBlockSize));
  cinder::gl::color(0, .5, .5);

  cinder::gl::drawSolidRect(Rectf(block_two_pos.x * kPPM,
                                  block_two_pos.y * kPPM,
                                  block_two_pos.x * kPPM + kBlockSize,
                                  block_two_pos.y * kPPM + kBlockSize));
}

void MyApp::DrawWalls() const {
    cinder::gl::color(0, .5, 0);
    cinder::gl::drawSolidRect(Rectf(wall_one_->GetPosition().x * kPPM,
                                    0,
                                    kWallWidth,
                                    getWindowHeight()));
    cinder::gl::drawSolidRect(Rectf(wall_two_->GetPosition().x * kPPM,0,
                                       getWindowWidth() - kWallWidth,
                                       getWindowHeight()));
}

void MyApp::CreateWalls() {
  b2BodyDef wall_body_def_one;
  wall_body_def_one.type = b2_staticBody;
  wall_body_def_one.position.Set(0.0f, double(getWindowHeight()) / (kPPM));
  b2PolygonShape wall_box;
  wall_box.SetAsBox(0, 800 / (2*kPPM));
  wall_one_ = world->CreateBody(&wall_body_def_one);
  wall_one_->CreateFixture(&wall_box, 0.0f);

  b2PolygonShape wall_box2;
  wall_box2.SetAsBox(20 / (2*kPPM), 800 / (2*kPPM));
  b2BodyDef wall_body_def_two;
  wall_body_def_two.type = b2_staticBody;
  wall_body_def_two.position.Set(getWindowWidth() / kPPM, double(getWindowHeight())/ (kPPM));
  wall_two_ = world->CreateBody(&wall_body_def_two);
  wall_two_->CreateFixture(&wall_box2, 0.0f);
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
    OneLeft();
    TwoRight();
  }
  float block_one_x = blocks_.GetBlockOnePos().x * kPPM;
  float block_two_x = blocks_.GetBlockTwoPos().x * kPPM;
  /*if (held_keys_.find(KeyEvent::KEY_LEFT) != held_keys_.end() &&
      held_keys_.find(KeyEvent::KEY_RIGHT) != held_keys_.end()) {
    if (block_one_x <= kWallWidth &&
    block_two_x >= getWindowWidth() - kWallWidth - kBlockSize) {
      OneRight();
      TwoLeft();
    } else if (block_one_x <= kWallWidth) {
      OneRight();
      TwoRight();
    } else if (block_two_x >= getWindowWidth()
               - kWallWidth - kBlockSize) {
      OneLeft();
      TwoRight();
    } else {
      OneLeft();
      TwoRight();
    }
  } else if (held_keys_.find(KeyEvent::KEY_LEFT) != held_keys_.end()) {
    if (block_two_x <= (kWallWidth + kBlockSize) &&
        block_one_x <= kWallWidth) {
      OneRight();
      TwoRight();
    } else if (block_one_x <= kWallWidth) {
      OneRight();
      TwoLeft();
    } else {
      OneLeft();
      TwoLeft();
    }
  } else if (held_keys_.find(KeyEvent::KEY_RIGHT) != held_keys_.end()) {
    if (block_one_x >= (getWindowWidth() - 2*kBlockSize
        - kWallWidth) && block_two_x >= (getWindowWidth()
        - kBlockSize - kWallWidth)) {
      OneLeft();
      TwoLeft();
    } else if (block_two_x >= (getWindowWidth() - kBlockSize - kWallWidth)) {
      OneRight();
      TwoLeft();
    } else {
      OneRight();
      TwoRight();
    }
  } else if (held_keys_.find(KeyEvent::KEY_LEFT) == held_keys_.end() &&
             held_keys_.find(KeyEvent::KEY_RIGHT) == held_keys_.end()) {
    // Move first block to center
    if (block_one_x < getWindowCenter().x - kBlockSize) {
      OneRight();
    } else if (block_one_x > getWindowCenter().x - kBlockSize) {
      OneLeft();
    } else if (block_one_x == getWindowCenter().x - kBlockSize) {
      OneStill();
    }
    // Move second block to center
    if (block_two_x >= getWindowCenter().x) {
      TwoLeft();
    } else if (block_two_x <= getWindowCenter().x) {
      TwoRight();
    } else if (block_two_x == getWindowCenter().x) {
      TwoStill();
    }
  } */
}

}  // namespace myapp
