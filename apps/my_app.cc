// Copyright (c) 2020 [Your Name]. All rights reserved.

#include "my_app.h"
#include <cinder/gl/draw.h>
#include <cinder/gl/gl.h>

#include <cinder/app/App.h>

using cinder::Rectf;

namespace myapp {

using cinder::app::KeyEvent;

MyApp::MyApp() { }

void MyApp::setup() { }

void MyApp::update() { }

void MyApp::draw() {
  DrawBlocks();
}

void MyApp::keyDown(KeyEvent event) { }


void MyApp::DrawBlocks() const {

}
void MyApp::DrawSpikes() const {}
void MyApp::ResetGame() const {}
void MyApp::DrawGameOver() const {}

}  // namespace myapp
