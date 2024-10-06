#define BONUS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "framework.h"
#include <stdbool.h>

#define INCORRECT_PARAM_COUNT 1
#define FILE_CANT_BE_OPENED 2
#define INVALID_FILE 3
#define OOM 4

typedef struct Letter{
  char character;
  size_t value;
} Letter;

typedef struct Game_Field{
  size_t field_dimension_ver;
  size_t field_dimension_hor;
  char ** field;
  size_t current_player;
  size_t p1_points;
  size_t p2_points;
  Letter* letters;
  size_t letter_array_size;
  size_t insertions_so_far;
  size_t max_points;
  char file_path[64];

}Game_Field;

bool readConfigFile(const char* config_file, size_t *error_num, Game_Field* game_field);
void parseLetterLine(Game_Field * game_field, char* raw_letter_string);
void printBoard(const Game_Field game_field);
bool terminal(Game_Field* game_field, size_t * load_error);
void changePlayer(Game_Field* game_field);
bool performCommand(Game_Field* game_field, const Input input, const char* buffer, size_t * load_error);
void help();
bool insert(Game_Field* game_field, const Input);
bool checkInsert(const Game_Field game_field, const Input, const size_t row, const size_t column, size_t* points);
bool save(const Game_Field game_field);
void printCommand(const char * buffer);
bool load(Game_Field* Game_Field, const Input input);


int main(int argc, char** argv){
  size_t ret_val = 0;
  Game_Field game_field;
  game_field.insertions_so_far = 0;
  game_field.p1_points = 0;
  game_field.p2_points = 0;

  if(argc != 2){
    printf("Usage: ./a3 configfile\n");
    return INCORRECT_PARAM_COUNT;
  }

  if (!readConfigFile(argv[1], &ret_val, &game_field)){
    return ret_val;
  }
  size_t load_error = 0;
  printBoard(game_field);
  terminal(&game_field, &load_error);

  if(load_error == 2){
    ret_val = 2;
  }


  for(size_t i = 0; i < game_field.field_dimension_ver; i++){
    free(game_field.field[i]);
  }
  free(game_field.field);
  free(game_field.letters);
  
  return ret_val;
}

bool terminal(Game_Field* game_field, size_t * load_error){
  bool continue_game = true;
  while(continue_game){
    printf("Player %lu > ", game_field->current_player);
    char buffer[100];
    fgets(buffer, 100, stdin);
    Input input;
    parseCommand(buffer, &input);
    continue_game = !input.is_error_;
    continue_game =  performCommand(game_field, input, buffer, load_error);
  }
  return true;
}
void printCommand(const char * buffer){
  for(size_t i = 0; i < strlen(buffer); i++){
    if(buffer[i] != ' '  && buffer[i] != '\n' && buffer[i] != '\0'){
      printf("%c", buffer[i]);
    }
  }
  printf("\n");
}

bool performCommand(Game_Field* game_field, const Input input, const char* buffer, size_t * load_error){
  if(feof(stdin)){
    return false;
  }
  switch(input.command_){
    case HELP:
      help();
      return true;
      break;
    case QUIT:
      return false;
      break;
    case INSERT:
      if(!insert(game_field, input)){
        free(input.word_);
        return false;
      }
      free(input.word_);
      break;
    case SAVE:
      save(*game_field);
      break;
    case LOAD:
      if(!load(game_field, input)){
        *load_error = 2;
        return false;
      }
      break;
    default:
      printf("Error: Unknown command: ");
      printCommand(buffer);
      return true;
  }

  return true;
}

bool load(Game_Field* game_field, const Input input){
  size_t ret_val;
  FILE * try_file = fopen(input.load_config_, "r");
  if(try_file){
    for(size_t i = 0; i < game_field->field_dimension_ver; i++){
    free(game_field->field[i]);
    }
    free(game_field->field);
    free(game_field->letters);
    fclose(try_file);
  }

  if(!readConfigFile(input.load_config_, &ret_val, game_field)){
    free(input.load_config_);
    return false;
  }
  printBoard(*game_field);
  free(input.load_config_);
  return true;
}

bool save(const Game_Field game_field){
  FILE* file = fopen(game_field.file_path, "w");
  fprintf(file, "Scrabble\n");
  for(size_t i = 0; i < game_field.field_dimension_ver; i++){
    fprintf(file, "%s", game_field.field[i]);
  }
  fprintf(file,"%lu\n", game_field.current_player);
  fprintf(file, "%lu\n", game_field.p1_points);
  fprintf(file, "%lu\n", game_field.p2_points);

  for(size_t i = 0; i < game_field.letter_array_size; i ++){
    fprintf(file, "%c", game_field.letters[i].character);
    fprintf(file, "%lu", game_field.letters[i].value);
    if(i < game_field.letter_array_size - 1){
      fprintf(file, " ");
    }
  }

  fclose(file);
  return true;
}

bool insert(Game_Field * game_field, const Input input){
  if(input.is_error_ || !isalpha(input.column_) || ! isalpha(input.row_)){
    printf("Error: Insert parameters not valid!\n");
    return true;
  }
  size_t row = (size_t) toupper(input.row_) - 65;
  size_t column = (size_t) (size_t) toupper(input.column_) - 65;
  size_t points = 0;
  if(!checkInsert(*game_field, input, row, column, &points)){
    printf("Error: Impossible move!\n");
    return true;
  }

  for(size_t i = 0; i < strlen(input.word_); i++){
    if(input.orientation_ == HORIZONTAL ){
      game_field->field[row][column++] = toupper(input.word_[i]);
    }
    if(input.orientation_ == VERTICAL ){
      game_field->field[row++][column] = toupper(input.word_[i]);
    }
  }
  game_field->insertions_so_far++;
  if(game_field->current_player == 1){
    game_field->p1_points += points;
    if(game_field->p1_points >= game_field->max_points){
      printf("Player 1 has won the game with %lu points!\n",game_field->p1_points);
      return false;
    }
  }
  else if(game_field->current_player == 2){
    game_field->p2_points += points;
    if(game_field->p2_points >= game_field->max_points){
      printf("Player 2 has won the game with %lu points!\n",game_field->p2_points);
      return false;
    }
  }
  changePlayer(game_field);
  printBoard(*game_field);
  return true;

}

bool checkInsert(const Game_Field game_field, const Input input, const size_t row, const size_t column, size_t * points){
  size_t word_i = row;
  size_t word_j = column;

  if(column + strlen(input.word_) > (game_field.field_dimension_hor -2) && input.orientation_ == HORIZONTAL){
    return false;
  }
  if(row + strlen(input.word_) > (game_field.field_dimension_ver) && input.orientation_ == VERTICAL){
    return false;
  }
  bool word_connected = false;
  size_t player_points = 0;
  for(size_t i = 0; i < strlen(input.word_); i++){
    if(!isalpha(input.word_[i])){
      return false;
    }
    bool valid_letter = false;
    size_t letter_index;
    for(size_t letter_i = 0; letter_i < game_field.letter_array_size; letter_i++ ){
      if(toupper(game_field.letters[letter_i].character) == toupper(input.word_[i])){
        valid_letter = true;
        letter_index = letter_i;
      }
    }
    if(!valid_letter){
      return false;
    }
     
    if(toupper(input.word_[i]) == toupper(game_field.field[word_i][word_j])){
      word_connected = true;
    }
    if(isalpha(game_field.field[word_i][word_j]) && toupper(input.word_[i]) != toupper(game_field.field[word_i][word_j])){
      return false;
    }
    if(toupper(game_field.field[word_i][word_j]) != toupper(input.word_[i])){
     player_points += game_field.letters[letter_index].value;
    }

    if(input.orientation_ == HORIZONTAL ){
      word_j ++;
    }
    if(input.orientation_ == VERTICAL ){
      word_i++;
    }
  }
  if(!word_connected && (game_field.p1_points > 0 || game_field.p2_points > 0)){
    return false;
  }
  *points = player_points;

  return true;
}

bool readConfigFile(const char* config_file, size_t * errror_num, Game_Field* game_field){
  FILE * file =  fopen(config_file, "r");
  strncpy(game_field->file_path, config_file, strlen(config_file) + 1);
  char* og_magic_num = "Scrabble\n";
  if (!file){
    printf("Error: Cannot open file: %s\n", config_file);
    *errror_num = FILE_CANT_BE_OPENED;
    return false;
  }

  char buffer [256];

  size_t line_count = 0;
  char* magic_num = fgets(buffer, 256, file);
  line_count ++;
  size_t field_start = ftell(file);
  
  size_t len = sizeof(magic_num);
  if(strncmp(magic_num, og_magic_num, len + 1) != 0){
    printf("Error: Invalid file: %s\n", config_file);
    fclose(file);
    *errror_num = INVALID_FILE;
    return false;
  }

  while(!feof(file)){
    fgets(buffer, 256, file);
    line_count++;
  }
  fseek(file, field_start, SEEK_SET);

  game_field->field_dimension_ver = line_count - 5;
  game_field->field = (char **) malloc((game_field->field_dimension_ver) * sizeof(char*));

  game_field->field_dimension_hor = game_field->field_dimension_ver +2;

  for(size_t i = 0; i < game_field->field_dimension_ver; i++){
    game_field->field[i] = (char*) malloc((game_field->field_dimension_hor)* sizeof(char));
    fgets(game_field->field[i], game_field->field_dimension_hor, file);
  }

  game_field->max_points = (size_t) (game_field->field_dimension_ver * game_field->field_dimension_ver) / 2;
  
  fgets(buffer, 252, file);
  game_field->current_player = atoi(buffer);

  fgets(buffer, 252, file);
  game_field->p1_points = atoi(buffer);

  fgets(buffer, 252, file);
  game_field->p2_points = atoi(buffer);

  fgets(buffer, 252, file);
  parseLetterLine(game_field, buffer);

  fclose(file);
  return true;
}

void parseLetterLine(Game_Field * game_field, char* raw_letter_string)
{
  size_t i = 0; //string size
  for (; raw_letter_string[i] != '\0'; i++){

  }
  size_t string_size = i++;
  size_t letter_array_size = (string_size / 3) + 1;

  game_field->letters = (Letter*) malloc((letter_array_size) * sizeof(Letter));

  size_t idx = 0;
  for (size_t j = 0; idx < (letter_array_size) ; idx++){
    game_field->letters[idx].character = raw_letter_string[j++];
    game_field->letters[idx].value = (size_t) raw_letter_string[j++] -48;
    j++;
  }

  game_field->letter_array_size = letter_array_size;
}



void printBoard(const Game_Field game_field){

  char A = 'A';
  printf("\n");
  bool new_line_flag = true;
  for(size_t i = 0 ;i < game_field.letter_array_size; i++ ){
    new_line_flag = true;
    printf("%c%zu",toupper(game_field.letters[i].character), game_field.letters[i].value);
    if(i < game_field.letter_array_size - 1){
      printf(", ");
    }
    if((i + 1)% 9 == 0 ){
      printf("\n");
      new_line_flag = false;
    }
  }
  if(new_line_flag){
    printf("\n");
  }
  printf("  P1:%5lu Points\n", game_field.p1_points);
  printf("  P2:%5lu Points\n", game_field.p2_points);
  printf(" |");
  for(size_t i = 0; i < game_field.field_dimension_ver; i++){
    printf("%c",(char)A + (char)i);
  }
  printf("\n");
  for(size_t i = 0; i < game_field.field_dimension_hor; i ++){
    printf("-");
  }
  printf("\n");
  for(size_t i = 0; i < game_field.field_dimension_ver; i++){
    printf("%c|%s",(char)A + (char)i, game_field.field[i]);
  }
  printf("\n");
}

void changePlayer(Game_Field* game_field){
  if(game_field->current_player == 1){
    game_field->current_player = 2;
  }
  else{
    game_field->current_player = 1;
  }
}

void help(){
  printf("Commands:\n"
              " - insert <ROW> <COLUMN> <H/V> <WORD>\n"
              "    <H/V> stands for H: horizontal, V: vertical.\n\n"

              " - help\n"
              "    Prints this help text.\n\n"

              " - quit\n"
              "    Terminates the game.\n\n"

              " - save\n"
              "    Saves the game to the current config file.\n\n"

              " - load <CONFIGFILE>\n"
              "    load config file and start game.\n");

}
