// Copyright (c) 2020 [Alice Huang]. All rights reserved.

#include "my_app.h"
#include "cinder/ImageIo.h"
#include <cinder/gl/draw.h>
#include <cinder/gl/gl.h>
#include <iostream>
#include "cinder/params/Params.h"
#include <cinder/app/App.h>
#include "cinder/gl/Texture.h"
#include <string>
#include <cinder/audio/audio.h>

using cinder::Rectf;
using namespace ci::app;
using namespace ci;

// Global variable
cinder::audio::VoiceRef background_music;

const double kForce = 5;
const float kBlockSize = 50.0f;
const float kTimeStep = 1.0f / 60.0f;
const float kWallWidth = 0.0f;
const float kPPM = 50.0f; // Convert from pixels to meters

namespace myapp {

using std::chrono::duration_cast;
using std::chrono::seconds;
using std::chrono::milliseconds;
using std::chrono::system_clock;

using cinder::app::KeyEvent;

MyApp::MyApp(): is_home_screen_{true},
                should_flicker_{false},
                score_{0},
                is_start_{true},
                has_collided_{false} {}

void MyApp::setup() {
  // Initialize world, blocks, and walls
  world_ = new b2World(b2Vec2(0, 0));
  world_->SetAllowSleeping(false);
  blocks_.init(world_, getWindowCenter().x - kBlockSize,
      getWindowCenter().y, getWindowCenter().x, getWindowCenter().y);
  CreateWalls();
  image = ci::gl::Texture::create(loadImage
      (loadAsset("background_shredd.jpg")));
  list_section_ = { 0, 0, 0 };
  section_width_ = (float)getWindowHeight() / (float)(list_section_.size() - 1);
  cinder::audio::SourceFileRef sourceFile = cinder::audio::load
      (cinder::app::loadAsset("my_app_audio.mp3"));
  background_music = cinder::audio::Voice::create(sourceFile);
  background_music->start();
}

void MyApp::update() {
  // In order to actually move the objects
  world_->Step(kTimeStep, 8, 3);
  world_->ClearForces();

  // Take care of key movements
  HandleKeyPressed();

  // Handle scoring
  if (!is_home_screen_ && !has_collided_) {
    if (is_start_) {
      time_game_start_ = getElapsedSeconds();
    }
    is_start_ = false;
    score_ = (int)(getElapsedSeconds() - time_game_start_);
  }

}

void MyApp::draw() {
  cinder::gl::enableAlphaBlending();
  gl::clear();
  gl::draw(image, getWindowBounds());
  if (is_home_screen_) {
    DrawHomeScreen();
    DrawBlocks();
  } else if (!is_home_screen_ && !has_collided_) {
    gl::draw(image, getWindowBounds());
    DrawSpikes();
    DrawBlocks();
    DrawGameScreen();
  } else if (has_collided_) {
    if (score_ > high_score_) {
      high_score_ = score_;
    }
    DrawHomeScreen();
    DrawBlocks();
  }


}

void MyApp::DrawHomeScreen() {
  // Text box for title
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

  // Flickering effect for starting instructions:
  // Can adjust numbers in this if statement to change rate of flickering
  if (int(getElapsedSeconds() * 3) % 2 == 0 && !has_collided_) {
    auto instructions = TextBox()
        .alignment(TextBox::CENTER)
        .font(cinder::Font("Impact", 30))
        .size({600, 100})
        .color(Color(0, .5, .5))
        .backgroundColor(ColorA(0, 0, 0, 0))
        .text("press SPACE to begin");
    const auto instructions_size = instructions.getSize();
    const cinder::vec2 loc_instructions = {0, 450};
    const auto instructions_surface = instructions.render();
    const auto instructions_texture =
        cinder::gl::Texture::create(instructions_surface);
    cinder::gl::draw(instructions_texture, loc_instructions);
  }

  // Have to figure out elegant way to do this...
  /*
  if (high_score_ != 0) {
    std::stringstream high_score_text;
    high_score_text << "Best: " << high_score_;
    auto high_score = TextBox()
        .alignment(TextBox::CENTER)
        .font(cinder::Font("Impact", 30))
        .size({600, 70})
        .color(Color(0, .5, .5))
        .backgroundColor(ColorA(0, 0, 0, 0))
        .text(high_score_text.str());
    const auto scores_size = high_score.getSize();
    const cinder::vec2 loc_scores = {0, 250};
    const auto scores_surface = high_score.render();
    const auto scores_texture =
        cinder::gl::Texture::create(scores_surface);
    cinder::gl::draw(scores_texture, loc_scores);
  } */

  // Draw scores
  if (has_collided_) {
    std::stringstream score_text;
    score_text << "Score: " << score_;
    auto your_score = TextBox()
        .alignment(TextBox::CENTER)
        .font(cinder::Font("Impact", 30))
        .size({600, 70})
        .color(Color(0, .5, .5))
        .backgroundColor(ColorA(0, 0, 0, 0))
        .text(score_text.str());
    const auto your_score_size = your_score.getSize();
    const cinder::vec2 loc_your_score = {0, 325};
    const auto your_scores_surface = your_score.render();
    const auto your_scores_texture =
        cinder::gl::Texture::create(your_scores_surface);

    cinder::gl::draw(your_scores_texture, loc_your_score);
  }
}

void MyApp::DrawGameScreen() {
  // Text box for title
  auto box = TextBox()
      .alignment(TextBox::CENTER)
      .font(cinder::Font("Impact", 30))
      .size({600, 100})
      .color(Color(0, .5, .5))
      .backgroundColor(ColorA(0, 0, 0, 0))
      .text(std::to_string(score_));
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

void MyApp::CreateWalls() {
  // Wall one
  b2BodyDef wall_body_def_one;
  wall_body_def_one.type = b2_staticBody;
  wall_body_def_one.position.Set(-50.0f / (kPPM),
                                 double(getWindowHeight()) / (2*kPPM));
  b2PolygonShape wall_box;
  wall_box.SetAsBox(kWallWidth / (2*kPPM),
      getWindowHeight() / (2*kPPM));
  wall_one_ = world_->CreateBody(&wall_body_def_one);
  wall_one_->CreateFixture(&wall_box, 0.0f);

  // Wall two
  b2PolygonShape wall_box2;
  wall_box2.SetAsBox(kWallWidth / (2*kPPM),
      getWindowHeight() / (2*kPPM));
  b2BodyDef wall_body_def_two;
  wall_body_def_two.type = b2_staticBody;
  wall_body_def_two.position.Set(getWindowWidth() / kPPM,
                                 double(getWindowHeight())/ (2*kPPM));
  wall_two_ = world_->CreateBody(&wall_body_def_two);
  wall_two_->CreateFixture(&wall_box2, 0.0f);
}

void MyApp::keyDown(KeyEvent event) {
  // Add the key being held down to held_keys_
  held_keys_.insert(event.getCode());
}

void MyApp::keyUp(KeyEvent event) {
  // Remove released key from held_keys_
  held_keys_.erase(event.getCode());
}

// Spike logic partially derived from:
// https://github.com/OneLoneCoder/videos/blob/master/OneLoneCoder_FlappyBird.cpp
void MyApp::DrawSpikes() {
  // Make it seem like spikes are moving
  spike_position_ += 0.2 * getElapsedSeconds(); // Increase # to make more fast
  if (spike_position_ > section_width_) {
    spike_position_ -= section_width_;
    list_section_.pop_front(); // Spike has moved off screen, so remove
    int i = (rand() % 4); // choose number 0, 1, 2, or 3
    list_section_.push_back(i);
  }
  int current_section = 0; // Keep track of the current section
  for (auto s: list_section_) {
    if (s != 0) {
      // Will randomly draw spikes
      if (s % 4 == 0) {
        DrawLeftSpike(current_section, 300);
      } else if (s % 4 == 1) {
        DrawRightSpike(current_section, 300);
      } else if (s % 4 == 2) {
        DrawLeftSpike(current_section, 150);
        DrawRightSpike(current_section, 150);
      } else if (s % 4 == 3) {
        DrawCenterSpike(current_section);
      }
    }
    current_section++;
  }
}

// Code derived from:
// https://www.geeksforgeeks.org/check-whether-a-given-point-lies-inside-a-triangle-or-not/
float area(int x1, int y1, int x2, int y2, int x3, int y3)
{
  return abs((x1*(y2-y3) + x2*(y3-y1)+ x3*(y1-y2))/2.0);
}

// Code derived from:
// https://www.geeksforgeeks.org/check-whether-a-given-point-lies-inside-a-triangle-or-not/
/* A function to check whether point P(x, y) lies inside the triangle formed
   by A(x1, y1), B(x2, y2) and C(x3, y3) */
bool isInside(int x1, int y1, int x2, int y2, int x3, int y3, int x, int y)
{
  /* Calculate area of triangle ABC */
  float A = area (x1, y1, x2, y2, x3, y3);

  /* Calculate area of triangle PBC */
  float A1 = area (x, y, x2, y2, x3, y3);

  /* Calculate area of triangle PAC */
  float A2 = area (x1, y1, x, y, x3, y3);

  /* Calculate area of triangle PAB */
  float A3 = area (x1, y1, x2, y2, x, y);

  /* Check if sum of A1, A2 and A3 is same as A */
  return (A == A1 + A2 + A3);
}

void MyApp::DrawLeftSpike(int current_section, int size) {
  float y_position_var =
      getWindowHeight() - current_section * section_width_ + spike_position_;
  cinder::gl::color(0, .5, .5);
  gl::drawSolidTriangle(vec2(size,y_position_var - 50),
                        vec2(0, y_position_var - 50),
                        vec2(0, y_position_var));
  // NOTE: GetBlock#Pos returns the top left corner of each block
  // Notice that for left spike, area of collision is block one top left corner
  // y_position_var is bottom of spike
  float block_one_pos_x = blocks_.GetBlockOnePos().x * kPPM; // Store pos
  float block_one_pos_y = blocks_.GetBlockOnePos().y * kPPM;

  if (isInside(0, y_position_var,
               0,y_position_var - 50,
                   size, y_position_var - 50,
                   block_one_pos_x, block_one_pos_y)) {
    has_collided_ = true;
  }
}

void MyApp::DrawRightSpike(int current_section, int size) {
  float y_position_var =
      getWindowHeight() - current_section * section_width_ + spike_position_;
  cinder::gl::color(0, .5, .5);
  gl::drawSolidTriangle(vec2(getWindowWidth() - size,
                         y_position_var - 50),
                        vec2(getWindowWidth(),
                            y_position_var - 50),
                        vec2(getWindowWidth(), y_position_var));
  float block_two_pos_x = blocks_.GetBlockTwoPos().x * kPPM; // Store pos
  float block_two_pos_y = blocks_.GetBlockTwoPos().y * kPPM;

  if (isInside(getWindowWidth() - size,y_position_var - 50,
               getWindowWidth(),y_position_var - 50,
               getWindowWidth(), y_position_var,
               block_two_pos_x + 50, block_two_pos_y)) {
    has_collided_ = true;
  }
}

void MyApp::DrawCenterSpike(int current_section) {
  float y_position_var =
      getWindowHeight() - current_section * section_width_ + spike_position_;
  // Combine two triangles to make a diamond
  cinder::gl::color(0, .5, .5);
  gl::drawSolidTriangle(vec2(getWindowWidth() / 2,
                                  y_position_var - 50),
                        vec2(getWindowWidth() / 2 + 100,
                                  y_position_var - 25),
                        vec2(getWindowWidth() / 2,
                                      y_position_var));

  gl::drawSolidTriangle(vec2(getWindowWidth() / 2,
                                  y_position_var - 50),
                        vec2(getWindowWidth() / 2 - 100,
                                  y_position_var - 25),
                        vec2(getWindowWidth() / 2,
                                      y_position_var));
  float block_one_pos_x = blocks_.GetBlockOnePos().x * kPPM; // Store pos
  float block_one_pos_y = blocks_.GetBlockOnePos().y * kPPM;
  float block_two_pos_x = blocks_.GetBlockTwoPos().x * kPPM; // Store pos
  float block_two_pos_y = blocks_.GetBlockTwoPos().y * kPPM;

  bool second_half_two = isInside(getWindowWidth() / 2,y_position_var - 50,
           getWindowWidth() / 2 + 100,y_position_var - 25,
           getWindowWidth() / 2, y_position_var,
           block_two_pos_x, block_two_pos_y)
               || isInside(getWindowWidth() / 2,y_position_var - 50,
                   getWindowWidth() / 2 + 100,y_position_var - 25,
                   getWindowWidth() / 2, y_position_var,
                   block_two_pos_x, block_two_pos_y - 50);
  bool second_half_one = isInside(getWindowWidth() / 2,y_position_var - 50,
                                  getWindowWidth() / 2 + 100,y_position_var - 25,
                                  getWindowWidth() / 2, y_position_var,
                                  block_one_pos_x, block_one_pos_y)
                         || isInside(getWindowWidth() / 2,y_position_var - 50,
                                     getWindowWidth() / 2 + 100,y_position_var - 25,
                                     getWindowWidth() / 2, y_position_var,
                                     block_one_pos_x, block_one_pos_y - 50);
  bool first_half_two = isInside(getWindowWidth() / 2,y_position_var - 50,
                                 getWindowWidth() / 2 - 100,y_position_var - 25,
                                  getWindowWidth() / 2, y_position_var,
                                  block_two_pos_x + 50, block_two_pos_y)
                         || isInside(getWindowWidth() / 2,y_position_var - 50,
                                     getWindowWidth() / 2 - 100,y_position_var - 25,
                                     getWindowWidth() / 2, y_position_var,
                                     block_two_pos_x + 50, block_two_pos_y - 50);
  bool first_half_one = isInside(getWindowWidth() / 2,y_position_var - 50,
                                getWindowWidth() / 2 - 100,y_position_var - 25,
                                getWindowWidth() / 2, y_position_var,
                                block_one_pos_x + 50, block_one_pos_y)
                       || isInside(getWindowWidth() / 2,y_position_var - 50,
                                   getWindowWidth() / 2 - 100,y_position_var - 25,
                                   getWindowWidth() / 2, y_position_var,
                                   block_one_pos_x + 50, block_one_pos_y - 50);
  if (first_half_one || first_half_two || second_half_one || second_half_two) {
    has_collided_ = true;
  }
}


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
  // If the game is on home screen and user starts game...
  if (held_keys_.find(KeyEvent::KEY_SPACE) != held_keys_.end()) {
    if (has_collided_) {
      has_collided_ = false;
      is_home_screen_ = false;
    }
    if (is_home_screen_) {
      is_home_screen_ = false;
    }
  }
  // Handle game movements
  if (held_keys_.find(KeyEvent::KEY_LEFT) != held_keys_.end() &&
      held_keys_.find(KeyEvent::KEY_RIGHT) != held_keys_.end()) {
    OneLeft();
    TwoRight();
  } else if (held_keys_.find(KeyEvent::KEY_LEFT) != held_keys_.end()) {
    OneLeft();
    TwoLeft();
  } else if (held_keys_.find(KeyEvent::KEY_RIGHT) != held_keys_.end()) {
    OneRight();
    TwoRight();
  } else if (held_keys_.find(KeyEvent::KEY_LEFT) == held_keys_.end() &&
             held_keys_.find(KeyEvent::KEY_RIGHT) == held_keys_.end()) {
    if (block_two_x <= 342 && block_two_x >= 318
        && block_one_x <= 242 && block_one_x >= 218) {
      blocks_.GetBlockOne()->SetLinearVelocity(b2Vec2(0, 0));
      blocks_.GetBlockTwo()->SetLinearVelocity(b2Vec2(0, 0));
    }
    if (block_one_x < 225) {
      OneRight();
    } else if (block_one_x > 225) {
      OneLeft();
    }
    if (block_two_x < 325) {
      TwoRight();
    } else if (block_two_x > 325) {
      TwoLeft();
    }
  }
}

void MyApp::ResetGame() {
  score_ = 0;
  has_collided_ = false;
  time_game_start_ = 0;
  list_section_.clear();
  list_section_ = { 0, 0, 0 };
}

}  // namespace myapp
