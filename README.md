# {PH-Platformer-Alpha}

## Description

{We are two students from CITM, It's our 2D platformer game called Hanging.}

## Key Features

 - {F1/F2 start from the first/second level}
 - {F3 - Start from the beginning of the current level}
 - {F5 -  Save the current game state}
 - {F6 -  Load the previous state}
 - {F9 - View colliders/logic/path}
 - {F10 - God Mode}
 - {F11 Enable/Disable FPS cap to 30}

## Controls

 - {A , D  - move}
 - {SPACE - jump}
 - The way to kill enemy:  guide the enemy to collide with the spikes.

## Rule

- Game is capped to stable 60 frames per second without vsync.
- The flying enemy "bird" can pathfind to the player avoiding non-walkable areas. 
- The platform enemy "chicken" can pathfind to the player, it also can reach the player by walking and falling down to other platforms.
- Guide the enemies to collide with the spikes to destroy them, don't touch them!
- The game state will be auto save if player passes by the checkpoint.
- Before checkpoint, the player will be teleported to the start point if his health -1.
- After checkpoint, the player will be teleported to the checkpoint if his health -1.
- Heart: player's health +1.
- Coin: player's score + 100.
- One enemy destroyed: player's score + 200.
- Each level passed: player's score + 500.
- Try to pass the level and get a best score!

## Developers

 - {Hang Xue} - {programmer, artist}
 - {Pablo Llorente} - {programmer, developer}

## License

This project is licensed under an unmodified MIT license, which is an OSI-certified license that allows static linking with closed source software. Check [LICENSE](LICENSE) for further details.

{AdditionalLicenses}
