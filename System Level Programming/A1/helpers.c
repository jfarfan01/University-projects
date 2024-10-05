/*
 * DO NOT CHANGE THIS FILE!
 */

#include "spaceinvader.h"
#include <ncurses.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>


position legion_position;


void init_map()
{
  for (int y = 0; y < MAP_LENGTH; y++)
  {
    for (int x = 0; x < MAP_WIDTH; x++)
    {
      game_map[y][x] = (char)EMPTY;
    }
  }
  legion_position.x_ = 0;
  legion_position.y_ = 0;
}

void init_screen()
{
  initscr();

  start_color();
  init_pair(SPACE, COLOR_YELLOW, COLOR_BLACK);
  init_pair(PLAYER_COLOR, COLOR_WHITE, COLOR_BLUE);
  init_pair(ALIEN_COLOR, COLOR_BLACK, COLOR_RED);
  init_pair(SHOT_COLOR, COLOR_BLACK, COLOR_MAGENTA);
  init_pair(ASTEROID_COLOR, COLOR_WHITE, COLOR_BLACK);

  if (has_colors() != TRUE)
  {
    printf("ERROR: Your Screen does not support colors\n");
  }
  game_window = newwin(MAP_LENGTH, MAP_WIDTH, 0, 0);
}

char getMapValue(int x_pos, int y_pos)
{
  if (IS_IN_BOUNDS(x_pos, y_pos))
    return game_map[y_pos][x_pos];
  return EMPTY;
}

void drawEnemy(char type, int x_pos, int y_pos)
{
  if (IS_IN_BOUNDS(x_pos, y_pos))
  {
    if (type == TYPE_ENEMY_ASTEROID)
    {
      game_map[y_pos][x_pos] = (char)TYPE_ENEMY_ASTEROID;
    }
    else if (type == TYPE_ENEMY_ALIEN)
    {
      game_map[y_pos][x_pos] = (char)TYPE_ENEMY_ALIEN;
    }
  }
}

void drawShot(int x_pos, int y_pos)
{
  if (IS_IN_BOUNDS(x_pos, y_pos))
    game_map[y_pos][x_pos] = (char)TYPE_SHOT;
}

void refreshMap()
{
  werase(game_window);
  mvwprintw(game_window, 1, 1, "SPACEINVADER");
  mvwprintw(game_window, 2, 1, "POINTS: %07d\n", points);

  for (int y = 3; y < MAP_LENGTH; ++y)
  {
    for (int x = 0; x < MAP_WIDTH; ++x)
    {
      if (game_map[y][x] != EMPTY)
      {
        if (game_map[y][x] == PLAYER)
        {
          wattron(game_window, COLOR_PAIR(PLAYER_COLOR));
          mvwaddch(game_window, y, x, 'P');
          wattroff(game_window, COLOR_PAIR(PLAYER_COLOR));
        }
        else if (game_map[y][x] == TYPE_ENEMY_ALIEN)
        {
          wattron(game_window, COLOR_PAIR(ALIEN_COLOR));
          mvwaddch(game_window, y, x, 'A');
          wattroff(game_window, COLOR_PAIR(ALIEN_COLOR));
        }
        else if (game_map[y][x] == TYPE_SHOT)
        {
          wattron(game_window, COLOR_PAIR(SHOT_COLOR));
          mvwaddch(game_window, y, x, '#');
          wattroff(game_window, COLOR_PAIR(SHOT_COLOR));
        }
        else if (game_map[y][x] == TYPE_ENEMY_ASTEROID)
        {
          wattron(game_window, COLOR_PAIR(ASTEROID_COLOR));
          mvwaddch(game_window, y, x, 'B');
          wattroff(game_window, COLOR_PAIR(ASTEROID_COLOR));
        }
      }
      else
      {
        wattron(game_window, COLOR_PAIR(SPACE));
        mvwaddch(game_window, y, x, '\0');
        wattroff(game_window, COLOR_PAIR(SPACE));
      }
    }
  }
  wrefresh(game_window);
}

char getRandPosX()
{
  return rand() % 30 + 3;
}

char getRandPosY()
{
  return rand() % 30 + 7;
}

bool movePlayer(char *direction, char *prev_direction, position *pos, char type)
{
  if (!*direction)
    return true;

  switch (*direction)
  {
  case 'l':
    if (pos->x_ < 1)
    {
      if (type == PLAYER)
      {
        lifepoints = 0;
        break;
      }
      else
      {
        // removing person
        game_map[pos->y_][pos->x_] = (char)EMPTY;

        // revoking on the opposite site of the map
        pos->x_ = MAP_WIDTH - 1;
        game_map[pos->y_][pos->x_] = (char)type;

        break;
      }
    }
    if (type == PLAYER)
    {
      if (game_map[pos->y_][pos->x_ - 1] == (char)TYPE_ENEMY_ASTEROID ||
          game_map[pos->y_][pos->x_ - 1] == (char)TYPE_ENEMY_ALIEN)
      {
        lifepoints = 0;
        break;
      }
    }
    game_map[pos->y_][pos->x_] = (char)EMPTY;
    game_map[pos->y_][--pos->x_] = (char)type;
    break;

  case 'r':
    if (pos->x_ > MAP_WIDTH - 2)
    {
      if (type == PLAYER)
      {
        lifepoints = 0;
        break;
      }
      else
      {
        // removing person
        game_map[pos->y_][pos->x_] = (char)EMPTY;

        // revoking on the opposite site of the map
        pos->x_ = 0;
        game_map[pos->y_][pos->x_] = (char)type;

        break;
      }
    }

    if (type == PLAYER)
    {
      if (game_map[pos->y_][pos->x_ + 1] == (char)TYPE_ENEMY_ASTEROID ||
          game_map[pos->y_][pos->x_ + 1] == (char)TYPE_ENEMY_ALIEN)
      {
        lifepoints = 0;
        break;
      }
    }
    game_map[pos->y_][pos->x_] = (char)EMPTY;
    game_map[pos->y_][++pos->x_] = (char)type;
    break;

  case 'u':
    if (pos->y_ < 4)
    {
      if (type == PLAYER)
      {
        lifepoints = 0;
        break;
      }
      else if (type == TYPE_SHOT)
      {
        game_map[pos->y_][pos->x_] = (char)EMPTY;
        return false;
      }
      else
      {
        // removing person
        game_map[pos->y_][pos->x_] = (char)EMPTY;

        // revoking on the opposite site of the map
        pos->y_ = MAP_LENGTH - 3;
        game_map[pos->y_][pos->x_] = (char)type;

        break;
      }
    }

    if (type == PLAYER)
    {
      if (game_map[pos->y_ - 1][pos->x_] == (char)TYPE_ENEMY_ASTEROID ||
          game_map[pos->y_ - 1][pos->x_] == (char)TYPE_ENEMY_ALIEN)
      {
        lifepoints = 0;
        break;
      }
    }
    game_map[pos->y_][pos->x_] = (char)EMPTY;
    game_map[--pos->y_][pos->x_] = (char)type;
    break;

  case 'd':
    if (pos->y_ > MAP_LENGTH - 2)
    {
      if (type == PLAYER)
      {
        lifepoints = 0;
        break;
      }
      else
      {
        // removing person
        game_map[pos->y_][pos->x_] = (char)EMPTY;

        // revoking on the opposite site of the map
        pos->y_ = 3;
        game_map[pos->y_][pos->x_] = (char)type;

        break;
      }
    }

    if (type == PLAYER)
    {
      if (game_map[pos->y_ + 1][pos->x_] == (char)TYPE_ENEMY_ASTEROID ||
          game_map[pos->y_ + 1][pos->x_] == (char)TYPE_ENEMY_ALIEN)
      {
        lifepoints = 0;
        break;
      }
    }
    game_map[pos->y_][pos->x_] = (char)EMPTY;
    game_map[++pos->y_][pos->x_] = (char)type;
    break;

  default:
    break;
  }
  *prev_direction = *direction;
  return true;
}

void doLegionMovement()
{
  static int counter = 0;
  static int y = 0;

  counter++;

  int highest_x = 0;
  int lowest_x = MAP_WIDTH;
  for (int i = 0; i < spawn_number_of_enemies; i++)
  {
    if (IS_IN_BOUNDS(alien_positions[i].x_, alien_positions[i].y_))
    {
      if (alien_positions[i].x_ > highest_x)
        highest_x = alien_positions[i].x_;
      if (alien_positions[i].x_ < lowest_x)
        lowest_x = alien_positions[i].x_;
    }
  }

  if (counter == 750)
  {
    y++;

    if (y >= 5)
    {
      legion_position.y_ += 1;
      y = 0;
    }

    while (1)
    {
      int idx = rand() % 3;

      if (idx == 0)
      {
        if (lowest_x >= 3)
          legion_position.x_ -= 1;
        break;
      }
      else if (idx == 1)
      {
        if (highest_x <= MAP_WIDTH - 3)
          legion_position.x_ += 1;
        break;
      }
      else
      {
      }
    }

    counter = 0;
  }
}

int end_game(void *rvalue, void **rvalues_enemies, void *rvalue_shot)
{
  if (game_window)
  {
    mvwprintw(game_window, (MAP_LENGTH / 2), 16, "    GAME OVER    ");
    mvwprintw(game_window, (MAP_LENGTH / 2 + 1), 16, " %08d POINTS ", points);
    wrefresh(game_window);
    usleep(5000000);
    wborder(game_window, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ');
    wclear(game_window);
    wrefresh(game_window);
    delwin(game_window);
    endwin();
  }
  free(rvalues_enemies);

  return 0;
}
