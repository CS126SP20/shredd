// Copyright (c) 2020 [Alice Huang]. All rights reserved.

#include "my_app.h"
#include "cinder/ImageIo.h"
#include <cinder/gl/draw.h>
#include <cinder/gl/gl.h>
#include <iostream>
#include "cinder/params/Params.h"
#include <cinder/app/App.h>
#include "cinder/gl/Texture.h"

using cinder::Rectf;
using namespace ci::app;
using namespace ci;

const double kForce = 5;
const float kBlockSize = 50.0f;
const float kTimeStep = 1.0f / 60.0f;
const float kWallWidth = 20.0f;
const float kPPM = 50.0f; // Convert from pixels to meters

namespace myapp {

using cinder::app::KeyEvent;

MyApp::MyApp(): is_home_screen{true} {}

void MyApp::setup() {
  // Initialize world, blocks, and walls
  world = new b2World(b2Vec2(0, 0));
  world->SetAllowSleeping(false);
  blocks_.init(world, getWindowCenter().x - kBlockSize,
      getWindowCenter().y, getWindowCenter().x, getWindowCenter().y);
  CreateWalls();
  image = ci::gl::Texture::create(loadImage(loadAsset("background_shredd.jpg")));
}

void MyApp::update() {
  // In order to actually move the objects
  world->Step(kTimeStep, 5, 2);
  world->ClearForces();
  // Take care of key movements
  HandleKeyPressed();
}

void MyApp::draw() {
  cinder::gl::enableAlphaBlending();

  gl::clear();
  if (is_home_screen) {
    //DrawHomeScreen();
    DrawBackground();
    DrawBlocks();
    DrawWalls();
  } else {
    DrawBackground();
    DrawBlocks();
    DrawWalls();
  }
}

void MyApp::DrawBackground() {
  gl::clear(Color(.5, 1, 1));
}

void MyApp::DrawHomeScreen() {
  gl::draw(image, getWindowBounds());
  auto box = TextBox()
      .alignment(TextBox::CENTER)
      .font(cinder::Font("Impact", 80))
      .size({600, 100})
      .color(Color(0, .5, .5))
      .backgroundColor(ColorA(0, 0, 0, 0))
      .text("SHREDD");

  const auto box_size = box.getSize();
  const cinder::vec2 locp = {0, 0};
  const auto surface = box.render();
  const auto texture = cinder::gl::Texture::create(surface);
  cinder::gl::draw(texture, locp);
}


void MyApp::DrawBlocks() const {
  // Get the current positioning
  b2Vec2 block_one_pos = blocks_.GetBlockOnePos();
  b2Vec2 block_two_pos = blocks_.GetBlockTwoPos();
  cinder::gl::color(.5, 1, .5);
  cinder::gl::drawSolidRect(Rectf(block_one_pos.x * kPPM,
                                  block_one_pos.y * kPPM,
                                  block_one_pos.x * kPPM + kBlockSize,
                                  block_one_pos.y * kPPM + kBlockSize));
  cinder::gl::color(1, .5, .5);

  cinder::gl::drawSolidRect(Rectf(block_two_pos.x * kPPM,
                                  block_two_pos.y * kPPM,
                                  block_two_pos.x * kPPM + kBlockSize,
                                  block_two_pos.y * kPPM + kBlockSize));
}

void MyApp::DrawWalls() const {
    cinder::gl::color(1, 1, 1);
    cinder::gl::drawSolidRect(Rectf(wall_one_->GetPosition().x * kPPM,
                                    0,
                                    10,
                                    getWindowHeight()));
    cinder::gl::drawSolidRect(Rectf(wall_two_->GetPosition().x * kPPM,0,
                                       getWindowWidth() - 10,
                                       getWindowHeight()));
}

void MyApp::CreateWalls() {
  b2BodyDef wall_body_def_one;
  wall_body_def_one.type = b2_staticBody;
  wall_body_def_one.position.Set(0.0f, double(getWindowHeight()) / (kPPM));
  b2PolygonShape wall_box;
  wall_box.SetAsBox(20, 800 / (2*kPPM));
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
void MyApp::DrawGameOver() const {}


void MyApp::OneLeft() {
  blocks_.ApplyForceToOne(b2Vec2(-kForce, 0));
}
void MyApp::OneRight() {
  blocks_.ApplyForceToOne(b2Vec2(kForce, 0));
}

void MyApp::TwoLeft() {
  blocks_.ApplyForceToTwo(b2Vec2(-kForce, 0));
}
void MyApp::TwoRight() {
  blocks_.ApplyForceToTwo(b2Vec2(kForce, 0));
}

void MyApp::HandleKeyPressed() {
  float block_one_x = blocks_.GetBlockOnePos().x * kPPM;
  float block_two_x = blocks_.GetBlockTwoPos().x * kPPM;
  if (held_keys_.find(KeyEvent::KEY_LEFT) != held_keys_.end() &&
      held_keys_.find(KeyEvent::KEY_RIGHT) != held_keys_.end()) {
    if (block_one_x <= kWallWidth &&
    block_two_x >= getWindowWidth() - kWallWidth - kBlockSize) {
      blocks_.GetBlockOne()->SetLinearVelocity(b2Vec2(0, 0));
      blocks_.GetBlockTwo()->SetLinearVelocity(b2Vec2(0, 0));
    } else if (block_one_x <= kWallWidth) {
      blocks_.GetBlockOne()->SetLinearVelocity(b2Vec2(0, 0));
      TwoRight();
    } else if (block_two_x >= getWindowWidth()
               - kWallWidth - kBlockSize) {
      OneLeft();
      blocks_.GetBlockTwo()->SetLinearVelocity(b2Vec2(0, 0));
    } else {
      OneLeft();
      TwoRight();
    }
  } else if (held_keys_.find(KeyEvent::KEY_LEFT) != held_keys_.end()) {
    if (block_two_x <= (kWallWidth + kBlockSize) &&
        block_one_x <= kWallWidth) {
      blocks_.GetBlockOne()->SetLinearVelocity(b2Vec2(0, 0));
      blocks_.GetBlockTwo()->SetLinearVelocity(b2Vec2(0, 0));
    } else if (block_one_x <= kWallWidth) {
      blocks_.GetBlockOne()->SetLinearVelocity(b2Vec2(0, 0));
      TwoLeft();
    } else {
      OneLeft();
      TwoLeft();
    }
  } else if (held_keys_.find(KeyEvent::KEY_RIGHT) != held_keys_.end()) {
    if (block_one_x >= (getWindowWidth() - 2*kBlockSize
        - kWallWidth) && block_two_x >= (getWindowWidth()
        - kBlockSize - kWallWidth)) {
      blocks_.GetBlockOne()->SetLinearVelocity(b2Vec2(0, 0));
      blocks_.GetBlockTwo()->SetLinearVelocity(b2Vec2(0, 0));
    } else if (block_two_x >= (getWindowWidth() - kBlockSize - kWallWidth)) {
      OneRight();
      blocks_.GetBlockTwo()->SetLinearVelocity(b2Vec2(0, 0));
    } else {
      OneRight();
      TwoRight();
    }
  } else if (held_keys_.find(KeyEvent::KEY_LEFT) == held_keys_.end() &&
             held_keys_.find(KeyEvent::KEY_RIGHT) == held_keys_.end()) {
    // TODO: why are the two blocks overlapping???????
    if (block_one_x + kBlockSize == block_two_x) { // Case where blocks are together
      if (block_two_x == getWindowCenter().x) {
        blocks_.GetBlockOne()->SetLinearVelocity(b2Vec2(0, 0));
        blocks_.GetBlockTwo()->SetLinearVelocity(b2Vec2(0, 0));
      }
      if (block_two_x > getWindowCenter().x) {
        // CASE 2: Both are to the right of middle
        OneLeft();
        TwoLeft();
      }
      if (block_two_x < getWindowCenter().x) {
        // CASE 3: Both are to the left of middle
        OneRight();
        TwoRight();
      }
    } else if (block_one_x + kBlockSize != block_two_x) { // Case where blocks are separated
      if (block_one_x + kBlockSize < getWindowCenter().x) {
        OneRight();
      } else if (block_one_x + kBlockSize > getWindowCenter().x) {
        OneLeft();
      } else if (block_one_x + kBlockSize == getWindowCenter().x) {
        blocks_.GetBlockOne()->SetLinearVelocity(b2Vec2(0, 0));
      }
      if (block_two_x < getWindowCenter().x) {
        TwoRight();
      } else if (block_two_x > getWindowCenter().x) {
        TwoLeft();
      } else if (block_two_x == getWindowCenter().x) {
        blocks_.GetBlockTwo()->SetLinearVelocity(b2Vec2(0, 0));
      }
    }


  }
}

}  // namespace myapp
