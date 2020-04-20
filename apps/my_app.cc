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
  blocks_.init(world, getWindowCenter().x - 50, getWindowCenter().y,
      getWindowCenter().x, getWindowCenter().y);
}

void MyApp::update() { }

void MyApp::draw() {
  DrawBlocks();
}

void MyApp::keyDown(KeyEvent event) { }


void MyApp::DrawBlocks() const {
  // Get the positioning
  b2Vec2 block_one_pos = blocks_.GetBlockOnePos();
  b2Vec2 block_two_pos = blocks_.GetBlockTwoPos();

  cinder::gl::color(1, 0, 0);
  cinder::gl::drawSolidRect(Rectf(block_one_pos.x,
                                  block_one_pos.y,
                                  block_one_pos.x + 50,
                                  block_one_pos.y + 50));
  cinder::gl::color(0, 1, 0);
  cinder::gl::drawSolidRect(Rectf(block_two_pos.x,
                                  block_two_pos.y,
                                  block_two_pos.x + 50,
                                  block_two_pos.y + 50));
}
void MyApp::DrawSpikes() const {}
void MyApp::ResetGame() const {}
void MyApp::DrawGameOver() const {}

}  // namespace myapp
