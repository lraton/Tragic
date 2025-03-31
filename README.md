# Tragic: Un gioco di carte
                     ____                                                                        ____                 
                   .'* *.'                                                                     .'* *.'
               __/_*_*(_                                                                    __/_*_*(_
              / _______ \\                                                                 / _______ \\
             _\\_)/___\\(_/_                                                              _\\_)/___\\(_/_
            / _((\\- -/))_ \\                                                            / _((\\- -/))_ \\
            \\ \\())(-)(()/ /     ▄▄▄█████▓ ██▀███  ▄▄▄        ▄████  ██▓ ▄████▄         \\ \\())(-)(()/ /
             ' \\(((()))/ '       ▓  ██▒ ▓▒▓██ ▒ ██▒████▄     ██▒ ▀█▒▓██▒▒██▀ ▀█          ' \\(((()))/ '
            / ' \\)).))/ ' \\     ▒ ▓██░ ▒░▓██ ░▄█ ▒██  ▀█▄  ▒██░▄▄▄░▒██▒▒▓█    ▄        / ' \\)).))/ ' \\
           / _ \\ - | - /_  \\    ░ ▓██▓ ░ ▒██▀▀█▄ ░██▄▄▄▄██ ░▓█  ██▓░██░▒▓▓▄ ▄██▒      / _ \\ - | - /_  \\
          (   ( .;''';. .'  )       ▒██▒ ░ ░██▓ ▒██▒▓█   ▓██▒░▒▓███▀▒░██░▒ ▓███▀ ░     (   ( .;''';. .'  )
          _\\ __ /    )\\ __ /_      ▒ ░░   ░ ▒▓ ░▒▓░▒▒   ▓▒█░ ░▒   ▒ ░▓  ░ ░▒ ▒  ░    _\\ __ /    )\\ __ /_
            \\/  \\   ' /  \\/       ░      ░▒ ░ ▒░ ▒   ▒▒ ░  ░   ░  ▒ ░  ░  ▒          \\/  \\   ' /  \\/
              .'  '...' ' )         ░        ░░   ░  ░   ▒   ░ ░   ░  ▒ ░░                .'  '...' ' )
              / /  |  \\ \\                ░          ░  ░      ░  ░  ░ ░                 / /  |  \\ \\
             / .   .   . \\                                           ░                   / .   .   . \\
            /   .     .   \\                        Un gioco di carte                    /   .     .   \\
           /   /   |   \\  \\                                                           /   /   |   \\  \\
         .'   /    b    '.  '.                                                        .'   /    b     '.  '.
     _.-'    /     Bb     '-. '-._                                                _.-'    /     Bb      '-. '-._
 _.-'       |      BBb       '-.  '-.                                         _.-'       |      BBb        '-.  '-.
(________mrf\\____.dBBBb.________)____)                                      (________mrf\\____.dBBBb.________)____)

Final Project Procedural Programming UniPG 2021/2022.

### Project Overview
Tragic: A Card Game is a command-line card game developed in C as part of the Procedural Programming exam. Two wizards engage in a one-on-one duel using decks of magical cards. The game is implemented using three mandatory source files:

- **main.c**: Contains only the `main()` function, responsible for displaying the game menu and handling user input.
- **gamelib.c**: Implements all game logic functions.
- **gamelib.h**: Declares the non-static functions of `gamelib.c` and defines the necessary data structures.

### Game Structure
#### Data Structures
- **struct Mago**: Represents a wizard with a name, life points (starting at 20), and a class (`enum Classe_mago`). It also contains three pointers to manage the deck (`mazzo`), hand (`mano`), and field (`campo`).
- **enum Classe_mago**: Defines wizard classes: `{tenebre, vita, luce}`, which influence card effects.
- **struct Carta**: Represents a card with a type (`enum Tipo_carta`), a `punti_vita` field (which varies based on card type), and a pointer to the next card in the deck.
- **enum Tipo_carta**: Defines possible card types: `{creatura, rimuovi_creatura, infliggi_danno, guarisci_danno}`.

#### Game Flow
The game consists of the following phases:
1. **Game Setup (`imposta_gioco()`)**:
   - Prompts players to enter their names and wizard class.
   - Generates random decks and fills each player's hand with 5 additional cards.
   - Prints all generated data to the screen.
2. **Combat Phase (`combatti()`)**:
   - Randomly selects the first player.
   - Players take turns performing actions: drawing, playing cards, attacking, or passing.
   - The game continues until a player reaches 0 life points or one player's deck runs out of cards.
3. **Game Termination (`termina_gioco()`)**:
   - Ends the game and exits.

#### Player Actions
During their turn, a player can perform one action per category:
- **Draw a Card (`pesca()`)**: Takes a card from the top of the deck and adds it to the hand (if space is available).
- **Play a Card (`gioca_carta()`)**:
  - `rimuovi_creatura`: Removes a creature from the opponent's field.
  - `infliggi_danno`: Deals damage to an enemy creature or the opposing wizard.
  - `guarisci_danno`: Heals the player or one of their creatures.
  - `creatura`: Places a creature on the player's field.
- **Attack (`attacca()`)**:
  - Creatures on the field can attack enemy creatures or the opposing wizard.
  - Damage is deducted from life points or creature health.
  - If a creature’s health reaches 0, it is removed from the game.
- **Pass (`passa()`)**: Ends the player’s turn and switches control to the opponent.

### Additional Features
- Deck generation is randomized, ensuring a different experience each playthrough.
- Wizards' class influences card statistics:
  - `vita`: Creature cards have 50% more life points.
  - `tenebre`: Damage-dealing cards inflict double damage.
  - `luce`: Healing cards restore 3.5 times their usual value.

### Compilation and Execution
1. **Compile separately:**
   ```sh
   gcc -c main.c -std=c11 -Wall
   gcc -c gamelib.c -std=c11 -Wall
   ```
2. **Link object files:**
   ```sh
   gcc -o game main.o gamelib.o
   ```
3. **Run the game:**
   ```sh
   ./game
   ```

