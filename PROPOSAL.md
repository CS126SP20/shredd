# Final Project: SHREDD

**Author**: Alice Huang - [`aliceh4@illinois.edu`](mailto:aliceh4@illinois.edu)

## About SHREDD
This project will be a take on the game SHREDD. The way the game works is the user has to press or not press certain buttons to make "pieces" avoid hitting various spikes. Every time the user passes a certain spike, their point level increases by one. Every ten points, the game speed picks up. The user keeps on going until they hit one of the spikes and the game ends. Here is a screenshot of what the game looks like:

![](https://is5-ssl.mzstatic.com/image/thumb/Purple7/v4/90/77/e4/9077e41d-c15f-f62c-cb85-2f9bec3a5889/mzl.ixwlrfwx.jpg/300x0w.jpg)

I used to play this game when I was younger, and even though the game itself is not very complicated, it was strangely addicting. I think it will be both interesting and formative to figure out how to code my own version of this game. Additionally, because this game is one that I have personal experience with, I will be more motivated to see this project to completion.

## External Library

SHREDD would work better if the pieces could accelerate, interact with the walls, and detect collisions with spikes. Because of this, I will use Box2D (https://github.com/erincatto/Box2D), a physics engine, to implement those features.

## Timeline

First week: Make the two pieces and allow them to be controlled by the user pressing the arrow keys.

Second week: Add the spike UI and further refine the movements of the blocks using Box2D.

Third week: Detect collisions to end the game, add audio, and add a score.

If I am ahead of this timeline, I will continue working on making the game more visually pleasing, taking inspiration from the original game. I might even try to add some more interesting graphics –– like the pieces exploding if they hit a spike, sparks coming off the pieces when they are against a wall, etc.
