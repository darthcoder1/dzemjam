# Next step tasks

* tweak quicktime difficulty
* Display player health
* Interactables destroyable by everyone
    * with penelty
* Loot interactable
* Close the game loop
    * Handle end match conditions
        * Alarm activated
        * Time out
        * Goal reached
    * Uncover traitor 
        * by killing or some screen
        * everyone has a 50% health bullet, so non-traitor can conspire
    * Stats 
    * Restart map
* Define/Implement escape gameplay
    * Alarm activated
    * spawn turrets/enemies


# Random Notes

## Game overview

* multiplayer heist
* unique characters
    * different movement
    * unique abilities
    * different weapons
* supplementing abilites
* fusion of characters
    * 2 chars can fuse into one super-char
* traps
* change of pace between infiltration and escape
* personal goals for every player
    * one of them is a traitor
* playtime ~7,5min
    * 5min infiltration
    * 2,5min esacape
* penelty when traitor kills another player
* interactable items in map to sabotage players progreess
* fails in interactions can be used deliberatly
    * failing players can be suspected traitors
* non-regen health
* health packs / ability
* item exchange

## Abilities

* All abilities feature a quicktime event
    * random difficulty
* hacking
* lockpicking
* violence is last resort
    * increases alarm
* bulky guy
    * can lift someone
    * healthpack
    * force open door
* speedy guy
    * acrobatic
    * lockpick
    * can disarm lasershranks
    * longer cam detection
* hackerman
    * hacking
    * companion bot
    * hackertool
* fusion of 2 chars to gain special power
    * both players control the same fusioned char
    * fusion has a max duration and a cooldown
    * bulky + speedy
        * can violently open hardcore things
          that are otherwise hackeronly
        * fusion  shares health
            * after disengange half-split of health
    * speedy + hacker
        * move through datalines
    * bulky + hacker
        * take over electronics
        * camera take over scouts room
        * when taking over turrets, the hacker needs to be 
          freed by the bulky again or loses health
    * bulky + speedy + hacker
        * back to back standoff

* failing to lift doors can damage/lockout player
* random difficultu of quicktime events

## Replayability

* handcrafted maps
* procedural maps
* challanges
* score/time
* traitor win


## Obstacles

Most obstacles can be avoided for a penelty that raises 
alarm level or decreases health.

* camera
    * hackable/destroyable
* door
    * hackacble/destroyable/lockpickable
* computers
* lasershranks
* buttons/switches
* keys (unique or generic)
* moving walls
* things only traversable by speedy
* turrets
* enemies
    * patrol in infiltration
    * fight when escaping

## Level setup

*  failed interactions can be retried

