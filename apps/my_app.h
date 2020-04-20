// Copyright (c) 2020 CS126SP20. All rights reserved.

#ifndef FINALPROJECT_APPS_MYAPP_H_
#define FINALPROJECT_APPS_MYAPP_H_

#include <cinder/app/App.h>
#include <Box2D/Box2D.h>
#include <mylibrary/blocks.h>


namespace myapp {

class MyApp : public cinder::app::App {
 public:
  MyApp();
  void setup() override;
  void update() override;
  void draw() override;
  void keyDown(cinder::app::KeyEvent) override;

 private:
  /**
   * This will draw the blocks onto the screen at the correct locations.
   */
  void DrawBlocks() const;

  /**
   * This will draw the spikes at the correct locations
   */
  void DrawSpikes() const;

  /**
   * This function will draw the leaderboard/other features once the game ends
   */
  void DrawGameOver() const;

  /**
   * This function resets the game state so that the user can safely play
   * another game.
   */
  void ResetGame() const;

 private:
  bool paused_;
  const std::string player_name_;
  // TODO: make a game engine
  b2World* world;
  mylibrary::Blocks blocks_;
};

}  // namespace myapp

#endif  // FINALPROJECT_APPS_MYAPP_H_
