// Copyright (c) 2020 [Alice Huang]. All rights reserved.

#include <mylibrary/blocks.h>

const int kBlockSize = 50;

namespace mylibrary {

Blocks::Blocks() : block_one_{nullptr}, block_two_{nullptr} {}

void Blocks::init(b2World* world, float x_one, float y_one, float x_two, float y_two) {
  // Make the blocks
  b2BodyDef body_one;
  body_one.type = b2_dynamicBody;
  body_one.gravityScale = 0;
  body_one.fixedRotation = true; // Will not rotate
  body_one.position.Set(x_one / kPPM, y_one / kPPM);
  block_one_ = world->CreateBody(&body_one);

  b2BodyDef body_two;
  body_two.type = b2_dynamicBody;
  body_two.gravityScale = 0;
  body_two.fixedRotation = true;
  body_two.position.Set(x_two / kPPM, y_two / kPPM);
  block_two_ = world->CreateBody(&body_two);

  // Make them boxes?
  b2PolygonShape polygonShape;
  polygonShape.SetAsBox(kBlockSize / kPPM, kBlockSize / kPPM);
  b2FixtureDef myFixtureDef;
  myFixtureDef.shape = &polygonShape;
  myFixtureDef.density = 1.0f;

  b2PolygonShape polygonShape2;
  polygonShape2.SetAsBox(kBlockSize / kPPM, kBlockSize / kPPM);
  b2FixtureDef myFixtureDef2;
  myFixtureDef2.shape = &polygonShape;
  myFixtureDef2.density = 1.0f;

  block_one_->CreateFixture(&myFixtureDef);
  block_two_->CreateFixture(&myFixtureDef2);
}

b2Vec2 Blocks::GetBlockOnePos() const { return block_one_->GetPosition(); }

b2Vec2 Blocks::GetBlockTwoPos() const { return block_two_->GetPosition(); }

void Blocks::ApplyForceToTwo(const b2Vec2& force) {
  //block_two_->ApplyLinearImpulse(force, block_two_->GetWorldCenter());
  block_two_->ApplyForceToCenter(force);
}

void Blocks::ApplyForceToOne(const b2Vec2& force) {
  //block_one_->ApplyLinearImpulse(force, block_one_->GetWorldCenter());
  block_one_->ApplyForceToCenter(force);
}


}  // namespace mylibrary
