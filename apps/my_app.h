// Copyright (c) 2020 CS126SP20. All rights reserved.

#ifndef FINALPROJECT_APPS_MYAPP_H_
#define FINALPROJECT_APPS_MYAPP_H_

#include <Box2D/Box2D.h>
#include <cinder/app/App.h>
#include <cinder/gl/Texture.h>
#include <cinder/params/Params.h>
#include <mylibrary/blocks.h>

#include <set>

namespace myapp {

using std::chrono::seconds;

class MyApp : public cinder::app::App {
 public:
  MyApp();
  void setup() override;
  void update() override;
  void draw() override;
  void keyDown(cinder::app::KeyEvent) override;
  void keyUp(cinder::app::KeyEvent) override;

 // The private helper functions for this class
 private:
  /**
   * This will draw the blocks onto the screen at the correct locations.
   */
  void DrawBlocks() const;

  /**
   * Create the wall objects.
   */
  void CreateWalls();

  /**
   * This will draw the spikes at the correct locations
   */
  void DrawSpikes();

  /**
   * Draws the left spike.
   *
   * @param current_section the section the spike should be in
   * @param length          the length of the spike
   */
  void DrawLeftSpike(int current_section, int length);

  /**
   * Draws the right spike.
   *
   * @param current_section the section the spike should be in
   * @param length          the length of the spike
   */
  void DrawRightSpike(int current_section, int length);

  /**
   * Draws the center spike (a diamond).
   *
   * @param current_section the section the spike should be in
   */
  void DrawCenterSpike(int current_section);

  /**
   * Draws the players score on the screen.
   */
  void DrawGameScreen();

  /**
   * Returns the area of a triangle given vertices.
   * Code derived from: https://www.geeksforgeeks.org/check-whether-a-given-point-lies-inside-a-triangle-or-not/
   *
   * @param x1 the x-coordinate of first vertex
   * @param y1 the y-coordinate of first vertex
   * @param x2 the x-coordinate of second vertex
   * @param y2 the y-coordinate of second vertex
   * @param x3 the x-coordinate of third vertex
   * @param y3 the y-coordinate of third vertex
   * @return the area of the triangle formed.
   */
  float Area(int x1, int y1, int x2, int y2, int x3, int y3);

  /**
   * Determine whether or not a given point lies within a triangle. This
   * function and Area are used together to detect collisions in this game.
   * Code derived from: https://www.geeksforgeeks.org/check-whether-a-given-point-lies-inside-a-triangle-or-not/
   *
   * @param x1 the x-coordinate of first vertex
   * @param y1 the y-coordinate of first vertex
   * @param x2 the x-coordinate of second vertex
   * @param y2 the y-coordinate of second vertex
   * @param x3 the x-coordinate of third vertex
   * @param y3 the y-coordinate of third vertex
   * @param x  the x-coordinate of the point we are examining
   * @param y  the y-coordinate of the point we are examining
   * @return true if the point (x,y) intersects the triangle, false otherwise
   */
  bool IsInside(int x1, int y1, int x2, int y2, int x3, int y3, int x, int y);

  /**
   * Draws the home screen
   */
  void DrawHomeScreen();

  /**
   * This function will handle all the key pressing for the game. Will handle
   * player movements, pressing space bar to start the game, etc.
   */
  void HandleKeyPressed();

  /**
   * The following are the functions for applying forces on the blocks. For
   * example, OneLeft moves the first block to the left and TwoRight moves the
   * second block to the right.
   */
  void OneLeft();
  void OneRight();
  void TwoLeft();
  void TwoRight();

 // Private variables
 private:

  // Simple variables
  bool is_home_screen_;
  bool should_flicker_;
  bool is_start_;
  double time_game_start_;
  bool has_collided_;
  int score_;
  int high_score_;

  // World
  b2World* world_;

  // Blocks
  mylibrary::Blocks blocks_;

  // Keeps track of which keys are currently being held down
  std::set<int> held_keys_;

  // Wall variables
  b2Body* wall_one_;
  b2Body* wall_two_;

  // Background image
  ci::gl::TextureRef image;

  // Spike variables
  float section_width_;
  std::list<int> list_section_;
  float spike_position_ = 0.0f;

};

}  // namespace myapp

#endif  // FINALPROJECT_APPS_MYAPP_H_
