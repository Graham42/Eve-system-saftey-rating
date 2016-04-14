# Solar System Safety Rating for [Eve Online](https://www.eveonline.com/)

This is an application of fuzzy logic for a university course. The goal is to evaluate how safe it
is to travel through a given solar system in Eve Online based on the available information to a
player.

Explanations here are targeted as people who may never have played Eve Online.

## Introduction

Eve Online is a space sim video game with a vast galaxy of solar systems. Different areas of space
can be more dangerous than other depending on several factors such as the number of other players,
the level of enforcement by CONCORD(the space police), or the value of the cargo a player is
carrying.

Players can contract out transportation of goods to other players. These other players want to
fulfill their contract without getting their ship blown up. This tool would theoretically help them
choose a route through systems that they can navigation with relative safety.

## Definitions

* MMO - Massively Multiplayer Online game
* NPC - Non-player Character
* Corporation - A Corporation is similar in concept to a guild in other MMOs. It's a group of
  players who work together. The Corporation is a formalized in-game entity that can do more than a
  player can on their own. For example: anchoring a starbase to a moon's orbit.
* Pilot - Another way to reference a player
* CONCORD - The space police
* Gate - A jump gate allows ships to travel to another solar system

## Inputs

There are many more inputs that could be added but for the purpose of keeping this application
simple, will limit to these 5.

* _SecurityRating_: A solar system's security rating can range from -1 to 1. In systems above 0
  players recieve a penalty for killing other players. In systems rated 0.5 and above CONCORD will
  also show up and destroy attackers. Solar systems with higher security rating have faster CONCORD
  response times.

* _IncursionControl_: From time to time a certain faction of NPCs will invade areas of space. This
  event is called an incursion. If an incursion is active in a system the gates will likely be
  gaurded by NPCs. Based on how much players have pushed back the incursion, the NPCs with have from
  0% to 100% control.

* _NonAlliedPilots_: This is a derived input from two pieces of information available to a player;
  the number of corporation members in space right now, and the average number of players in space
  in the last 30 minutes. The more pilots in space, who are **not** in your corporation, the more
  dangerous it is to travel through a given system.

* _ShipsDestroyed_: The player has the number of ships destroyed in the last hour. If ships have
  been recently destroyed in an area of space it's possibly still dangerous.

## Output Range

Safe, Somewhat Safe, Somewhat Dangerous, Dangerous, Very Dangerous

The output is asymmetric on purpose as a player should understand they are never "Very Safe" if they
have undocked their ship from a station.

# Setup

These steps assume a linux machine

## Fuzzylite

This program relies on the fuzzylite library.

1. Download the source code from https://github.com/fuzzylite/fuzzylite/archive/v5.1.zip
2. Extract the zip and go into that folder in a terminal
3. Follow this

  ```sh
  cd fuzzylite/
  ./build.sh release
  cd release/
  make
  sudo make install
  ```

## This Program

If `make` fails you may need to run `export PKG_CONFIG_PATH="/usr/local/lib/pkgconfig/"` first

```sh
make
./safety-rating.run
```

