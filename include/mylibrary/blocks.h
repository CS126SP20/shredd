// Copyright (c) 2020 [Your Name]. All rights reserved.
#ifndef FINALPROJECT_MYLIBRARY_BLOCKS_H_
#define FINALPROJECT_MYLIBRARY_BLOCKS_H_
#include "Box2D/Box2D.h"

namespace mylibrary {

class Blocks {
 public:
  Blocks();

  /**
   * Initialize the blocks correctly.
   *
   * @param world the world that these blocks will be generated in
   * @param x_one the x position of the first block
   * @param y_one the y position of the first block
   * @param x_two the x position of the second block
   * @param y_two the y position of the second block
   */
  void init(b2World* world, int x_one, int y_one, int x_two, int y_two);

  /**
   * Return block_one_'s current position.
   *
   * @return a vector that represents the block's current position
   */
  b2Vec2 GetBlockOnePos() const;

  /**
   * Return block_two_'s current position.
   *
   * @return a vector that represents the block's current position
   */
  b2Vec2 GetBlockTwoPos() const;

  /**
   * Applies a force to block_one_.
   *
   * @param force the force to be applied
   */
  void ApplyForceToOne(const b2Vec2& force);

  /**
   * Applies a force to block_two_.
   *
   * @param force the force to be applied
   */
  void ApplyForceToTwo(const b2Vec2& force);

  b2Body* GetBlockOne() const { return block_one_; }
  b2Body* GetBlockTwo() const { return block_two_; }

 private:
  // Variables that represent the two blocks
  b2Body* block_one_;
  b2Body* block_two_;
};

}  // namespace mylibrary


#endif // FINALPROJECT_MYLIBRARY_BLOCKS_H_
