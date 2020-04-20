// Copyright (c) 2020 [Alice Huang]. All rights reserved.

#include <mylibrary/blocks.h>

namespace mylibrary {

Blocks::Blocks() : block_one_{nullptr}, block_two_{nullptr} {}

void Blocks::init(b2World* world, int x_one, int y_one, int x_two, int y_two) {
  // Make the blocks
  b2BodyDef body_one;
  body_one.type = b2_dynamicBody;
  body_one.fixedRotation = true; // Will not rotate
  body_one.position.Set(x_one, y_one);
  block_one_ = world->CreateBody(&body_one);

  b2BodyDef body_two;
  body_two.type = b2_dynamicBody;
  body_two.fixedRotation = true;
  body_two.position.Set(x_two, y_two);
  block_two_ = world->CreateBody(&body_two);

  // Make them boxes?
  b2PolygonShape polygonShape;
  polygonShape.SetAsBox(50, 50);
  b2FixtureDef myFixtureDef;
  myFixtureDef.shape = &polygonShape;
  myFixtureDef.density = 1;

  block_one_->CreateFixture(&myFixtureDef);
  block_two_->CreateFixture(&myFixtureDef);
}

b2Vec2 Blocks::GetBlockOnePos() const { return block_one_->GetPosition(); }

b2Vec2 Blocks::GetBlockTwoPos() const { return block_two_->GetPosition(); }

void Blocks::ApplyForceToTwo(const b2Vec2& force) {
  block_two_->ApplyLinearImpulse(force, block_two_->GetWorldCenter());
}

void Blocks::ApplyForceToOne(const b2Vec2& force) {
  block_one_->ApplyLinearImpulse(force, block_one_->GetWorldCenter());
}


}  // namespace mylibrary
