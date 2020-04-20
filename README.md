# Final Project: SHREDD

**Author**: Alice Huang - [`aliceh4@illinois.edu`](mailto:aliceh4@illinois.edu)

## About SHREDD
This project will be a take on the game SHREDD. The way the game works is the user has to press or not press certain buttons to make "pieces" avoid hitting various spikes. Every time the user passes a certain spike, their point level increases by one. Every ten points, the game speed picks up. The user keeps on going until they hit one of the spikes and the game ends. Here is a screenshot of what the game looks like:

![](https://is5-ssl.mzstatic.com/image/thumb/Purple7/v4/90/77/e4/9077e41d-c15f-f62c-cb85-2f9bec3a5889/mzl.ixwlrfwx.jpg/300x0w.jpg)

I used to play this game when I was younger, and even though the game itself is not very complicated, it was strangely addicting. I think it will be both interesting and formative to figure out how to code my own version of this game. Additionally, because this game is one that I have personal experience with, I will be more motivated to see this project to completion.

## External Library

After completing the Snake project, the main thing that bothered me about the game was the UI for the leaderboard that was displayed at the end of the game. In order to make my game's UI more attractive, I will be using PretzelGui (a Cinder Block). The link for this Cinder Block is https://github.com/cwhitney/PretzelGui. However, I might change the external library I use if I discover later during the project's development that PretzelGui is not sufficient. Also, SHREDD would work better if the pieces could accelerate, interact with the walls, and detect collisions with spikes. Using a physics engine like box2d (https://github.com/erincatto/Box2D) might be helpful in implementing those features.

## Timeline

By the end of the first week, I hope I can figure out the UI for the two pieces and allow them to be controlled by the user. Adding the spikes that will end the game upon collision with the pieces will be my task for the second week. The third week would be used to fix any problems from the first two weeks. I will treat the third week of this project like our snake assignment: the game's functionality will already completed, but I still have to add queries for a leaderboard, add audio, add visual features like colors, and maybe try speeding up the game after the user passes certain checkpoints without failing.

If I am ahead of this timeline, I will continue working on making the game more visually pleasing, taking inspiration from the original game. I might even try to add some more interesting graphics –– like the pieces exploding if they hit a spike, sparks coming off the pieces when they are against a wall, etc.
