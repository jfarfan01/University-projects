/*
 * Implement the missing functionality in this file. DO NOT CHANGE ANY OTHER FILE!
 * Add your code only inside the given TODO borders
 */

#include "spaceinvader.h"
#include <stdlib.h>

WINDOW *game_window = 0;
char game_map[MAP_LENGTH][MAP_WIDTH];

int spawn_number_of_enemies = 30;
int spawn_number_of_asteroids = 7;

pthread_t player_tid;
pthread_t *alien_tids;
pthread_t *asteroid_tids;
pthread_t shot_tid;

time_t time1;

position player_position = {MAP_WIDTH / 2, MAP_LENGTH - 2};
position *alien_positions;

int lifepoints = 100;
int points = 0;
int aliens_alive = 0;

bool is_shot_alive = false;

void *rvalue_player = 0;
void **rvalue_enemies;
void *rvalue_shot = 0;

void *playerLogic()
{
  game_map[player_position.y_][player_position.x_] = (char)PLAYER;

  char prev_direction = '\0';

  int c;
  keypad(stdscr, TRUE);
  noecho();
  timeout(1);

  while (lifepoints > 0 && aliens_alive)
  {
    char shoot = 0;
    char direction = '\0';

    c = getchar();
    switch (c)
    {
    case 's':
      direction = 'd';
      break;
    case 'w':
      direction = 'u';
      break;
    case 'a':
      direction = 'l';
      break;
    case 'd':
      direction = 'r';
      break;
    case ' ':
      shoot = 1;
      break;
    default:
      break;
    }
    if (c == 'q')
    {
      lifepoints = 0;
      continue;
    }

    usleep(100000);

    // TODO (7):
    // If the player is currently not shooting, spawn the shot. There is only *ONE* shot allowed at a time (is_shot_alive might help you as a variable).
    // The shot is spawned at the relative to the position of the player (player_position) one field upwards in the y direction. (aka above the player in the game map)
    // The shot should run in its own thread and the thread id is stored in shot_tid.
    // Make sure to wait until the last shot is finished before spawning a new one. Save the return value of the thread in rvalue_shot.

    if (shoot)
    { 

      if (!is_shot_alive)
      {
        if(shot_tid != 0 && shot_tid != INVALID_TID) //!!
        {
          pthread_join(shot_tid, &rvalue_shot);
        }
        parameters *parameters_ptr = malloc(sizeof(parameters));
        // *parameters_ptr = {->pos_x_ = player_position.x_, .pos_y_ = player_position.y_ - 1, .type_ = TYPE_SHOT};
        parameters_ptr -> pos_x_ = player_position.x_;
        parameters_ptr->pos_y_ = player_position.y_ - 1;
        parameters_ptr -> type_ = TYPE_SHOT;
        is_shot_alive = true;
        pthread_create(&shot_tid, NULL, (void *(*)(void *))placeShot, parameters_ptr);
      }
      // pthread_join(shot_tid, &rvalue_shot);
    }

    // TODO END

    movePlayer(&direction, &prev_direction, &player_position, PLAYER);
  }

  return (void *)998;
}

void *enemyAsteroid(parameters *params)
{
  if (params->type_ != TYPE_ENEMY_ASTEROID)
    return (void *)-1;

  // TODO (3):
  // Spawn the asteroids correctly in the map. The given position gives the position of the top left field.
  // Make sure to initialize the other fields correctly!
  // drawEnemy *only* draws *one* field on the map.
  pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
  int x = params-> pos_x_;
  int y = params ->pos_y_;
  free(params);


  while (1)
  {
    drawEnemy(TYPE_ENEMY_ASTEROID, x, y);
    drawEnemy(TYPE_ENEMY_ASTEROID, x + 1, y);
    drawEnemy(TYPE_ENEMY_ASTEROID, x, y + 1);
    drawEnemy(TYPE_ENEMY_ASTEROID, x + 1, y + 1);
  }

  // return (void*) 0;
    

  // TODO END
}

void *enemyAlien(parameters *params)
{
  if (params->type_ != TYPE_ENEMY_ALIEN)
    return (void *)-1;

  position local_legion_position = legion_position;

  // TODO (4):
  // -) set the alien position correctly based on the received params
  pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);

  alien_positions[params->index_].x_ = params->pos_x_;
  alien_positions[params->index_].y_ = params->pos_y_;
  int index = params->index_;
  free(params);
  // TODO END

  while (1)
  {
    char prev_direction = 0;
    char direction = 'd';

    if (local_legion_position.y_ != legion_position.y_)
      movePlayer(&direction, &prev_direction, &alien_positions[index], TYPE_ENEMY_ALIEN);

    if (local_legion_position.y_ < legion_position.y_)
    {
      direction = 'r';
      movePlayer(&direction, &prev_direction, &alien_positions[index], TYPE_ENEMY_ALIEN);
    }
    else if (local_legion_position.y_ > legion_position.y_)
    {
      direction = 'l';
      movePlayer(&direction, &prev_direction, &alien_positions[index], TYPE_ENEMY_ALIEN);
    }

    local_legion_position = legion_position;

    drawEnemy(TYPE_ENEMY_ALIEN, alien_positions[index].x_, alien_positions[index].y_);
  }
}

void init_enemies(unsigned char type, int number_of_enemy_type)
{
  pthread_attr_t enemy_attr;
  pthread_attr_init(&enemy_attr);

  // TODO (2): Implement the whole function:
  // - spawn all enemies of the given type (one thread per entity), make sure each one has the type specified in type_
  // - use the attribute for thread creation
  // - use the alien_tids and asteroid_tids arrays to store the thread IDs in them
  // - look at the provided data structures in the header. Use them!
  // - make sure to get a new random position for each asteroid!
  // - Arguments needed? generate them via getRandPosX() and getRandPosY()
  // - pay attention to not call the getRandPos-functions unnecessary or more often then you need, since this will fail on the testsystem
  // - Furthermore, make sure to keep the lifetime of variables in mind and to not leak any memory!

  // Aliens: Their y position starts at index 4.
  // Keep the leftmost AND rightmost 3 (x)fields open for legion movement => wrap to next line. (y+1) (---XXXXXX---) Also make sure to increase alien_alive
  // You can place the aliens however you like, as long as all are spawned, they are not overlapping each other and you keep in mind the 3 field rule from above.
  // e.g.(---X-X-X-X--X---) is possible as well
  // Multiple line wraps should be supported!

  // It is ok for aliens and asteroids to overlap! It is also ok to have race conditions in your code. The testsystem will not test for this
  // and you will learn how to fix them in A2!

  pthread_attr_setdetachstate(&enemy_attr, PTHREAD_CREATE_JOINABLE);

  if (type == TYPE_ENEMY_ASTEROID)
  {
    for (int i = 0; i < number_of_enemy_type; i++)
    {
      parameters *params = malloc(sizeof(parameters));
      params->pos_x_ = getRandPosX();
      params->pos_y_ = getRandPosY();
      params->type_ = type;
      params->index_ = i;
      pthread_create(&(asteroid_tids[i]), &enemy_attr, (void *(*)(void *))enemyAsteroid, params);
    }
  }

  if (type == TYPE_ENEMY_ALIEN)
  {
    for (int i = 0; i < number_of_enemy_type; i++)
    {
      parameters *params = malloc(sizeof(parameters));
      params->pos_x_ = MAP_WIDTH - 10 - 3 - ((i % 5) * 2);
      params->pos_y_ = 4 + (i / 5);
      params->type_ = type;
      params->index_ = i;
      aliens_alive++;
      pthread_create(&(alien_tids[i]), &enemy_attr, (void *(*)(void *))enemyAlien, params);
    }
  }

  // TODO END

  pthread_attr_destroy(&enemy_attr);
}

void *placeShot(parameters *params)
{
  if (params->type_ != TYPE_SHOT)
    return (void *)-1;

  // TODO (5):
  // - In general there should be only one shot on the map at a time
  // - place the shot on the position taken from the function parameters

  position pos = {params->pos_x_, params->pos_y_};
  free(params);

  // TODO END

  while (lifepoints > 0 && aliens_alive)
  {
    if (!IS_IN_BOUNDS(pos.x_, pos.y_))
    {
      is_shot_alive = false;
      game_map[pos.y_][pos.x_] = EMPTY;
      break;
    }

    drawShot(pos.x_, pos.y_);

    // check if we hit an enemy...

    int next_y_pos = pos.y_ - 1;
    if (!IS_IN_BOUNDS(pos.x_, next_y_pos))
    {
      is_shot_alive = false;
      game_map[pos.y_][pos.x_] = EMPTY;
      break;
    }
    char map_val = getMapValue(pos.x_, next_y_pos);

    if (map_val == TYPE_ENEMY_ASTEROID)
    {
      is_shot_alive = false;
      game_map[pos.y_][pos.x_] = EMPTY;
      break;
    }

    if (map_val == TYPE_ENEMY_ALIEN)
    {
      is_shot_alive = false;

      // TODO (6):
      // Find out if the shot hit an alien and if so, kill it and increase the points by POINTS. Also reduce aliens_alive
      // Make sure to kill all related threads and wait for their termination! Clean up the game map.
      for (int i = 0; i < spawn_number_of_enemies; i++)
      {
        if (alien_positions[i].x_ == pos.x_ && alien_positions[i].y_ == next_y_pos)
        {
          pthread_cancel(alien_tids[i]);
          pthread_join(alien_tids[i], &rvalue_enemies[i]);
          points += POINTS;
          aliens_alive--;
          game_map[alien_positions[i].y_][alien_positions[i].x_] = EMPTY;
          break;
        }
      }

      // TODO END

      game_map[pos.y_][pos.x_] = EMPTY;
      break;
    }

    char prev_direction = 0;
    if (!movePlayer("u", &prev_direction, &pos, TYPE_SHOT))
    {
      is_shot_alive = false;
      // we hit a wall
      break;
    }

    usleep(30000);
  }

  return (void *)999;
}

int startGame(int argc, char *argv[])
{

  init_screen();
  srand((unsigned int)time(&time1));

  alien_positions = malloc(sizeof(position) * spawn_number_of_enemies);
  alien_tids = malloc(sizeof(pthread_t) * spawn_number_of_enemies);
  asteroid_tids = malloc(sizeof(pthread_t) * spawn_number_of_asteroids);
  rvalue_enemies = malloc(sizeof(void *) * (spawn_number_of_enemies + spawn_number_of_asteroids));
  memset(rvalue_enemies, 0, sizeof(void *) * (spawn_number_of_enemies + spawn_number_of_asteroids));

  init_map();

  // TODO (1):
  // - spawn the player and enemies by creating separate threads for each entity
  // - Arguments needed? Attributes needed? Look at the playerLogic and the enemy functions
  // - look at the pthread_t variables from above... USE them!!
  // - Init all aliens with one call to init_enemies and all asteroids with another call to init_enemies
  // - Generate spawn_number_of_enemies and spawn_number_of_asteroids amount of entities! Make sure to not override those variables!

  init_enemies(TYPE_ENEMY_ALIEN, spawn_number_of_enemies);
  init_enemies(TYPE_ENEMY_ASTEROID, spawn_number_of_asteroids);
  pthread_create(&player_tid, NULL, &playerLogic, NULL);

  // TODO END

  refreshMap();

  while (lifepoints > 0 && aliens_alive)
  {
    usleep(1000);

    doLegionMovement();

    refreshMap();
  }

  refreshMap();

  // TODO (8):
  // - make sure that all the running threads are terminated before returning from main and fetch their return values
  // - All threads which are able to return on their own should also do that so and therefore give back the corresponding return value
  // - Force the termination of threads which do not terminate on their own.
  // - have a closer look on the other TODOs. There might be some connections
  // - we want to make sure, that all threads are terminated for sure. But are all threads even joinable?
  pthread_join(player_tid, &rvalue_player);
  pthread_join(shot_tid, &rvalue_shot);

  for (int i = 0; i < spawn_number_of_enemies; i++)
  {
    if( getMapValue(alien_positions[i].x_, alien_positions[i].y_) == TYPE_ENEMY_ALIEN)
    {
      pthread_cancel(alien_tids[i]);
      pthread_join(alien_tids[i], &rvalue_enemies[i]);
    }
  }

  for (int i = 0; i < spawn_number_of_asteroids; i++)
  {
    pthread_cancel(asteroid_tids[i]);
    pthread_join(asteroid_tids[i], &rvalue_enemies[i + spawn_number_of_enemies]);
  }

  // TODO END

  free(alien_positions);
  free(alien_tids);
  free(asteroid_tids);
  // rvalue_enemies gets freed inside end_game
  return end_game((void **)rvalue_player, (void *)rvalue_enemies, (void *)rvalue_shot);
}