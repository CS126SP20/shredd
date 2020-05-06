// Copyright (c) 2020 CS126SP20. All rights reserved.

#define CATCH_CONFIG_MAIN

#include <mylibrary/blocks.h>

#include <catch2/catch.hpp>

TEST_CASE("Test blocks", "[blocks][init]") {
  // Initialize world and blocks
  b2World* world = new b2World(b2Vec2(0, 0));
  world->SetAllowSleeping(false);
  mylibrary::Blocks blocks;
  blocks.init(world, 300, 500, 350, 500);

  SECTION("Test block positions") {
    // Make sure the positions are correct
    REQUIRE(blocks.GetBlockOnePos().x * 50 == 300);
    REQUIRE(blocks.GetBlockTwoPos().x * 50 == 350);
  }

  SECTION("Test forces") {
    blocks.ApplyForceToOne(b2Vec2(-10, 0));
    blocks.ApplyForceToTwo(b2Vec2(10, 0));
    // Make sure the forces actually change the world...
    world->Step(1.0f / 60.0f, 8, 3);
    // Check to see if positions have changes to see if a force was applied
    REQUIRE(blocks.GetBlockOnePos().x * 50 < 300);
    REQUIRE(blocks.GetBlockTwoPos().x * 50 > 350);
  }
}