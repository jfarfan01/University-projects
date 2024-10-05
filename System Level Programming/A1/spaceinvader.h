/*
 * DO NOT CHANGE THIS FILE!
 */

#include <ncurses.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <assert.h>
#include "string.h"
#include <time.h>

// Map geometry
#define MAP_WIDTH 50
#define MAP_LENGTH 50

#define IS_IN_BOUNDS(x, y) ((x >= 0 && x < MAP_WIDTH) && (y >= 0 && y < MAP_LENGTH))

// Field Types
#define EMPTY 1
#define PLAYER 2
#define TYPE_SHOT 3
#define TYPE_ENEMY_ALIEN 6
#define TYPE_ENEMY_ASTEROID 7

// ncurses constants
#define SPACE 51
#define PLAYER_COLOR 52
#define ALIEN_COLOR 53
#define SHOT_COLOR 54
#define ASTEROID_COLOR 55

// Game settings
#define POINTS 1000

#define INVALID_TID ((pthread_t)(-1))

#ifndef SNP_TUTOR_TEST_1_H
#define SNP_TUTOR_TEST_1_H

typedef struct
{
  unsigned char x_;
  unsigned char y_;
} position;

typedef struct
{
  unsigned char pos_x_;
  unsigned char pos_y_;
  unsigned char type_;
  unsigned char index_;
} parameters;

extern WINDOW *game_window;
extern char game_map[MAP_LENGTH][MAP_WIDTH];

extern int lifepoints;
extern int points;
extern int aliens_alive;
extern int spawn_number_of_enemies;
extern int spawn_number_of_asteroids;
extern pthread_t* alien_tids;
extern pthread_t* asteroid_tids;
extern position player_position;
extern position legion_position;
extern pthread_t player_tid;
extern pthread_t shot_tid;
extern time_t time1;
extern bool is_shot_alive;
extern position* alien_positions;

extern void *rvalue_player;
extern void **rvalue_enemies;
extern void *rvalue_shot;

void init_map();
void init_screen();

void drawShot(int x_pos, int y_pos);
void drawEnemy(char type, int x_pos, int y_pos);
char getMapValue(int x_pos, int y_pos);

void refreshMap();

int end_game(void *rvalue, void **rvalues_enemies, void *rvalue_shot);

void *playerLogic();

void *enemyAsteroid(parameters *params);
void *enemyAlien(parameters *params);

void *placeShot(parameters *params);

void init_enemies(unsigned char type, int number_of_enemy_type);

bool movePlayer(char *direction, char *prev_direction, position *pos, char type);

char getRandPosX();

char getRandPosY();

void doLegionMovement();

int startGame(int argc, char* argv[]);

#endif // SNP_TUTOR_TEST_1_H
