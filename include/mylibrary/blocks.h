// Copyright (c) 2020 [Your Name]. All rights reserved.
#ifndef FINALPROJECT_MYLIBRARY_BLOCKS_H_
#define FINALPROJECT_MYLIBRARY_BLOCKS_H_
#include "Box2D/Box2D.h"

namespace mylibrary {

class Blocks {
 public:
  Blocks();

  void init(b2World* world, int x_one, int y_one, int x_two, int y_two);

  void LeftKey();

  void RightKey();

  void BothKeys();

  b2Vec2 GetBlockOnePos() const;

  b2Vec2 GetBlockTwoPos() const;

 private:
  b2Body* block_one_;
  b2Body* block_two_;
};

}  // namespace mylibrary


#endif // FINALPROJECT_MYLIBRARY_BLOCKS_H_
