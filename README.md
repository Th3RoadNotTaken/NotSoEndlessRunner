# NotSoEndlessRunner
 A casual endless runner which actually ends after a specific amount of time
 
 

https://github.com/Th3RoadNotTaken/NotSoEndlessRunner/assets/34506769/de3e924f-7864-4bdc-be74-5c39f21f9bd0

Watch the full gameplay video [here](https://www.youtube.com/watch?v=HsewKU5FTqk&ab_channel=TheRoadNotTaken)

Download the game [here](https://theroadnottaken.itch.io/not-so-endless-runner)

## Steps to run the game 
1. Download the APK from the link provided above
2. Install the APK on your android phone and start playing!

## About the project
I've developed this game using **Unreal Engine** 4.27 and **C++** using the following capabilities - 
* Player Inputs - 
  - Created touch swiping functionality which allows the player to swipe to move and jump.
  - The player automatically runs forward without requiring input.

* Unreal Motion Graphics UI Design (UMG) - 
  - Created widgets for the game timer, distance covered, high score table, and powerups

* Memory Management - 
  - Carefully deallocated memory for various game assets according to the situation the player is in. 
  - Removed floor tiles, obstacles and uncollected powerups from the map as the player keeps progressing.

* Mathematical concepts - 
  - Used interpolation to interpolate the player between different tracks of the floor.
  - Used Randomization from the Math library to calculate random probability for the obstacles and powerups to be spawned.

* Animation - 
  - Created animation blueprints and blendspaces for the character for jogging, running and jumping.

* Game optimization -
  - Refactored the code, minimized function calls, passing values to functions by reference to avoid copies.
  - Used interfaces and inheritance to avoid redundant code.
  - Used dynamic arrays wherever necessary and Unreal's reflection system to participate in garbage collection
