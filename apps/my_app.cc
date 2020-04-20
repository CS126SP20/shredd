// Copyright (c) 2020 [Your Name]. All rights reserved.

#include "my_app.h"
#include <cinder/gl/draw.h>
#include <cinder/gl/gl.h>
#include <iostream>

#include <cinder/app/App.h>

using cinder::Rectf;

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
  if (held_keys_.find(KeyEvent::KEY_LEFT) != held_keys_.end() &&
      held_keys_.find(KeyEvent::KEY_RIGHT) != held_keys_.end()) {
    // TODO: figure out why pieces are not moving
    // https://www.iforce2d.net/b2dtut/constant-speed
    blocks_.ApplyForceToOne(b2Vec2(-10, 0));
    blocks_.ApplyForceToTwo(b2Vec2(10, 0));
  } else if (held_keys_.find(KeyEvent::KEY_LEFT) != held_keys_.end()) {
    blocks_.ApplyForceToOne(b2Vec2(-100000000, 0));
    std::cout << blocks_.GetBlockOnePos().x << std::endl;
    std::cout<< "left pressed" << std::endl;
  }
}

void MyApp::draw() {
  DrawBlocks();
}


void MyApp::DrawBlocks() const {
  // Get the current positioning
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

}  // namespace myapp
