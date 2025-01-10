# Generation

![Release](https://img.shields.io/badge/Version-v0.2.2-blueviolet)
![Language](https://img.shields.io/badge/Language-C%2B%2B-0052cf)
![Open Source](https://badges.frapsoft.com/os/v2/open-source.svg?v=103)

We are the "Boys At Github" team. We create our own game in C++ using the SFML library.

We developing our own engine specifically for this game, but part of engine can be used for other projects

You can suggest various features in VK messages: https://vk.com/majong562

At the moment the game is not playable. We are at the development stage of the engine. In the future, game mechanics and a plot that are already being written will be added. The game is conceived in the roguelike style. Our team has artists, so all the sprites for the game will be their own.

<p align="center">
    <img src="./sources/Example1.png">
    <img src="./sources/Example2.png">
    <img src="./sources/Example3.png">
</p>

# currently available mechanics:
- walk
  - w a s d
  - Shift (sprint)
- shoot
  - LMB
- weapons (selected by scrolling wheel)
  - Pistol
  - Shotgun
  - Rifle
- Interactible objects (interaction with the X button while on it)
  - portal (teleport you to dangeon)
  - box (give you money)
  - Artifact (improved player stats like mana capacity and health)
- minimap
  - acrivated by Tab
  - move and zoom by mouse
  - can be hold on player by Space
- Enemies
  - Angular Body
- Shop (you can buy something usefull)
  - drugs (healing can be used by H)
  - dasher (aplpy you to dashing while holding Shift)
- Upgrade shop (you can upgrade weapons)

# Our sprites and arts
- **[Textures](https://github.com/George562/Generation/blob/main/sources/textures)**
- **[Portal](https://github.com/George562/Generation/blob/main/sources/textures/Portal3.png)**
- **[Player](https://github.com/George562/Generation/blob/main/sources/textures/Player.png)**

<p align="center">
    <img src="./sources/textures/Portal.png" height="200">
  <img src="./sources/textures/Player.png" width="200">
</p>

# Our music
- **[Main menu music](https://github.com/George562/Generation/blob/main/sources/music/MainMenu.wav)**
- **[Dungeon music](https://github.com/George562/Generation/blob/main/sources/music/Fight1.flac)**
- **[Dungeon music](https://github.com/George562/Generation/blob/main/sources/music/Fight2.flac)**

# Attention

The game may not run on your computer due to the lack of some libraries (.dll). If this happens, then report us about problem and we try to solve it.

If you want to recompile the game, then use the Makefile.
