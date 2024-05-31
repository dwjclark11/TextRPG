![TextRPGTitleScreen](https://user-images.githubusercontent.com/63356975/230724037-e4c896e6-c1c4-4aca-a286-eac773cc7a15.png)

# C++ Text RPG

This is the repository for my youtube series that I am working on [**Create a C++ Text RPG**!](https://www.youtube.com/watch?v=B2RPgKxsEVs&list=PL3HUvSWOJR7W9YSPUHodF3SZxAS5_unau&index=1&t=13s) 
Currently this is only compatible with windows. It uses direct Windows API calls for the Console, Events, Inputs, etc. I may try to make it cross platform; however, I have a lot of other projects that need work as well, so that is on the back burner currently.

This is a work in progress and there are still many different features that we need. 

## Build
* Clone the repository ```git clone https://github.com/dwjclark11/TextRPG.git```.
* Go into the repo directory and run ```cmake -S . -B build```.
* Go into the build directory and run ```cmake --build . --config Debug``` or ```config Release```.
  * If on MSVC, you can build from the sln that would have been generated.
  * MSVC also creates new Debug/Release folders when building. The ```assets``` folder gets copied to the ```bin``` folder.
  * Move the ```assets``` folder into the Debug/Release folders.   

## Features
Here are some of the features that have already been implemented. 
* StateMachine
  * The state machine allows the user to switch between different game states such as GameState, MenuState, TitleState, etc.
* Typewritter
  * The typewritter is a simple class that will scroll the text based on certain parameters. The typewriter should be used in conjunction with the TextState that has not yet been implemented. 
* Item/Equipment Loaders
  * The Item/Equipment loaders are factories that will create the items for use based on the parameters that are passed in. We are also able to load these items from an XML file.
* XML Parser
  * We have a simple XML parser that uses TinyXML2. The parser can be used to read XML files to help create the items/equipment in the game.
### Typewriter Example
https://user-images.githubusercontent.com/63356975/230790574-a5cfbf9f-69d9-48e1-bdd0-a55347a9fcf6.mp4

## Desired Features
Some of the desired or planned features for the future are:
* Battle system
  * A simple system in where we can battle various enemies. This will also have a battle state
* Shops
  * This is something I already have in the works. We will have different shops where the player can buy items/weapons/armor etc.

Comments, feedback, and ideas are highly appreciated. Please check out the Youtube channel and let me know what you think. I am trying to make this a learning resource, for me as well. I love learning and becoming better everyday
