//Mazharul Maaz     ICS3U     January 19th 2018       Ms. Cullum
//This program lets two players go against each other in a Pokemon Battle
//Same concept as a normal Pokemon with limited functionalities

#pragma warning(disable:4996)

// You must include the allegro header files
#include <allegro5/allegro.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>
#include <stdio.h>
#include <string.h>

//Allegro stuff that needs to be declared and initialized for most of the functions to work
ALLEGRO_EVENT_QUEUE* event_queue = nullptr;
ALLEGRO_DISPLAY* display = nullptr;
ALLEGRO_BITMAP* background1 = nullptr;
ALLEGRO_BITMAP* background2 = nullptr;
ALLEGRO_BITMAP* background3 = nullptr;
ALLEGRO_FONT* font1 = nullptr;
ALLEGRO_FONT* font2 = nullptr;
ALLEGRO_EVENT ev;
bool game = true;

//Variables for colors
#define RED		 al_map_rgb(230, 25, 75)
#define GREEN	 al_map_rgb(60, 180, 75)
#define YELLOW	 al_map_rgb(255, 225, 25)
#define BLUE	 al_map_rgb(0, 130, 200)
#define ORANGE   al_map_rgb(245, 130, 48)
#define PURPLE	 al_map_rgb(145, 30, 180)
#define BROWN	 al_map_rgb(170, 110, 40)
#define BLACK	 al_map_rgb(0, 0, 0)

//Structure for the abilities of Pokemons
struct Abilities {
    char abilityName[20];
    char abilityType[5];
    int damage;
    int counter;
};

//Structures for the Pokemons themselves
struct Pokemons {
    char pokeType[8];
    char pokeName[20];
    Abilities ability1;
    Abilities ability2;
    Abilities ability3;
    int health;
};

//Structure for player
struct Player {
    char name[10];
    Pokemons pokemon[3];
};

int initializeAllegro();
void refreshScreen1();
void refreshScreen2();
void refreshScreen3();
void printName(char player1Name[]);
void pickPokemons(char pokeName[], char playerName[]);
void drawPokemons(char playerName[]);
void readAbilities(char pokemon[], char ability1[], char ability2[], char ability3[], char type[]);
void initializePokemons(Pokemons& pokemon);
void drawPokeBattle(char pokemon1[], int x1, int y1, char pokemon2[], int x2, int y2, char word1[], char word2[], char word3[], int& health1, int& health2, int counter1, int counter2, char player1Name[], char player2Name[], char poke1Type[], char poke2Type[]);
void userBattle(Pokemons& pokemon1, Pokemons& pokemon2, int x1, int x2, int y1, int y2, char player1Name[], char player2Name[], char attackerType[], char defenderType[], int beforeHealth1, int beforeHealth2, int& damage1, char& damage2);
void drawAttack(Pokemons& pokemon1, Pokemons& pokemon2, int x1, int x2, int y1, int y2, char player1Name[], char player2Name[], char attackerType[], char defenderType[], int beforeHealth1, int beforeHealth2, int& damage1, char& damage2);
char attack2Damage(char ability[]);
int attackDamage(int& health, int damage, char attackerType[], char defenderType[]);
void drawHealth(int& health1, int& health2);
void damageDone(int& player1Health, int& player2Health, int player1Damage, char player1Special, int player2Damage, char player2Special, bool& dragonRage1, bool& dragonRage2);
int battleScene(Pokemons& pokemon1, Pokemons& pokemon2, char player1Name[], char player2Name[]);
void winnerScene(Player player);
void showTutorial();

int main(int argc, char* argv[]) {

    //Creates two variables to store in information on both players
    Player player1;
    Player player2;

    //Initializes all the allegro stuff to make sure the display is correct
    initializeAllegro();

    showTutorial();

    //Creates the picking stage background
    refreshScreen1();

    //Initializes the names of the players
    strcpy(player1.name, "          ");
    strcpy(player2.name, "          ");
    int i = 0;

    //Draws text
    al_draw_text(font1, BLACK, 75, 435, ALLEGRO_ALIGN_LEFT, "Player1, what is your first name:");
    al_flip_display();

    //Loop was created so that player has to at least have 1 letter in their name
    while (strcmp(player1.name, "          ") == 0) {
        //player 1 types their name and as they do that, the text is displayed on screen
        printName(player1.name);
    }

    //Refreshes the picking stage background
    refreshScreen1();

    al_draw_textf(font1, BLACK, 75, 435, ALLEGRO_ALIGN_LEFT, "%s, pick your Pokemons!", player1.name);
    al_flip_display();

    //Draws the Pokemon on the screen
    drawPokemons(player1.name);

    game = true;

    //ev.keyboard.keycode set to a key outside of the keyboard so that the program doesn't consider one key for several input; Essentially resetting the value to default
    ev.keyboard.keycode = ALLEGRO_KEY_BUTTON_L1;
    //Function to pick first Pokemon
    pickPokemons(player1.pokemon[0].pokeName, player1.name);

    //Lets player 1 pick 3 different Pokemon
    //Loop was created so that player can't pick the same Pokemon multiple times
    do {
        //ev.keyboard.keycode set to a key outside of the keyboard so that the program doesn't consider one key for several input; Essentially resetting the value to default
        ev.keyboard.keycode = ALLEGRO_KEY_BUTTON_L1;
        //Function to pick second Pokemon
        pickPokemons(player1.pokemon[1].pokeName, player1.name);
    } while (strcmp(player1.pokemon[0].pokeName, player1.pokemon[1].pokeName) == 0);

    do {
        //ev.keyboard.keycode set to a key outside of the keyboard so that the program doesn't consider one key for several input; Essentially resetting the value to default
        ev.keyboard.keycode = ALLEGRO_KEY_BUTTON_L1;
        //Function to pick third Pokemon
        pickPokemons(player1.pokemon[2].pokeName, player1.name);
    } while (strcmp(player1.pokemon[2].pokeName, player1.pokemon[1].pokeName) == 0 || strcmp(player1.pokemon[0].pokeName, player1.pokemon[2].pokeName) == 0);


    //These lines were copy and pasted off lines 102-143. Same comments apply
    refreshScreen1();
    al_draw_text(font1, BLACK, 75, 435, ALLEGRO_ALIGN_LEFT, "Player2, what is your first name:");
    al_flip_display();

    while (strcmp(player2.name, "          ") == 0) {
        printName(player2.name);
    }

    refreshScreen1();

    al_draw_textf(font1, BLACK, 75, 435, ALLEGRO_ALIGN_LEFT, "%s, pick your Pokemons!", player2.name);
    al_flip_display();

    drawPokemons(player2.name);

    game = true;
    i = 0;

    ev.keyboard.keycode = ALLEGRO_KEY_BUTTON_L1;
    pickPokemons(player2.pokemon[0].pokeName, player2.name);

    do {
        ev.keyboard.keycode = ALLEGRO_KEY_BUTTON_L1;
        pickPokemons(player2.pokemon[1].pokeName, player2.name);
    } while (strcmp(player2.pokemon[0].pokeName, player2.pokemon[1].pokeName) == 0);

    do {
        ev.keyboard.keycode = ALLEGRO_KEY_BUTTON_L1;
        pickPokemons(player2.pokemon[2].pokeName, player2.name);
    } while (strcmp(player2.pokemon[2].pokeName, player2.pokemon[1].pokeName) == 0 || strcmp(player2.pokemon[0].pokeName, player2.pokemon[2].pokeName) == 0);

    //Initializes all the Pokemon picked for both players.
    while (i < 3) {
        readAbilities(player2.pokemon[i].pokeName, player2.pokemon[i].ability1.abilityName, player2.pokemon[i].ability2.abilityName, player2.pokemon[i].ability3.abilityName, player2.pokemon[i].pokeType);
        readAbilities(player1.pokemon[i].pokeName, player1.pokemon[i].ability1.abilityName, player1.pokemon[i].ability2.abilityName, player1.pokemon[i].ability3.abilityName, player1.pokemon[i].pokeType);
        initializePokemons(player1.pokemon[i]);
        initializePokemons(player2.pokemon[i]);
        i++;
    }

    //Function for Player1 Pokemon1 VS Player2 Pokemon1
    battleScene(player1.pokemon[0], player2.pokemon[0], player1.name, player2.name);

    //Extremely long and complicated if statements that goes through all the possible outcomes of which Pokemon faint first and at the end, determines  who wins the game.
    //Goes into this if loop if player 2's first Pokemon destroys player 1's first Pokemon
    if (player1.pokemon[0].health <= 0) {
        battleScene(player1.pokemon[1], player2.pokemon[0], player1.name, player2.name);

        if (player2.pokemon[0].health <= 0) {
            battleScene(player2.pokemon[1], player1.pokemon[1], player2.name, player1.name);

            if (player1.pokemon[1].health <= 0) {
                battleScene(player1.pokemon[2], player2.pokemon[1], player1.name, player2.name);

                if (player1.pokemon[2].health <= 0) {
                    //Function for displaying end game scene
                    winnerScene(player2);
                }
                if (player2.pokemon[1].health <= 0) {
                    battleScene(player2.pokemon[2], player1.pokemon[2], player2.name, player1.name);

                    if (player1.pokemon[2].health <= 0) {
                        winnerScene(player2);
                    }
                    if (player2.pokemon[2].health <= 0) {
                        winnerScene(player1);
                    }
                }
            }
            if (player2.pokemon[1].health <= 0) {
                battleScene(player2.pokemon[2], player1.pokemon[1], player2.name, player1.name);

                if (player2.pokemon[2].health <= 0) {
                    winnerScene(player1);
                }
                if (player1.pokemon[1].health <= 0) {
                    battleScene(player1.pokemon[2], player2.pokemon[2], player1.name, player2.name);

                    if (player1.pokemon[2].health <= 0) {
                        winnerScene(player2);
                    }
                    if (player2.pokemon[2].health <= 0) {
                        winnerScene(player1);
                    }
                }
            }
        }
        if (player1.pokemon[1].health <= 0) {
            battleScene(player1.pokemon[2], player2.pokemon[0], player1.name, player2.name);

            if (player1.pokemon[2].health <= 0) {
                winnerScene(player2);
            }
            if (player2.pokemon[0].health <= 0) {
                battleScene(player2.pokemon[1], player1.pokemon[2], player2.name, player1.name);

                if (player1.pokemon[2].health <= 0) {
                    winnerScene(player2);
                }
                if (player2.pokemon[1].health <= 0) {
                    battleScene(player2.pokemon[2], player1.pokemon[2], player2.name, player1.name);

                    if (player1.pokemon[2].health <= 0) {
                        winnerScene(player2);
                    }
                    if (player2.pokemon[2].health <= 0) {
                        winnerScene(player1);
                    }
                }
            }
        }
    }

    //Goes into this if loop if player 1's first Pokemon destroys player 2's first Pokemon
    if (player2.pokemon[0].health <= 0) {
        battleScene(player2.pokemon[1], player1.pokemon[0], player2.name, player1.name);

        if (player1.pokemon[0].health <= 0) {
            battleScene(player1.pokemon[1], player2.pokemon[1], player1.name, player2.name);

            if (player2.pokemon[1].health <= 0) {
                battleScene(player2.pokemon[2], player1.pokemon[1], player2.name, player1.name);

                if (player2.pokemon[2].health <= 0) {
                    winnerScene(player1);
                }
                if (player1.pokemon[1].health <= 0) {
                    battleScene(player1.pokemon[2], player2.pokemon[2], player1.name, player2.name);

                    if (player2.pokemon[2].health <= 0) {
                        winnerScene(player1);
                    }
                    if (player1.pokemon[2].health <= 0) {
                        winnerScene(player2);
                    }
                }
            }
            if (player1.pokemon[1].health <= 0) {
                battleScene(player1.pokemon[2], player2.pokemon[1], player1.name, player2.name);

                if (player1.pokemon[2].health <= 0) {
                    winnerScene(player2);
                }
                if (player2.pokemon[1].health <= 0) {
                    battleScene(player2.pokemon[2], player1.pokemon[2], player2.name, player1.name);

                    if (player2.pokemon[2].health <= 0) {
                        winnerScene(player1);
                    }
                    if (player1.pokemon[2].health <= 0) {
                        winnerScene(player2);
                    }
                }
            }
        }
        if (player2.pokemon[1].health <= 0) {
            battleScene(player2.pokemon[2], player1.pokemon[0], player2.name, player1.name);

            if (player2.pokemon[2].health <= 0) {
                winnerScene(player1);
            }
            if (player1.pokemon[0].health <= 0) {
                battleScene(player1.pokemon[1], player2.pokemon[2], player1.name, player2.name);

                if (player2.pokemon[2].health <= 0) {
                    winnerScene(player1);
                }
                if (player1.pokemon[1].health <= 0) {
                    battleScene(player1.pokemon[2], player2.pokemon[2], player1.name, player2.name);

                    if (player2.pokemon[2].health <= 0) {
                        winnerScene(player1);
                    }
                    if (player1.pokemon[2].health <= 0) {
                        winnerScene(player2);
                    }
                }
            }
        }
    }

    // Free memory and return with successful return code.
    al_destroy_display(display);
    al_destroy_bitmap(background1);
    al_destroy_bitmap(background2);
    al_destroy_bitmap(background3);
    return 0;
}
int initializeAllegro() {

    //Initializes all the Allegro stuff.
    const int SCREEN_W = 960;       // screen width
    const int SCREEN_H = 560;       // screen height

    // Initialize Allegro
    al_init();

    // Initialize image add on
    if (!al_init_image_addon()) {
        al_show_native_message_box(display, "Error", "Error", "Failed to initialize image addon!", nullptr, ALLEGRO_MESSAGEBOX_ERROR);
        return -1;
    }

    // Initialize display
    display = al_create_display(SCREEN_W, SCREEN_H);
    if (!display) {
        al_show_native_message_box(display, "Error", "Error", "Failed to initialize display!", nullptr, ALLEGRO_MESSAGEBOX_ERROR);
        return -1;
    }
    al_set_window_title(display, "Pokemon Game");

    // Initialize primative add on
    if (!al_init_primitives_addon()) {
        al_show_native_message_box(display, "Error", "Error", "Failed to initialize primatives addon!", nullptr, ALLEGRO_MESSAGEBOX_ERROR);
        return -1;
    }

    event_queue = al_create_event_queue();
    if (!event_queue) {
        al_show_native_message_box(display, "Error", "Error", "Failed to create event_queue!", nullptr, ALLEGRO_MESSAGEBOX_ERROR);
        al_destroy_display(display);
        return -1;
    }

    // Initialize keyboard routines
    if (!al_install_keyboard()) {
        al_show_native_message_box(display, "Error", "Error", "failed to initialize the keyboard!", nullptr, ALLEGRO_MESSAGEBOX_ERROR);
        return -1;
    }

    al_init_font_addon(); // initialize the font1 addon
    al_init_ttf_addon();// initialize the ttf (True Type font1) addon
    // load the specific font1 you want
    font1 = al_load_ttf_font("Pokemon GB.ttf", 18, 0);
    if (!font1) {
        al_show_native_message_box(display, "Error", "Error", "Could not load Pokemon GB.ttf", nullptr, ALLEGRO_MESSAGEBOX_ERROR);
        return -1;
    }

    font2 = al_load_ttf_font("Pokemon GB.ttf", 32, 0);
    if (!font2) {
        al_show_native_message_box(display, "Error", "Error", "Could not load Pokemon GB.ttf", nullptr, ALLEGRO_MESSAGEBOX_ERROR);
        return -1;
    }

    // Load the bitmap into the Bitmap structure
    // image file must be in same directory.
    // Particularly check return code of this type of function that will fail if file not found.
    background1 = al_load_bitmap("PickingScene.jpg");
    if (!background1) {
        al_show_native_message_box(display, "Error", "Error", "Failed to load background1!", nullptr, ALLEGRO_MESSAGEBOX_ERROR);
        al_destroy_display(display);
        return -1;
    }

    background2 = al_load_bitmap("BattleScene.png");
    if (!background2) {
        al_show_native_message_box(display, "Error", "Error", "Failed to load background2!", nullptr, ALLEGRO_MESSAGEBOX_ERROR);
        al_destroy_display(display);
        return -1;
    }

    background3 = al_load_bitmap("WinnerScene.png");
    if (!background3) {
        al_show_native_message_box(display, "Error", "Error", "Failed to load background3!", nullptr, ALLEGRO_MESSAGEBOX_ERROR);
        al_destroy_display(display);
        return -1;
    }

    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_keyboard_event_source());
}

void refreshScreen1() {
    //Function for the picking stage background
    al_draw_bitmap(background1, 0, 0, 0);
    al_flip_display();
}

void refreshScreen2() {
    //Function for the fighting stage background
    al_draw_bitmap(background2, 0, 0, 0);
    al_flip_display();
}

void refreshScreen3() {
    //Function for the winning stage background
    al_draw_bitmap(background3, 0, 0, 0);
    al_flip_display();
}

void printName(char playerName[]) {

    //Function to display the name of the player and set the value to player1.name
    ALLEGRO_BITMAP* backSpace = nullptr;
    backSpace = al_load_bitmap("Backspace.jpg");

    int i = 0;
    game = true;

    //Loops until either the max number of letters is reached or enter is pressed
    while (game == true) {

        //Code for if the user presses any letters, space bar, enter or backspace
        al_wait_for_event(event_queue, &ev);
        if (i < 10) {
            switch (ev.keyboard.keycode) {
            case ALLEGRO_KEY_A:
                if (ALLEGRO_EVENT_KEY_UP == ev.type) {
                    al_draw_text(font1, BLACK, 125 + 25 * i, 470, ALLEGRO_ALIGN_LEFT, "A");
                    playerName[i] = 'A';
                    i++;
                }
                break;
            case ALLEGRO_KEY_B:
                if (ALLEGRO_EVENT_KEY_UP == ev.type) {
                    al_draw_text(font1, BLACK, 125 + 25 * i, 470, ALLEGRO_ALIGN_LEFT, "B");
                    playerName[i] = 'B';
                    i++;
                }
                break;
            case ALLEGRO_KEY_C:
                if (ALLEGRO_EVENT_KEY_UP == ev.type) {
                    al_draw_text(font1, BLACK, 125 + 25 * i, 470, ALLEGRO_ALIGN_LEFT, "C");
                    playerName[i] = 'C';
                    i++;
                }
                break;
            case ALLEGRO_KEY_D:
                if (ALLEGRO_EVENT_KEY_UP == ev.type) {
                    al_draw_text(font1, BLACK, 125 + 25 * i, 470, ALLEGRO_ALIGN_LEFT, "D");
                    playerName[i] = 'D';
                    i++;
                }
                break;
            case ALLEGRO_KEY_E:
                if (ALLEGRO_EVENT_KEY_UP == ev.type) {
                    al_draw_text(font1, BLACK, 125 + 25 * i, 470, ALLEGRO_ALIGN_LEFT, "E");
                    playerName[i] = 'E';
                    i++;
                }
                break;
            case ALLEGRO_KEY_F:
                if (ALLEGRO_EVENT_KEY_UP == ev.type) {
                    al_draw_text(font1, BLACK, 125 + 25 * i, 470, ALLEGRO_ALIGN_LEFT, "F");
                    playerName[i] = 'F';
                    i++;
                }
                break;
            case ALLEGRO_KEY_G:
                if (ALLEGRO_EVENT_KEY_UP == ev.type) {
                    al_draw_text(font1, BLACK, 125 + 25 * i, 470, ALLEGRO_ALIGN_LEFT, "G");
                    playerName[i] = 'G';
                    i++;
                }
                break;
            case ALLEGRO_KEY_H:
                if (ALLEGRO_EVENT_KEY_UP == ev.type) {
                    al_draw_text(font1, BLACK, 125 + 25 * i, 470, ALLEGRO_ALIGN_LEFT, "H");
                    playerName[i] = 'H';
                    i++;
                }
                break;
            case ALLEGRO_KEY_I:
                if (ALLEGRO_EVENT_KEY_UP == ev.type) {
                    al_draw_text(font1, BLACK, 125 + 25 * i, 470, ALLEGRO_ALIGN_LEFT, "I");
                    playerName[i] = 'I';
                    i++;
                }
                break;
            case ALLEGRO_KEY_J:
                if (ALLEGRO_EVENT_KEY_UP == ev.type) {
                    al_draw_text(font1, BLACK, 125 + 25 * i, 470, ALLEGRO_ALIGN_LEFT, "J");
                    playerName[i] = 'J';
                    i++;
                }
                break;
            case ALLEGRO_KEY_K:
                if (ALLEGRO_EVENT_KEY_UP == ev.type) {
                    al_draw_text(font1, BLACK, 125 + 25 * i, 470, ALLEGRO_ALIGN_LEFT, "K");
                    playerName[i] = 'K';
                    i++;
                }
                break;
            case ALLEGRO_KEY_L:
                if (ALLEGRO_EVENT_KEY_UP == ev.type) {
                    al_draw_text(font1, BLACK, 125 + 25 * i, 470, ALLEGRO_ALIGN_LEFT, "L");
                    playerName[i] = 'L';
                    i++;
                }
                break;
            case ALLEGRO_KEY_M:
                if (ALLEGRO_EVENT_KEY_UP == ev.type) {
                    al_draw_text(font1, BLACK, 125 + 25 * i, 470, ALLEGRO_ALIGN_LEFT, "M");
                    playerName[i] = 'M';
                    i++;
                }
                break;
            case ALLEGRO_KEY_N:
                if (ALLEGRO_EVENT_KEY_UP == ev.type) {
                    al_draw_text(font1, BLACK, 125 + 25 * i, 470, ALLEGRO_ALIGN_LEFT, "N");
                    playerName[i] = 'N';
                    i++;
                }
                break;
            case ALLEGRO_KEY_O:
                if (ALLEGRO_EVENT_KEY_UP == ev.type) {
                    al_draw_text(font1, BLACK, 125 + 25 * i, 470, ALLEGRO_ALIGN_LEFT, "O");
                    playerName[i] = 'O';
                    i++;
                }
                break;
            case ALLEGRO_KEY_P:
                if (ALLEGRO_EVENT_KEY_UP == ev.type) {
                    al_draw_text(font1, BLACK, 125 + 25 * i, 470, ALLEGRO_ALIGN_LEFT, "P");
                    playerName[i] = 'P';
                    i++;
                }
                break;
            case ALLEGRO_KEY_Q:
                if (ALLEGRO_EVENT_KEY_UP == ev.type) {
                    al_draw_text(font1, BLACK, 125 + 25 * i, 470, ALLEGRO_ALIGN_LEFT, "Q");
                    playerName[i] = 'Q';
                    i++;
                }
                break;
            case ALLEGRO_KEY_R:
                if (ALLEGRO_EVENT_KEY_UP == ev.type) {
                    al_draw_text(font1, BLACK, 125 + 25 * i, 470, ALLEGRO_ALIGN_LEFT, "R");
                    playerName[i] = 'R';
                    i++;
                }
                break;
            case ALLEGRO_KEY_S:
                if (ALLEGRO_EVENT_KEY_UP == ev.type) {
                    al_draw_text(font1, BLACK, 125 + 25 * i, 470, ALLEGRO_ALIGN_LEFT, "S");
                    playerName[i] = 'S';
                    i++;
                }
                break;
            case ALLEGRO_KEY_T:
                if (ALLEGRO_EVENT_KEY_UP == ev.type) {
                    al_draw_text(font1, BLACK, 125 + 25 * i, 470, ALLEGRO_ALIGN_LEFT, "T");
                    playerName[i] = 'T';
                    i++;
                }
                break;
            case ALLEGRO_KEY_U:
                if (ALLEGRO_EVENT_KEY_UP == ev.type) {
                    al_draw_text(font1, BLACK, 125 + 25 * i, 470, ALLEGRO_ALIGN_LEFT, "U");
                    playerName[i] = 'U';
                    i++;
                }
                break;
            case ALLEGRO_KEY_V:
                if (ALLEGRO_EVENT_KEY_UP == ev.type) {
                    al_draw_text(font1, BLACK, 125 + 25 * i, 470, ALLEGRO_ALIGN_LEFT, "V");
                    playerName[i] = 'V';
                    i++;
                }
                break;
            case ALLEGRO_KEY_W:
                if (ALLEGRO_EVENT_KEY_UP == ev.type) {
                    al_draw_text(font1, BLACK, 125 + 25 * i, 470, ALLEGRO_ALIGN_LEFT, "W");
                    playerName[i] = 'W';
                    i++;
                }
                break;
            case ALLEGRO_KEY_X:
                if (ALLEGRO_EVENT_KEY_UP == ev.type) {
                    al_draw_text(font1, BLACK, 125 + 25 * i, 470, ALLEGRO_ALIGN_LEFT, "X");
                    playerName[i] = 'X';
                    i++;
                }
                break;
            case ALLEGRO_KEY_Y:
                if (ALLEGRO_EVENT_KEY_UP == ev.type) {
                    al_draw_text(font1, BLACK, 125 + 25 * i, 470, ALLEGRO_ALIGN_LEFT, "Y");
                    playerName[i] = 'Y';
                    i++;
                }
                break;
            case ALLEGRO_KEY_Z:
                if (ALLEGRO_EVENT_KEY_UP == ev.type) {
                    al_draw_text(font1, BLACK, 125 + 25 * i, 470, ALLEGRO_ALIGN_LEFT, "Z");
                    playerName[i] = 'Z';
                    i++;
                }
                break;
            case ALLEGRO_KEY_SPACE:
                if (ALLEGRO_EVENT_KEY_UP == ev.type) {
                    al_draw_text(font1, BLACK, 125 + 25 * i, 470, ALLEGRO_ALIGN_LEFT, " ");
                    playerName[i] = ' ';
                    i++;
                }
                break;
            case ALLEGRO_KEY_BACKSPACE:
                //Overlaps the last letter with the background color and moves the cursor one letter behind
                if (ALLEGRO_EVENT_KEY_UP == ev.type) {
                    if (i > 0) {
                        i--;
                        playerName[i] = ' ';
                        al_draw_bitmap(backSpace, 125 + 25 * i, 465, 0);
                    }
                }
                break;
            case ALLEGRO_KEY_ENTER:
                //Breaks out of function
                if (ALLEGRO_EVENT_KEY_UP == ev.type) {
                    game = false;
                }
                break;
            }
            al_flip_display();
            //ev.keyboard.keycode set to a key outside of the keyboard so that the program doesn't consider one key for several input; Essentially resetting the value to default
            ev.keyboard.keycode = ALLEGRO_KEY_BUTTON_L1;
        }
        else {
            //goes to this loop if max number of letters reached
            game = false;
        }
    }
}

void drawPokemons(char playerName[]) {

    //Declares and initializes all the images of Pokemon
    ALLEGRO_BITMAP* alakazam = nullptr;
    ALLEGRO_BITMAP* articuno = nullptr;
    ALLEGRO_BITMAP* blastoise = nullptr;
    ALLEGRO_BITMAP* charizard = nullptr;
    ALLEGRO_BITMAP* gengar = nullptr;
    ALLEGRO_BITMAP* machamp = nullptr;
    ALLEGRO_BITMAP* moltres = nullptr;
    ALLEGRO_BITMAP* onix = nullptr;
    ALLEGRO_BITMAP* venusaur = nullptr;
    ALLEGRO_BITMAP* zapdos = nullptr;

    //Draws all the Pokemon
    alakazam = al_load_bitmap("alakazam.png");
    al_draw_bitmap(alakazam, 50, 0, 0);

    articuno = al_load_bitmap("articuno.png");
    al_draw_bitmap(articuno, 50, 130, 0);

    blastoise = al_load_bitmap("blastoise.png");
    al_draw_bitmap(blastoise, 220, 0, 0);

    charizard = al_load_bitmap("charizard.png");
    al_draw_bitmap(charizard, 220, 130, 0);

    gengar = al_load_bitmap("gengar.png");
    al_draw_bitmap(gengar, 390, 0, 0);

    machamp = al_load_bitmap("machamp.png");
    al_draw_bitmap(machamp, 390, 130, 0);

    moltres = al_load_bitmap("moltres.png");
    al_draw_bitmap(moltres, 560, 0, 0);

    onix = al_load_bitmap("onix.png");
    al_draw_bitmap(onix, 560, 130, 0);

    venusaur = al_load_bitmap("venusaur.png");
    al_draw_bitmap(venusaur, 730, 0, 0);

    zapdos = al_load_bitmap("zapdos.png");
    al_draw_bitmap(zapdos, 730, 130, 0);

    //Draws text
    al_draw_textf(font1, BLACK, 75, 435, ALLEGRO_ALIGN_LEFT, "%s, pick your Pokemons!", playerName);

    al_flip_display();
}

void pickPokemons(char pokeName[], char playerName[]) {

    //Function used to move the square around and determine which Pokemon the player wants.
    int x = 0;
    int y = 0;

    //every time the square moves, the background and Pokemon are redrawn
    refreshScreen1();
    drawPokemons(playerName);
    al_draw_rectangle(50, 0, 190, 135, BLACK, 10);
    al_flip_display();

    game = true;
    while (game == true) {
        //ev.keyboard.keycode set to a key outside of the keyboard so that the program doesn't consider one key for several input; Essentially resetting the value to default
        ev.keyboard.keycode = ALLEGRO_KEY_BUTTON_L1;

        al_wait_for_event(event_queue, &ev);
        //Switch statement for where the square is drawn if the player presses the arrow keys. Also tracks the x and y coordinates of the square to find out which pokemon was picked
        switch (ev.keyboard.keycode) {
        case ALLEGRO_KEY_UP:
            if (ALLEGRO_EVENT_KEY_UP == ev.type) {
                if (y > 0) {
                    refreshScreen1();
                    drawPokemons(playerName);
                    y--;
                    al_draw_rectangle(50 + x * 170, 5 + y * 140, 190 + x * 170, 135 + y * 140, BLACK, 10);
                    al_flip_display();
                }
            }
            break;
        case ALLEGRO_KEY_DOWN:
            if (ALLEGRO_EVENT_KEY_UP == ev.type) {
                if (y < 1) {
                    refreshScreen1();
                    drawPokemons(playerName);
                    y++;
                    al_draw_rectangle(50 + x * 170, 5 + y * 140, 190 + x * 170, 135 + y * 140, BLACK, 10);
                    al_flip_display();
                }
            }
            break;
        case ALLEGRO_KEY_LEFT:
            if (ALLEGRO_EVENT_KEY_UP == ev.type) {
                if (x > 0) {
                    refreshScreen1();
                    drawPokemons(playerName);
                    x--;
                    al_draw_rectangle(50 + x * 170, 5 + y * 140, 190 + x * 170, 135 + y * 140, BLACK, 10);
                    al_flip_display();
                }
            }
            break;
        case ALLEGRO_KEY_RIGHT:
            if (ALLEGRO_EVENT_KEY_UP == ev.type) {
                if (x < 4) {
                    refreshScreen1();
                    drawPokemons(playerName);
                    x++;
                    al_draw_rectangle(50 + x * 170, 5 + y * 140, 190 + x * 170, 135 + y * 140, BLACK, 10);
                    al_flip_display();
                }
            }
            break;
        case ALLEGRO_KEY_ENTER:
            //Another switch statement for the program to figure out which Pokemon the user picked and sets that name into the variable
            if (ALLEGRO_EVENT_KEY_UP == ev.type) {
                switch (y) {
                case 0:
                    switch (x) {
                    case 0:
                        strcpy(pokeName, "Alakazam");
                        break;
                    case 1:
                        strcpy(pokeName, "Blastoise");
                        break;
                    case 2:
                        strcpy(pokeName, "Gengar");
                        break;
                    case 3:
                        strcpy(pokeName, "Moltres");
                        break;
                    case 4:
                        strcpy(pokeName, "Venusaur");
                        break;
                    }
                    break;
                case 1:
                    switch (x) {
                    case 0:
                        strcpy(pokeName, "Articuno");
                        break;
                    case 1:
                        strcpy(pokeName, "Charizard");
                        break;
                    case 2:
                        strcpy(pokeName, "Machamp");
                        break;
                    case 3:
                        strcpy(pokeName, "Onix");
                        break;
                    case 4:
                        strcpy(pokeName, "Zapdos");
                        break;
                    }
                    break;
                }
                game = false;
            }
        }
    }
}

void readAbilities(char pokemon[], char ability1[], char ability2[], char ability3[], char type[]) {

    //Function to read in the abilities and what type of Pokemon it is from a text file
    char junk[30];

    FILE* fptr;
    fptr = fopen("Abilities.txt", "r");

    //Used a very inefficient way of reading the text file. Sorry
    if (strcmp(pokemon, "Alakazam") == 0) {
        fscanf(fptr, "%s %s %s %s", type, ability1, ability2, ability3);
    }

    if (strcmp(pokemon, "Blastoise") == 0) {
        fscanf(fptr, "%s %s %s %s", junk, junk, junk, junk);
        fscanf(fptr, "%s %s %s %s", type, ability1, ability2, ability3);
    }

    if (strcmp(pokemon, "Gengar") == 0) {
        fscanf(fptr, "%s %s %s %s", junk, junk, junk, junk);
        fscanf(fptr, "%s %s %s %s", junk, junk, junk, junk);
        fscanf(fptr, "%s %s %s %s", type, ability1, ability2, ability3);
    }

    if (strcmp(pokemon, "Moltres") == 0) {
        fscanf(fptr, "%s %s %s %s", junk, junk, junk, junk);
        fscanf(fptr, "%s %s %s %s", junk, junk, junk, junk);
        fscanf(fptr, "%s %s %s %s", junk, junk, junk, junk);
        fscanf(fptr, "%s %s %s %s", type, ability1, ability2, ability3);
    }

    if (strcmp(pokemon, "Venusaur") == 0) {
        fscanf(fptr, "%s %s %s %s", junk, junk, junk, junk);
        fscanf(fptr, "%s %s %s %s", junk, junk, junk, junk);
        fscanf(fptr, "%s %s %s %s", junk, junk, junk, junk);
        fscanf(fptr, "%s %s %s %s", junk, junk, junk, junk);
        fscanf(fptr, "%s %s %s %s", type, ability1, ability2, ability3);
    }

    if (strcmp(pokemon, "Articuno") == 0) {
        fscanf(fptr, "%s %s %s %s", junk, junk, junk, junk);
        fscanf(fptr, "%s %s %s %s", junk, junk, junk, junk);
        fscanf(fptr, "%s %s %s %s", junk, junk, junk, junk);
        fscanf(fptr, "%s %s %s %s", junk, junk, junk, junk);
        fscanf(fptr, "%s %s %s %s", junk, junk, junk, junk);
        fscanf(fptr, "%s %s %s %s", type, ability1, ability2, ability3);
    }

    if (strcmp(pokemon, "Charizard") == 0) {
        fscanf(fptr, "%s %s %s %s", junk, junk, junk, junk);
        fscanf(fptr, "%s %s %s %s", junk, junk, junk, junk);
        fscanf(fptr, "%s %s %s %s", junk, junk, junk, junk);
        fscanf(fptr, "%s %s %s %s", junk, junk, junk, junk);
        fscanf(fptr, "%s %s %s %s", junk, junk, junk, junk);
        fscanf(fptr, "%s %s %s %s", junk, junk, junk, junk);
        fscanf(fptr, "%s %s %s %s", type, ability1, ability2, ability3);
    }

    if (strcmp(pokemon, "Machamp") == 0) {
        fscanf(fptr, "%s %s %s %s", junk, junk, junk, junk);
        fscanf(fptr, "%s %s %s %s", junk, junk, junk, junk);
        fscanf(fptr, "%s %s %s %s", junk, junk, junk, junk);
        fscanf(fptr, "%s %s %s %s", junk, junk, junk, junk);
        fscanf(fptr, "%s %s %s %s", junk, junk, junk, junk);
        fscanf(fptr, "%s %s %s %s", junk, junk, junk, junk);
        fscanf(fptr, "%s %s %s %s", junk, junk, junk, junk);
        fscanf(fptr, "%s %s %s %s", type, ability1, ability2, ability3);
    }

    if (strcmp(pokemon, "Onix") == 0) {
        fscanf(fptr, "%s %s %s %s", junk, junk, junk, junk);
        fscanf(fptr, "%s %s %s %s", junk, junk, junk, junk);
        fscanf(fptr, "%s %s %s %s", junk, junk, junk, junk);
        fscanf(fptr, "%s %s %s %s", junk, junk, junk, junk);
        fscanf(fptr, "%s %s %s %s", junk, junk, junk, junk);
        fscanf(fptr, "%s %s %s %s", junk, junk, junk, junk);
        fscanf(fptr, "%s %s %s %s", junk, junk, junk, junk);
        fscanf(fptr, "%s %s %s %s", junk, junk, junk, junk);
        fscanf(fptr, "%s %s %s %s", type, ability1, ability2, ability3);
    }

    if (strcmp(pokemon, "Zapdos") == 0) {
        fscanf(fptr, "%s %s %s %s", junk, junk, junk, junk);
        fscanf(fptr, "%s %s %s %s", junk, junk, junk, junk);
        fscanf(fptr, "%s %s %s %s", junk, junk, junk, junk);
        fscanf(fptr, "%s %s %s %s", junk, junk, junk, junk);
        fscanf(fptr, "%s %s %s %s", junk, junk, junk, junk);
        fscanf(fptr, "%s %s %s %s", junk, junk, junk, junk);
        fscanf(fptr, "%s %s %s %s", junk, junk, junk, junk);
        fscanf(fptr, "%s %s %s %s", junk, junk, junk, junk);
        fscanf(fptr, "%s %s %s %s", junk, junk, junk, junk);
        fscanf(fptr, "%s %s %s %s", type, ability1, ability2, ability3);
    }
    fclose(fptr);
}

void initializePokemons(Pokemons& pokemon) {

    //Initialize the default Pokemon structure
    pokemon.ability3.damage = 20;
    pokemon.ability1.damage = 50;
    pokemon.health = 100;
    pokemon.ability1.counter = 1;
    pokemon.ability2.counter = 3;
}

void drawPokeBattle(char pokemon1[], int x1, int y1, char pokemon2[], int x2, int y2, char word1[], char word2[], char word3[], int& health1, int& health2, int counter1, int counter2, char player1Name[], char player2Name[], char poke1Type[], char poke2Type[]) {

    //Function used to redraw the entire battle scene when the box is moved to minimize code
    //Figures out which the aggressive and the defensive Pokemon are and draws them accordingly
    if (strcmp(pokemon1, "Alakazam") == 0) {
        ALLEGRO_BITMAP* alakazam = nullptr;
        alakazam = al_load_bitmap("alakazam.png");
        al_draw_bitmap(alakazam, x1, y1, 0);
    }

    if (strcmp(pokemon1, "Articuno") == 0) {
        ALLEGRO_BITMAP* articuno = nullptr;
        articuno = al_load_bitmap("articuno.png");
        al_draw_bitmap(articuno, x1, y1, 0);
    }

    if (strcmp(pokemon1, "Blastoise") == 0) {
        ALLEGRO_BITMAP* blastoise = nullptr;
        blastoise = al_load_bitmap("blastoise.png");
        al_draw_bitmap(blastoise, x1, y1, 0);
    }

    if (strcmp(pokemon1, "Charizard") == 0) {
        ALLEGRO_BITMAP* charizard = nullptr;
        charizard = al_load_bitmap("charizard.png");
        al_draw_bitmap(charizard, x1, y1, 0);
    }

    if (strcmp(pokemon1, "Gengar") == 0) {
        ALLEGRO_BITMAP* gengar = nullptr;
        gengar = al_load_bitmap("gengar.png");
        al_draw_bitmap(gengar, x1, y1, 0);
    }

    if (strcmp(pokemon1, "Machamp") == 0) {
        ALLEGRO_BITMAP* machamp = nullptr;
        machamp = al_load_bitmap("machamp.png");
        al_draw_bitmap(machamp, x1, y1, 0);
    }

    if (strcmp(pokemon1, "Moltres") == 0) {
        ALLEGRO_BITMAP* moltres = nullptr;
        moltres = al_load_bitmap("moltres.png");
        al_draw_bitmap(moltres, x1, y1, 0);
    }

    if (strcmp(pokemon1, "Onix") == 0) {
        ALLEGRO_BITMAP* onix = nullptr;
        onix = al_load_bitmap("onix.png");
        al_draw_bitmap(onix, x1, y1, 0);
    }

    if (strcmp(pokemon1, "Venusaur") == 0) {
        ALLEGRO_BITMAP* venusaur = nullptr;
        venusaur = al_load_bitmap("venusaur.png");
        al_draw_bitmap(venusaur, x1, y1, 0);
    }

    if (strcmp(pokemon1, "Zapdos") == 0) {
        ALLEGRO_BITMAP* zapdos = nullptr;
        zapdos = al_load_bitmap("zapdos.png");
        al_draw_bitmap(zapdos, x1, y1, 0);
    }

    if (strcmp(pokemon2, "Alakazam") == 0) {
        ALLEGRO_BITMAP* alakazam = nullptr;
        alakazam = al_load_bitmap("alakazam.png");
        al_draw_bitmap(alakazam, x2, y2, 0);
    }

    if (strcmp(pokemon2, "Articuno") == 0) {
        ALLEGRO_BITMAP* articuno = nullptr;
        articuno = al_load_bitmap("articuno.png");
        al_draw_bitmap(articuno, x2, y2, 0);
    }

    if (strcmp(pokemon2, "Blastoise") == 0) {
        ALLEGRO_BITMAP* blastoise = nullptr;
        blastoise = al_load_bitmap("blastoise.png");
        al_draw_bitmap(blastoise, x2, y2, 0);
    }

    if (strcmp(pokemon2, "Charizard") == 0) {
        ALLEGRO_BITMAP* charizard = nullptr;
        charizard = al_load_bitmap("charizard.png");
        al_draw_bitmap(charizard, x2, y2, 0);
    }

    if (strcmp(pokemon2, "Gengar") == 0) {
        ALLEGRO_BITMAP* gengar = nullptr;
        gengar = al_load_bitmap("gengar.png");
        al_draw_bitmap(gengar, x2, y2, 0);
    }

    if (strcmp(pokemon2, "Machamp") == 0) {
        ALLEGRO_BITMAP* machamp = nullptr;
        machamp = al_load_bitmap("machamp.png");
        al_draw_bitmap(machamp, x2, y2, 0);
    }

    if (strcmp(pokemon2, "Moltres") == 0) {
        ALLEGRO_BITMAP* moltres = nullptr;
        moltres = al_load_bitmap("moltres.png");
        al_draw_bitmap(moltres, x2, y2, 0);
    }

    if (strcmp(pokemon2, "Onix") == 0) {
        ALLEGRO_BITMAP* onix = nullptr;
        onix = al_load_bitmap("onix.png");
        al_draw_bitmap(onix, x2, y2, 0);
    }

    if (strcmp(pokemon2, "Venusaur") == 0) {
        ALLEGRO_BITMAP* venusaur = nullptr;
        venusaur = al_load_bitmap("venusaur.png");
        al_draw_bitmap(venusaur, x2, y2, 0);
    }

    if (strcmp(pokemon2, "Zapdos") == 0) {
        ALLEGRO_BITMAP* zapdos = nullptr;
        zapdos = al_load_bitmap("zapdos.png");
        al_draw_bitmap(zapdos, x2, y2, 0);
    }

    //Draws the 3 texts on the bottom part of the battle scene
    al_draw_textf(font2, BLACK, 480, 420, ALLEGRO_ALIGN_CENTRE, "%s", word1);
    al_draw_textf(font2, BLACK, 250, 505, ALLEGRO_ALIGN_CENTRE, "%s", word2);
    al_draw_textf(font2, BLACK, 725, 505, ALLEGRO_ALIGN_CENTRE, "%s", word3);

    //Draws the missing health if the Pokemon took damage from previous rounds
    drawHealth(health1, health2);

    //Draws the counter for the abilities
    //The loop is there because I recycled this function and the counter is not needed in every scene
    if (counter1 < 10 && counter2 < 10) {
        al_draw_textf(font1, BLACK, 930, 410, ALLEGRO_ALIGN_CENTRE, "%d", counter1);
        al_draw_textf(font1, BLACK, 930, 495, ALLEGRO_ALIGN_CENTRE, "%d", counter2);
    }

    //Draws the names of the players during battle
    al_draw_textf(font2, BLACK, 570, 278, ALLEGRO_ALIGN_LEFT, "%s", player1Name);
    al_draw_textf(font2, BLACK, 70, 75, ALLEGRO_ALIGN_LEFT, "%s", player2Name);

    //Draws the type of Pokemon currently in battle
    if (strcmp(poke1Type, "Fire") == 0) {
        al_draw_text(font1, RED, 570, 343, ALLEGRO_ALIGN_LEFT, "Fire");
    }

    if (strcmp(poke1Type, "Water") == 0) {
        al_draw_text(font1, BLUE, 570, 343, ALLEGRO_ALIGN_LEFT, "Water");
    }

    if (strcmp(poke1Type, "Psychic") == 0) {
        al_draw_text(font1, PURPLE, 570, 343, ALLEGRO_ALIGN_LEFT, "Psychic");
    }

    if (strcmp(poke1Type, "Rock") == 0) {
        al_draw_text(font1, BROWN, 570, 343, ALLEGRO_ALIGN_LEFT, "Rock");
    }

    if (strcmp(poke1Type, "Leaf") == 0) {
        al_draw_text(font1, GREEN, 570, 343, ALLEGRO_ALIGN_LEFT, "Leaf");
    }

    if (strcmp(poke2Type, "Fire") == 0) {
        al_draw_text(font1, RED, 70, 140, ALLEGRO_ALIGN_LEFT, "Fire");
    }

    if (strcmp(poke2Type, "Water") == 0) {
        al_draw_text(font1, BLUE, 70, 140, ALLEGRO_ALIGN_LEFT, "Water");
    }

    if (strcmp(poke2Type, "Psychic") == 0) {
        al_draw_text(font1, PURPLE, 70, 140, ALLEGRO_ALIGN_LEFT, "Psychic");
    }

    if (strcmp(poke2Type, "Rock") == 0) {
        al_draw_text(font1, BROWN, 70, 140, ALLEGRO_ALIGN_LEFT, "Rock");
    }

    if (strcmp(poke2Type, "Leaf") == 0) {
        al_draw_text(font1, GREEN, 70, 140, ALLEGRO_ALIGN_LEFT, "Leaf");
    }

    al_flip_display();
}

void userBattle(Pokemons& pokemon1, Pokemons& pokemon2, int x1, int x2, int y1, int y2, char player1Name[], char player2Name[], char attackerType[], char defenderType[], int beforeHealth1, int beforeHealth2, int& damage1, char& damage2) {

    //Function which inevitably leads to fight as I did not have time to add other functions like Switching Pokemon and forfeiting the battle
    int y = 0;
    int x = 0;
    game = true;

    al_draw_rectangle(10, 395, 950, 468, BLACK, 10);
    al_flip_display();

    //Stays in loop until enter is pressed
    while (game == true) {
        //ev.keyboard.keycode set to a key outside of the keyboard so that the program doesn't consider one key for several input; Essentially resetting the value to default
        ev.keyboard.keycode = ALLEGRO_KEY_BUTTON_L1;
        char* fight = const_cast<char*>("Fight");

        al_wait_for_event(event_queue, &ev);
        //Switch statement with only one output: Fight. Moves the box around
        switch (ev.keyboard.keycode) {
        case ALLEGRO_KEY_UP:
            if (ALLEGRO_EVENT_KEY_UP == ev.type) {
                if (y > 0) {
                    refreshScreen2();
                    drawPokeBattle(pokemon1.pokeName, x1, y1, pokemon2.pokeName, x2, y2, fight, fight, fight, beforeHealth1, beforeHealth2, 500, 500, player1Name, player2Name, attackerType, defenderType);
                    y = 0;
                    al_draw_rectangle(10, 395, 950, 468, BLACK, 10);
                    al_flip_display();
                }
            }
            break;
        case ALLEGRO_KEY_DOWN:
            if (ALLEGRO_EVENT_KEY_UP == ev.type) {
                if (y < 1) {
                    refreshScreen2();
                    drawPokeBattle(pokemon1.pokeName, x1, y1, pokemon2.pokeName, x2, y2, fight, fight, fight, beforeHealth1, beforeHealth2, 500, 500, player1Name, player2Name, attackerType, defenderType);
                    al_draw_rectangle(10, 480, 470, 551, BLACK, 10);
                    al_flip_display();
                    y = 1;
                    x = 0;
                }
            }
            break;
        case ALLEGRO_KEY_LEFT:
            if (ALLEGRO_EVENT_KEY_UP == ev.type) {
                if (y > 0 && x > 0) {
                    refreshScreen2();
                    drawPokeBattle(pokemon1.pokeName, x1, y1, pokemon2.pokeName, x2, y2, fight, fight, fight, beforeHealth1, beforeHealth2, 500, 500, player1Name, player2Name, attackerType, defenderType);
                    al_draw_rectangle(10, 480, 470, 551, BLACK, 10);
                    al_flip_display();
                    x = 0;
                }
            }
            break;
        case ALLEGRO_KEY_RIGHT:
            if (ALLEGRO_EVENT_KEY_UP == ev.type) {
                if (y > 0 && x < 1) {
                    refreshScreen2();
                    drawPokeBattle(pokemon1.pokeName, x1, y1, pokemon2.pokeName, x2, y2, fight, fight, fight, beforeHealth1, beforeHealth2, 500, 500, player1Name, player2Name, attackerType, defenderType);
                    al_draw_rectangle(478, 480, 950, 551, BLACK, 10);
                    al_flip_display();
                    x = 1;
                }
            }
            break;
        case ALLEGRO_KEY_ENTER:
            //I still left these lines of codes which determines where the square's coordinates are even though they all lead to the same output. This is just in case I want to continue and improve my codes in the future
            if (ALLEGRO_EVENT_KEY_UP == ev.type) {
                switch (y) {
                case 0:
                    drawAttack(pokemon1, pokemon2, x1, x2, y1, y2, player1Name, player2Name, attackerType, defenderType, beforeHealth1, beforeHealth2, damage1, damage2);
                    break;
                case 1:
                    switch (x) {
                    case 0:
                        drawAttack(pokemon1, pokemon2, x1, x2, y1, y2, player1Name, player2Name, attackerType, defenderType, beforeHealth1, beforeHealth2, damage1, damage2);
                        break;
                    case 1:
                        drawAttack(pokemon1, pokemon2, x1, x2, y1, y2, player1Name, player2Name, attackerType, defenderType, beforeHealth1, beforeHealth2, damage1, damage2);
                        break;
                    }
                    break;
                }
            }
        }
    }
}

void drawAttack(Pokemons& pokemon1, Pokemons& pokemon2, int x1, int x2, int y1, int y2, char player1Name[], char player2Name[], char attackerType[], char defenderType[], int beforeHealth1, int beforeHealth2, int& damage1, char& damage2) {

    //Function which determines where the square is situated and does the output accordingly
    //Very similar function to userBattle so look at that function for comments on what the things do. The only difference are the different outputs
    int y = 0;
    int x = 0;
    damage1 = 0;
    damage2 = 'Z';
    game = true;

    refreshScreen2();
    drawPokeBattle(pokemon1.pokeName, x1, y1, pokemon2.pokeName, x2, y2, pokemon1.ability1.abilityName, pokemon1.ability2.abilityName, pokemon1.ability3.abilityName, beforeHealth1, beforeHealth2, pokemon1.ability1.counter, pokemon1.ability2.counter, player1Name, player2Name, attackerType, defenderType);
    al_draw_rectangle(10, 395, 950, 468, BLACK, 10);
    al_flip_display();

    while (game == true) {
        ev.keyboard.keycode = ALLEGRO_KEY_BUTTON_L1;

        al_wait_for_event(event_queue, &ev);
        switch (ev.keyboard.keycode) {
        case ALLEGRO_KEY_UP:
            if (ALLEGRO_EVENT_KEY_UP == ev.type) {
                if (y > 0) {
                    refreshScreen2();
                    drawPokeBattle(pokemon1.pokeName, x1, y1, pokemon2.pokeName, x2, y2, pokemon1.ability1.abilityName, pokemon1.ability2.abilityName, pokemon1.ability3.abilityName, beforeHealth1, beforeHealth2, pokemon1.ability1.counter, pokemon1.ability2.counter, player1Name, player2Name, attackerType, defenderType);
                    al_draw_rectangle(10, 395, 950, 468, BLACK, 10);
                    al_flip_display();
                    y = 0;
                }
            }
            break;
        case ALLEGRO_KEY_DOWN:
            if (ALLEGRO_EVENT_KEY_UP == ev.type) {
                if (y < 1) {
                    refreshScreen2();
                    drawPokeBattle(pokemon1.pokeName, x1, y1, pokemon2.pokeName, x2, y2, pokemon1.ability1.abilityName, pokemon1.ability2.abilityName, pokemon1.ability3.abilityName, beforeHealth1, beforeHealth2, pokemon1.ability1.counter, pokemon1.ability2.counter, player1Name, player2Name, attackerType, defenderType);
                    al_draw_rectangle(10, 480, 470, 551, BLACK, 10);
                    al_flip_display();
                    x = 0;
                    y = 1;
                }
            }
            break;
        case ALLEGRO_KEY_LEFT:
            if (ALLEGRO_EVENT_KEY_UP == ev.type) {
                if (y > 0 && x > 0) {
                    refreshScreen2();
                    drawPokeBattle(pokemon1.pokeName, x1, y1, pokemon2.pokeName, x2, y2, pokemon1.ability1.abilityName, pokemon1.ability2.abilityName, pokemon1.ability3.abilityName, beforeHealth1, beforeHealth2, pokemon1.ability1.counter, pokemon1.ability2.counter, player1Name, player2Name, attackerType, defenderType);
                    al_draw_rectangle(10, 480, 470, 551, BLACK, 10);
                    al_flip_display();
                    x = 0;
                }
            }
            break;
        case ALLEGRO_KEY_RIGHT:
            if (ALLEGRO_EVENT_KEY_UP == ev.type) {
                if (y > 0 && x < 1) {
                    refreshScreen2();
                    drawPokeBattle(pokemon1.pokeName, x1, y1, pokemon2.pokeName, x2, y2, pokemon1.ability1.abilityName, pokemon1.ability2.abilityName, pokemon1.ability3.abilityName, beforeHealth1, beforeHealth2, pokemon1.ability1.counter, pokemon1.ability2.counter, player1Name, player2Name, attackerType, defenderType);
                    al_draw_rectangle(478, 480, 950, 551, BLACK, 10);

                    al_flip_display();
                    x = 1;
                }
            }
            break;
        case ALLEGRO_KEY_ENTER:
            if (ALLEGRO_EVENT_KEY_UP == ev.type) {
                //Switch statement which determines where the Square's coordinates are located and does the outputs accordingly
                switch (y) {
                case 0:

                    if (pokemon1.ability1.counter > 0) {
                        damage1 = attackDamage(pokemon2.health, pokemon1.ability1.damage, attackerType, defenderType);
                        pokemon1.ability1.counter--;
                        game = false;
                    }
                    break;
                case 1:
                    switch (x) {
                    case 0:
                        damage1 = attackDamage(pokemon2.health, pokemon1.ability3.damage, attackerType, defenderType);
                        game = false;
                        break;
                    case 1:
                        if (pokemon1.ability2.counter > 0) {
                            damage2 = attack2Damage(pokemon1.ability3.abilityName);
                            pokemon1.ability2.counter--;
                            game = false;
                        }
                        break;
                    }
                    break;
                }
            }
        }
    }
}

int attackDamage(int& health, int damage, char attackerType[], char defenderType[]) {

    //Function for the normal attacks. (Basic attack with unlimited counters or the Ultimate attack with only 1 charge)
    //Also calculates super effective and stuff
    if (strcmp(attackerType, "Water") == 0 && strcmp(defenderType, "Fire") == 0) {
        damage = damage * 2;
    }

    else if (strcmp(attackerType, "Fire") == 0 && strcmp(defenderType, "Leaf") == 0) {
        damage = damage * 2;
    }

    else if (strcmp(attackerType, "Leaf") == 0 && strcmp(defenderType, "Psychic") == 0) {
        damage = damage * 2;
    }

    else if (strcmp(attackerType, "Psychic") == 0 && strcmp(defenderType, "Rock") == 0) {
        damage = damage * 2;
    }

    else if (strcmp(attackerType, "Rock") == 0 && strcmp(defenderType, "Water") == 0) {
        damage = damage * 2;
    }

    else if (strcmp(attackerType, "Fire") == 0 && strcmp(defenderType, "Water") == 0) {
        damage = damage / 2;
    }

    else if (strcmp(attackerType, "Leaf") == 0 && strcmp(defenderType, "Fire") == 0) {
        damage = damage / 2;
    }

    else if (strcmp(attackerType, "Psychic") == 0 && strcmp(defenderType, "Leaf") == 0) {
        damage = damage / 2;
    }

    else if (strcmp(attackerType, "Rock") == 0 && strcmp(defenderType, "Psychic") == 0) {
        damage = damage / 2;
    }

    else if (strcmp(attackerType, "Water") == 0 && strcmp(defenderType, "Rock") == 0) {
        damage = damage / 2;
    }

    else {
        damage = damage;
    }

    return damage;
}

char attack2Damage(char ability[]) {
    //Determines which type of Special attack the Pokemon has and returns it.

    if (strcmp(ability, "Confusion") == 0 || strcmp(ability, "Revenge") == 0 || strcmp(ability, "Hypnosis") == 0) {
        return 'A';
    }

    if (strcmp(ability, "Protect") == 0 || strcmp(ability, "Detect") == 0 || strcmp(ability, "Endure") == 0 || strcmp(ability, "Sandstorm") == 0) {
        return 'B';
    }

    if (strcmp(ability, "Synthesis") == 0) {
        return 'C';
    }

    if (strcmp(ability, "Dragon-Rage") == 0) {
        return 'D';
    }

    if (strcmp(ability, "Reflect") == 0) {
        return 'E';
    }
}

void drawHealth(int& health1, int& health2) {
    //Draws the reduction of health
    //Sets the health to 0 so that the bar isn't too long

    //player1.pokemon.health = health1
    ALLEGRO_BITMAP* missingHealth = nullptr;
    missingHealth = al_load_bitmap("Health.png");

    if (health1 < 0) {
        health1 = 0;
    }

    if (health2 < 0) {
        health2 = 0;
    }

    for (int i = 0; i < (100 - health2) / 10; i++) {
        al_draw_bitmap(missingHealth, 390 - i * 20, 111, 0);
        al_flip_display();
    }

    for (int i = 0; i < (100 - health1) / 10; i++) {
        al_draw_bitmap(missingHealth, 887 - i * 20, 313, 0);
        al_flip_display();
    }
}

void damageDone(int& player1Health, int& player2Health, int player1Damage, char player1Special, int player2Damage, char player2Special, bool& dragonRage1, bool& dragonRage2) {
    //Checks the two types of done by each player and checks if they counter each other.
    //Example: If player 1 uses an attack and player2 uses reflect, player 2 takes not damage and player 1's attack is reflected back to him
    //Second player 2's attack is only applied if player 1's attack is not fatal

    //Checks whether either players set the Dragon Rage move which doubles the next damage.
    if (dragonRage1 == true) {
        player1Damage = player1Damage * 2;
    }
    if (dragonRage2 == true) {
        player2Damage = player2Damage * 2;
    }

    if (player1Damage > 0) {
        if (player2Damage > 0) {
            //Output if both players used damage attacks
            player2Health = player2Health - player1Damage;
            player1Health = player1Health - player2Damage;

            //Sets the Dragon Rage value to false so that all of Charizard's attacks are not double damaged.
            dragonRage1 = false;
            dragonRage2 = false;

            //Player 2 can only use his attack if player 1's attack does not kill him
            if (player2Health <= 0) {
                player1Health = player1Health + player2Damage;
            }
        }
        switch (player2Special) {
        case 'A':
            //Both player gets attacked by Player1's attack
            player2Damage = player1Damage;
            player2Health = player2Health - player1Damage;
            player1Health = player1Health - player2Damage;
            //Player 2 can only use his attack if player 1's attack does not kill him
            if (player2Health <= 0) {
                player1Health = player1Health + player2Damage;
            }
            dragonRage1 = false;
            dragonRage2 = false;
            break;
        case 'B':
            //Player1's attack misses
            player2Health = player2Health - player1Damage;
            //Player 2 can only use his attack if player 1's attack does not kill him
            if (player2Health <= 0) {
                player2Health = player2Health - player1Damage;
            }
            player2Health = player2Health + player1Damage;
            dragonRage1 = false;
            dragonRage2 = false;
            break;
        case 'C':
            //Player2 heals himself
            player2Health = player2Health - player1Damage;
            player2Health = player2Health + 33;
            dragonRage1 = false;
            dragonRage2 = false;
            break;
        case 'D':
            //Dragon rage is activated
            player2Health = player2Health - player1Damage;
            dragonRage1 = false;
            dragonRage2 = true;
            //Player 2 can only use his attack if player 1's attack does not kill him
            if (player2Health <= 0) {
                dragonRage2 = false;
            }
            break;
        case 'E':
            //Player 1's damage is reflected back
            player2Damage = player1Damage;
            player1Health = player1Health - player2Damage;
            dragonRage1 = false;
            dragonRage2 = false;
            break;
        }
    }

    //Similar comments to the above switch loop
    else if (player1Damage == 0) {
        switch (player1Special) {
        case 'A':
            if (player2Damage > 0) {
                player1Damage = player2Damage;
                player2Health = player2Health - player1Damage;
                //Player 2 can only use his attack if player 1's attack does not kill him
                if (player2Health <= 0) {
                    dragonRage1 = false;
                    dragonRage2 = false;
                    break;
                }
                else {
                    player1Health = player1Health - player2Damage;
                    dragonRage2 = false;
                    break;
                }
            }
            switch (player2Special) {
            case 'A':
                break;
            case 'B':
                break;
            case 'C':
                player2Health = player2Health + 33;
                break;
            case 'D':
                dragonRage2 = true;
            case 'E':
                break;
            }
            break;
        case 'B':
            //Player 2 can only use his attack if player 1's attack does not kill him
            if (player2Damage > 0) {
                break;
            }

            switch (player2Special) {
            case 'A':
                break;
            case 'B':
                break;
            case 'C':
                player2Health = player2Health + 33;
                break;
            case 'D':
                dragonRage2 = true;
            case 'E':
                break;
            }
            break;
        case 'C':
            player1Health = player1Health + 33;

            //Player 2 can only use his attack if player 1's attack does not kill him
            if (player2Damage > 0) {
                player1Health = player1Health - player2Damage;
            }

            switch (player2Special) {
            case 'A':
                break;
            case 'B':
                break;
            case 'C':
                player2Health = player2Health + 33;
                break;
            case 'D':
                dragonRage2 = true;
            case 'E':
                break;
            }
            break;
        case 'D':
            dragonRage1 = true;

            if (player2Damage > 0) {
                player1Health = player1Health - player2Damage;

                //Player 2 can only use his attack if player 1's attack does not kill him
                if (player1Health <= 0) {
                    dragonRage1 = false;
                }
            }
            switch (player2Special) {
            case 'A':
                break;
            case 'B':
                break;
            case 'C':
                player2Health = player2Health + 33;
                break;
            case 'D':
                dragonRage2 = true;
            case 'E':
                break;
            }
            break;
        case 'E':
            if (player2Damage > 0) {
                player1Damage = player2Damage;
                player2Health = player2Health - player1Damage;
                break;
            }
            switch (player2Special) {
            case 'A':
                break;
            case 'B':
                break;
            case 'C':
                player2Health = player2Health + 33;
                break;
            case 'D':
                dragonRage2 = true;
            case 'E':
                break;
            }
            break;
        }
    }
}

int battleScene(Pokemons& pokemon1, Pokemons& pokemon2, char player1Name[], char player2Name[]) {
    //Loop used for the actual battle and the round mechanism
    //Loop is structured so that the battle goes like this: Player1, Player2, Player2, Player1. This is to keep the battle fair.
    char* fight = const_cast<char*>("Fight");

    while (pokemon1.health >= 0 || pokemon2.health >= 0) {

        //beforeHealth was created to make sure the updated health bar is only shown after both players have attacked
        int beforeHealth1 = pokemon1.health;
        int beforeHealth2 = pokemon2.health;
        //Quick way to sort out which damage type the player used
        int attack1Damage = 0;
        char attack1Special = 'Z';
        int attack2Damage = 1000000;
        char attack2Special = 'Z';
        //Initialized so that DragonRage from previous round doesn't work twice
        bool player1DragonRage = false;
        bool player2DragonRage = false;

        //FIRST ROUND
        //Makes sure the health bar only updates after the round ends
        beforeHealth1 = pokemon1.health;
        beforeHealth2 = pokemon2.health;

        //Draws background
        refreshScreen2();
        //BattleScene for player1
        drawPokeBattle(pokemon1.pokeName, 175, 240, pokemon2.pokeName, 625, 120, fight, fight, fight, beforeHealth1, beforeHealth2, 500, 500, player1Name, player2Name, pokemon1.pokeType, pokemon2.pokeType);
        userBattle(pokemon1, pokemon2, 175, 625, 240, 120, player1Name, player2Name, pokemon1.pokeType, pokemon2.pokeType, beforeHealth1, beforeHealth2, attack1Damage, attack1Special);

        //Draws background
        refreshScreen2();
        //BattleScene for player2
        drawPokeBattle(pokemon2.pokeName, 175, 240, pokemon1.pokeName, 625, 120, fight, fight, fight, beforeHealth2, beforeHealth1, 500, 500, player2Name, player1Name, pokemon2.pokeType, pokemon1.pokeType);
        userBattle(pokemon2, pokemon1, 175, 625, 240, 120, player2Name, player1Name, pokemon2.pokeType, pokemon1.pokeType, beforeHealth2, beforeHealth1, attack2Damage, attack2Special);
        printf("Player1 Hp: %c\n", attack1Special);
        printf("Player2 Hp: %c\n\n", attack2Special);
        //Deals damage of player 1 and 2
        damageDone(pokemon1.health, pokemon2.health, attack1Damage, attack1Special, attack2Damage, attack2Special, player1DragonRage, player2DragonRage);
        printf("Player1 Hp: %d\n", pokemon1.health);
        printf("Player2 Hp: %d\n\n", pokemon2.health);
        //Breaks the loop if either player dies
        if (pokemon1.health <= 0 || pokemon2.health <= 0) {
            return 0;
        }

        //SECOND ROUND
        //Makes sure the health bar only updates after the round ends
        beforeHealth1 = pokemon1.health;
        beforeHealth2 = pokemon2.health;

        //Draws background
        refreshScreen2();
        //BattleScene for player2
        drawPokeBattle(pokemon2.pokeName, 175, 240, pokemon1.pokeName, 625, 120, fight, fight, fight, beforeHealth2, beforeHealth1, 500, 500, player2Name, player1Name, pokemon2.pokeType, pokemon1.pokeType);
        userBattle(pokemon2, pokemon1, 175, 625, 240, 120, player2Name, player1Name, pokemon2.pokeType, pokemon1.pokeType, beforeHealth2, beforeHealth1, attack2Damage, attack2Special);

        //Draws background
        refreshScreen2();
        //BattleScene for player1
        drawPokeBattle(pokemon1.pokeName, 175, 240, pokemon2.pokeName, 625, 120, fight, fight, fight, beforeHealth1, beforeHealth2, 500, 500, player1Name, player2Name, pokemon1.pokeType, pokemon2.pokeType);
        userBattle(pokemon1, pokemon2, 175, 625, 240, 120, player1Name, player2Name, pokemon1.pokeType, pokemon2.pokeType, beforeHealth1, beforeHealth2, attack1Damage, attack1Special);
        //Deals damage of player 2 and 1
        damageDone(pokemon2.health, pokemon1.health, attack2Damage, attack2Special, attack1Damage, attack1Special, player2DragonRage, player1DragonRage);
        printf("Player1 Hp: %d\n", pokemon1.health);
        printf("Player2 Hp: %d\n\n", pokemon2.health);
        //Breaks the loop if either player dies
        if (pokemon1.health <= 0 || pokemon2.health <= 0) {
            return 0;
        }
    }
}

void winnerScene(Player player) {
    //Draws the Winner Scene

    //Draws the third background
    refreshScreen3();

    //Draws the winner's name
    al_draw_textf(font2, BLACK, 60, 130, ALLEGRO_ALIGN_LEFT, "%s", player.name);

    //All this code is to draw the three Pokemon of the winner
    if (strcmp(player.pokemon[0].pokeName, "Alakazam") == 0) {
        ALLEGRO_BITMAP* alakazam = nullptr;
        alakazam = al_load_bitmap("alakazam.png");
        al_draw_bitmap(alakazam, 50, 210, 0);
    }

    if (strcmp(player.pokemon[0].pokeName, "Articuno") == 0) {
        ALLEGRO_BITMAP* articuno = nullptr;
        articuno = al_load_bitmap("articuno.png");
        al_draw_bitmap(articuno, 50, 210, 0);
    }

    if (strcmp(player.pokemon[0].pokeName, "Blastoise") == 0) {
        ALLEGRO_BITMAP* blastoise = nullptr;
        blastoise = al_load_bitmap("blastoise.png");
        al_draw_bitmap(blastoise, 50, 210, 0);
    }

    if (strcmp(player.pokemon[0].pokeName, "Charizard") == 0) {
        ALLEGRO_BITMAP* charizard = nullptr;
        charizard = al_load_bitmap("charizard.png");
        al_draw_bitmap(charizard, 50, 210, 0);
    }

    if (strcmp(player.pokemon[0].pokeName, "Gengar") == 0) {
        ALLEGRO_BITMAP* gengar = nullptr;
        gengar = al_load_bitmap("gengar.png");
        al_draw_bitmap(gengar, 50, 210, 0);
    }

    if (strcmp(player.pokemon[0].pokeName, "Machamp") == 0) {
        ALLEGRO_BITMAP* machamp = nullptr;
        machamp = al_load_bitmap("machamp.png");
        al_draw_bitmap(machamp, 50, 210, 0);
    }

    if (strcmp(player.pokemon[0].pokeName, "Moltres") == 0) {
        ALLEGRO_BITMAP* moltres = nullptr;
        moltres = al_load_bitmap("moltres.png");
        al_draw_bitmap(moltres, 50, 210, 0);
    }

    if (strcmp(player.pokemon[0].pokeName, "Onix") == 0) {
        ALLEGRO_BITMAP* onix = nullptr;
        onix = al_load_bitmap("onix.png");
        al_draw_bitmap(onix, 50, 210, 0);
    }

    if (strcmp(player.pokemon[0].pokeName, "Venusaur") == 0) {
        ALLEGRO_BITMAP* venusaur = nullptr;
        venusaur = al_load_bitmap("venusaur.png");
        al_draw_bitmap(venusaur, 50, 210, 0);
    }

    if (strcmp(player.pokemon[0].pokeName, "Zapdos") == 0) {
        ALLEGRO_BITMAP* zapdos = nullptr;
        zapdos = al_load_bitmap("zapdos.png");
        al_draw_bitmap(zapdos, 50, 210, 0);
    }

    if (strcmp(player.pokemon[1].pokeName, "Alakazam") == 0) {
        ALLEGRO_BITMAP* alakazam = nullptr;
        alakazam = al_load_bitmap("alakazam.png");
        al_draw_bitmap(alakazam, 250, 210, 0);
    }

    if (strcmp(player.pokemon[1].pokeName, "Articuno") == 0) {
        ALLEGRO_BITMAP* articuno = nullptr;
        articuno = al_load_bitmap("articuno.png");
        al_draw_bitmap(articuno, 250, 210, 0);
    }

    if (strcmp(player.pokemon[1].pokeName, "Blastoise") == 0) {
        ALLEGRO_BITMAP* blastoise = nullptr;
        blastoise = al_load_bitmap("blastoise.png");
        al_draw_bitmap(blastoise, 250, 210, 0);
    }

    if (strcmp(player.pokemon[1].pokeName, "Charizard") == 0) {
        ALLEGRO_BITMAP* charizard = nullptr;
        charizard = al_load_bitmap("charizard.png");
        al_draw_bitmap(charizard, 250, 210, 0);
    }

    if (strcmp(player.pokemon[1].pokeName, "Gengar") == 0) {
        ALLEGRO_BITMAP* gengar = nullptr;
        gengar = al_load_bitmap("gengar.png");
        al_draw_bitmap(gengar, 250, 210, 0);
    }

    if (strcmp(player.pokemon[1].pokeName, "Machamp") == 0) {
        ALLEGRO_BITMAP* machamp = nullptr;
        machamp = al_load_bitmap("machamp.png");
        al_draw_bitmap(machamp, 250, 210, 0);
    }

    if (strcmp(player.pokemon[1].pokeName, "Moltres") == 0) {
        ALLEGRO_BITMAP* moltres = nullptr;
        moltres = al_load_bitmap("moltres.png");
        al_draw_bitmap(moltres, 250, 210, 0);
    }

    if (strcmp(player.pokemon[1].pokeName, "Onix") == 0) {
        ALLEGRO_BITMAP* onix = nullptr;
        onix = al_load_bitmap("onix.png");
        al_draw_bitmap(onix, 250, 210, 0);
    }

    if (strcmp(player.pokemon[1].pokeName, "Venusaur") == 0) {
        ALLEGRO_BITMAP* venusaur = nullptr;
        venusaur = al_load_bitmap("venusaur.png");
        al_draw_bitmap(venusaur, 250, 210, 0);
    }

    if (strcmp(player.pokemon[1].pokeName, "Zapdos") == 0) {
        ALLEGRO_BITMAP* zapdos = nullptr;
        zapdos = al_load_bitmap("zapdos.png");
        al_draw_bitmap(zapdos, 250, 210, 0);
    }

    if (strcmp(player.pokemon[2].pokeName, "Alakazam") == 0) {
        ALLEGRO_BITMAP* alakazam = nullptr;
        alakazam = al_load_bitmap("alakazam.png");
        al_draw_bitmap(alakazam, 450, 210, 0);
    }

    if (strcmp(player.pokemon[2].pokeName, "Articuno") == 0) {
        ALLEGRO_BITMAP* articuno = nullptr;
        articuno = al_load_bitmap("articuno.png");
        al_draw_bitmap(articuno, 450, 210, 0);
    }

    if (strcmp(player.pokemon[2].pokeName, "Blastoise") == 0) {
        ALLEGRO_BITMAP* blastoise = nullptr;
        blastoise = al_load_bitmap("blastoise.png");
        al_draw_bitmap(blastoise, 450, 210, 0);
    }

    if (strcmp(player.pokemon[2].pokeName, "Charizard") == 0) {
        ALLEGRO_BITMAP* charizard = nullptr;
        charizard = al_load_bitmap("charizard.png");
        al_draw_bitmap(charizard, 450, 210, 0);
    }

    if (strcmp(player.pokemon[2].pokeName, "Gengar") == 0) {
        ALLEGRO_BITMAP* gengar = nullptr;
        gengar = al_load_bitmap("gengar.png");
        al_draw_bitmap(gengar, 450, 210, 0);
    }

    if (strcmp(player.pokemon[2].pokeName, "Machamp") == 0) {
        ALLEGRO_BITMAP* machamp = nullptr;
        machamp = al_load_bitmap("machamp.png");
        al_draw_bitmap(machamp, 450, 210, 0);
    }

    if (strcmp(player.pokemon[2].pokeName, "Moltres") == 0) {
        ALLEGRO_BITMAP* moltres = nullptr;
        moltres = al_load_bitmap("moltres.png");
        al_draw_bitmap(moltres, 450, 210, 0);
    }

    if (strcmp(player.pokemon[2].pokeName, "Onix") == 0) {
        ALLEGRO_BITMAP* onix = nullptr;
        onix = al_load_bitmap("onix.png");
        al_draw_bitmap(onix, 450, 210, 0);
    }

    if (strcmp(player.pokemon[2].pokeName, "Venusaur") == 0) {
        ALLEGRO_BITMAP* venusaur = nullptr;
        venusaur = al_load_bitmap("venusaur.png");
        al_draw_bitmap(venusaur, 450, 210, 0);
    }

    if (strcmp(player.pokemon[2].pokeName, "Zapdos") == 0) {
        ALLEGRO_BITMAP* zapdos = nullptr;
        zapdos = al_load_bitmap("zapdos.png");
        al_draw_bitmap(zapdos, 450, 210, 0);
    }
    al_flip_display();

    //Puts the name of the winner in the Hall of Fame for eternal glory!
    FILE* fptr;
    fptr = fopen("Hall of Fame.txt", "a");
    fprintf(fptr, "\n%s: %s, %s, %s", player.name, player.pokemon[0].pokeName, player.pokemon[1].pokeName, player.pokemon[2].pokeName);
    fclose(fptr);

    //This loop just makes sure this display stays forever until user closes the console
    game = true;
    while (game == true) {

    }
}

void showTutorial() {

    //Displays all the necessary guide
    refreshScreen1();
    al_draw_textf(font1, BLACK, 490, -5, ALLEGRO_ALIGN_CENTRE, "______________|_______________________________________________________________|______________");
    al_draw_textf(font2, BLACK, 490, 20, ALLEGRO_ALIGN_CENTRE, "|1v1 MULTIPLAYER POKEMON GAME|");
    al_draw_textf(font1, BLACK, 490, 50, ALLEGRO_ALIGN_CENTRE, "______________|_______________________________________________________________|______________");
    al_draw_textf(font1, BLACK, 490, 70, ALLEGRO_ALIGN_CENTRE, "Battle your opponent until either of you run out of Pokemons!");
    al_draw_textf(font1, BLACK, 490, 90, ALLEGRO_ALIGN_CENTRE, "________________________________________________________________________________________________");
    al_draw_textf(font1, BLACK, 490, 110, ALLEGRO_ALIGN_CENTRE, "The battle sequence goes: Player 1, Player2, Player 2, Player 1, etc!");
    al_draw_textf(font1, BLACK, 490, 130, ALLEGRO_ALIGN_CENTRE, "________________________________________________________________________________________________");
    al_draw_textf(font1, BLACK, 490, 150, ALLEGRO_ALIGN_CENTRE, "Water > Fire > Leaf > Psychic > Rock > Water");
    al_draw_textf(font1, BLACK, 490, 170, ALLEGRO_ALIGN_CENTRE, "________________________________________________________________________________________________");
    al_draw_textf(font1, BLACK, 490, 190, ALLEGRO_ALIGN_CENTRE, "During a battle, your Pokemon will be to the right/left of your name!");
    al_draw_textf(font1, BLACK, 490, 210, ALLEGRO_ALIGN_CENTRE, "________________________________________________________________________________________________");
    al_draw_textf(font1, BLACK, 490, 230, ALLEGRO_ALIGN_CENTRE, "Read the user guide for more information on what each attack does!");
    al_draw_textf(font1, BLACK, 490, 250, ALLEGRO_ALIGN_CENTRE, "________________________________________________________________________________________________");
    al_draw_textf(font1, BLACK, 490, 270, ALLEGRO_ALIGN_CENTRE, "Use arrow keys to navigate! (No mouse functionalities yet!)");
    al_draw_textf(font1, BLACK, 490, 480, ALLEGRO_ALIGN_CENTRE, "Press Enter to Continue!");
    al_flip_display();

    //This loop makes it so that the tutorial screen stays until the user presses on the enter key and releases it
    while (game == true) {
        ev.keyboard.keycode = ALLEGRO_KEY_BUTTON_L1;
        al_wait_for_event(event_queue, &ev);

        if (ev.keyboard.keycode == ALLEGRO_KEY_ENTER) {
            if (ALLEGRO_EVENT_KEY_UP == ev.type) {
                game = false;
            }
        }
    }
}
