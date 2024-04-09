These files are sourced from Eric Hedgrens 1.07 code drop
I will be completing the 1.08 project on top of this code.
I will keep the README the same and add a 1.08 section


1.08



--------------------------------------------------------------------------
# Pokemon

## 1.07

### What it does

This assignment reads a series of csv files from the locations stated in the assignment description and a personal location that I have locally. It first reads each csv by putting each value in each line in a struct for the respective file, then puts all those structs in a vector for the whole csv. Then based off the option in the switch --filename it will print the values for that file by reading from the structs in the vectors, if not filename it prints nothing. It prints as so: variable name: value for each variable.

### How to use it

By default using ANSI color, which should work on any system

#### How to run and compile
```
in the parent direcotry run:
make
./PokemonGame
```

#### How to specify --filename
```
./Pokemon --filename=pokemon
```
pokemon is a valid file name. If it is not a valid filename it does nothing.

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

Q or q - Quit the game.


## How to submit assignmnet

### create zip
```
cp -R Pokemon/ hedgren_eric.assignment-[ass #]
```
```
cd hedgren_eric.assignment-[ass #]
```
```
rm -rf .git .vscode
```
```
cd ..
```
```
tar cvfz hedgren_eric.assignment-[ass #].tar.gz hedgren_eric.assignment-[ass #]
```

### test zip
```
mkdir test
```
```
cd test
```
```
gzip -dc ../hedgren_eric.assignment-1.06.tar.gz |tar xvf -
```
