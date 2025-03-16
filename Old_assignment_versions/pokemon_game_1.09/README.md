These files are sourced from Eric Hedgrens 1.08 code drop
I will be completing the 1.09 project on top of this code.
I will keep the README the same and add a 1.09 section

1.09

Catch pokemon, in tall grass with pokeballs and can add them to PC storage. Revives, Postions, Pokeballs in game. Pokemart restores items, Pokecenter heals pokemon. Pokemon can be healed in revived while in battle and in the main screen with B. Cannot escape Trainer battles but can run from wild encounters. 

# Pokemon

## 1.08

### What it does

This assignment implements pokemon. At the beginning you have the option of 3 pokemon, hit enter to select. Each trainer is given between 1-6 pokemon. If you run (or don't move using 5) through tall grass you have a 10% chance of finding a pokemon, and it will display its stats. If you battle a trainer it will display your first pokemon and the trainer's pokemon stats. The farther you are from the origin the hgher the level pokemon there are. 

NOTE: Flying right now is kind of buggy and may do a core dump. Values that typically don't work are 2 3-digit values and ones with 200. Other than that it works, it also is a little awkward. I am planning to fix this soon.

### How to use it

By default using ANSI color, which should work on any system

#### How to run and compile
```
in the parent direcotry run:
make
./PokemonGame
```

------------ The rest is not for this assignment --------------

#### How to specify --numtrainers
The default number of trainers is 10.

following is an example of how to set --numtrainers
```
./PokemonGame --numtrainers=20

NOTE: this sets the max number of trainers. A random number of trainers is chosen from the range of the value set with the switch and it divided by 2.
```

#### How to use commands
7 or y - Attempt to move PC one cell to the upper left.
8 or k - Attempt to move PC one cell up.
9 or u - Attempt to move PC one cell to the upper right.
6 or l - Attempt to move PC one cell to the right.
3 or n - Attempt to move PC one cell to the lower right.
2 or j - Attempt to move PC one cell down.
1 or b - Attempt to move PC one cell to the lower left.
4 or h - Attempt to move PC one cell to the left.

">" - Attempt to enter a Pokémart or Pokémon Center. Works only if standing on a
building. Leads to a user interface for the appropriate building. You may simply
add a placeholder for this for now, which you exit with a <.

5 or space or . - Rest for a turn. NPCs still move.

t - Display a list of trainers on the map, with their symbol and position relative to
the PC (e.g.: “r, 2 north and 14 west”).

up arrow - When displaying trainer list, if entire list does not t in screen and not currently at top of list, scroll list up.

down arrow - When displaying trainer list, if entire list does not t in screen and not currently at bottom of list, scroll list down.

esc - When displaying trainer list, return to character control.

Q - Quit the game.


## How to submit assignmnet

### create zip
```
327-tarball.sh
```

### test zip
```
327-test-tarball.sh
cd test
```
remove test
```
cd ..
rm -rf test
```
