/* Ethan Ling, hw3.c, CS 24000, Spring 2023
 * Last updated Feb 8, 2023
 */

/* Add any includes here */
#include "hw3.h"

#include <math.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>

/* Define your global variables here */
int g_player_count = 0;
char g_player_name[MAX_PLAYERS][NUM_ATTR][MAX_NAME_LEN] = {};
int g_attributes[MAX_PLAYERS][NUM_ATTR] = {};
int g_shooting[MAX_PLAYERS][NUM_COLS] = {};
int g_statistics[MAX_PLAYERS][NUM_COLS] = {};

/* Define your functions here */

/*
 * open the file specified by the first argument and read/copies the records
 */

int read_tables(char *in_file) {
  int count = 0;
  if (in_file == NULL) {
    g_player_count = count;
    return FILE_READ_ERR;
  }
  FILE *file = fopen(in_file, "r");
  if (file == NULL) {
    g_player_count = count;
    return FILE_READ_ERR;
  }

  char player_name[MAX_NAME_LEN] = {""};
  char position[MAX_NAME_LEN] = {""};
  int height = 0;
  char height_unit[MAX_UNIT_LEN] = {""};
  int weight = 0;
  char weight_unit[MAX_UNIT_LEN] = {""};
  int att_2 = 0;
  int made_2 = 0;
  int att_3 = 0;
  int made_3 = 0;
  int att_ft = 0;
  int made_ft = 0;
  int rebound = 0;
  int assist = 0;
  int steal = 0;
  int block = 0;
  int turnover = 0;
  int foul = 0;

  int scan = fscanf(file, "%50[^,],%50[^|]|%d%4[^,],%d%4[^|]|%d,"
                          "%d,%d,%d,%d,%d|%d,"
                          "%d,%d,%d,%d,%d\n", player_name, position, &height,
                    height_unit, &weight, weight_unit, &att_2, &made_2, &att_3,
                    &made_3, &att_ft, &made_ft, &rebound, &assist, &steal,
                    &block, &turnover, &foul);
  if (scan == EOF) {
    fclose(file);
    file = NULL;
    g_player_count = count;
    return NO_DATA_POINTS;
  }

  while (scan != EOF) {
    if (count >= MAX_PLAYERS) {
      fclose(file);
      file = NULL;
      g_player_count = count;
      return OUT_OF_BOUNDS;
    }

    if (scan != 18) {
      fclose(file);
      file = NULL;
      g_player_count = count;
      return RECORD_ERROR;
    }

    if ((strlen(player_name) > MAX_NAME_LEN)
        || (strlen(position) > MAX_NAME_LEN)
        || (strlen(height_unit) > MAX_UNIT_LEN)
        || (strlen(weight_unit) > MAX_UNIT_LEN)
        || (((!strcasecmp(height_unit, "in")) &&
             (!strcasecmp(height_unit, "cm"))) ||
            (((!strcasecmp(weight_unit, "lbs")) &&
              (!strcasecmp(weight_unit, "kg")))))) {
      fclose(file);
      file = NULL;
      g_player_count = count;
      return RECORD_ERROR;
    }

    if ((height < 0) || (weight < 0) || (att_2 < 0) || (made_2 < 0) ||
        (att_3 < 0) || (made_3 < 0) || (att_ft < 0) || (made_ft < 0) ||
        (rebound < 0) || (assist < 0) || (steal < 0) || (block < 0) ||
        (turnover < 0) || (foul < 0) || (made_2 > att_2) || (made_3 > att_3)) {
      fclose(file);
      file = NULL;
      g_player_count = count;
      return BAD_DATA;
    }

    strcpy(g_player_name[count][0], player_name);
    strcpy(g_player_name[count][1], position);
    if (strcasecmp(height_unit, "in")) {
      g_attributes[count][0] = round(height * IN_PER_CM);
    }
    else if (strcasecmp(height_unit, "cm")) {
      g_attributes[count][0] = round(height);
    }
    else {
      fclose(file);
      file = NULL;
      g_player_count = count;
      return RECORD_ERROR;
    }
    if (strcasecmp(weight_unit, "lbs")) {
      g_attributes[count][1] = round(weight * LB_PER_KG);
    }
    else if (strcasecmp(weight_unit, "kg")) {
      g_attributes[count][1] = round(weight);
    }
    else {
      fclose(file);
      file = NULL;
      g_player_count = count;
      return RECORD_ERROR;
    }
    g_shooting[count][0] = att_2;
    g_shooting[count][1] = made_2;
    g_shooting[count][2] = att_3;
    g_shooting[count][3] = made_3;
    g_shooting[count][4] = att_ft;
    g_shooting[count][5] = made_ft;

    g_statistics[count][0] = rebound;
    g_statistics[count][1] = assist;
    g_statistics[count][2] = steal;
    g_statistics[count][3] = block;
    g_statistics[count][4] = turnover;
    g_statistics[count][5] = foul;

    scan = fscanf(file, "%50[^,],%50[^|]|%d%4[^,],%d%4[^|]|%d,%d,"
                        "%d,%d,%d,%d|%d,"
                        "%d,%d,%d,%d,%d\n", player_name, position, &height,
                  height_unit, &weight, weight_unit, &att_2, &made_2, &att_3,
                  &made_3, &att_ft, &made_ft, &rebound, &assist, &steal,
                  &block, &turnover, &foul);
    count++;
  }
  fclose(file);
  file = NULL;
  g_player_count = count;
  return g_player_count;
} /* read_tables() */

/*
 *  return the index of player with the most points made
 */

int find_top_scorer(void) {
  if (g_player_count == 0) {
    return NO_DATA_POINTS;
  }
  int max_points = 0;
  int max_index = 0;
  for (int i = 0; i < g_player_count; i++) {
    int points = g_shooting[i][1] * 2
                 + g_shooting[i][3] * 3 + g_shooting[i][5];
    if (points > max_points) {
      max_points = points;
      max_index = i;
    }
  }
  return max_index;

} /* find_top_scorer() */

/*
 *  return the index of player with the most accurate shots
 */

int find_most_accurate_player(void) {
  if (g_player_count == 0) {
    return NO_DATA_POINTS;
  }
  float max_accuracy = -1;
  float max_attempts = -1;
  int max_height = -1;
  int max_index = -1;
  for (int i = 0; i < g_player_count; i++) {
    float made = (float) (g_shooting[i][1] + g_shooting[i][3] +
                          g_shooting[i][5]);
    float attempts = (float) (g_shooting[i][0] + g_shooting[i][2] +
                              g_shooting[i][4]);
    float accuracy = made / attempts;

    if (accuracy > max_accuracy) {
      max_accuracy = accuracy;
      max_attempts = attempts;
      max_height = g_attributes[i][0];
      max_index = i;
    }
    else if (accuracy == max_accuracy) {
      if (attempts > max_attempts) {
        max_accuracy = accuracy;
        max_attempts = attempts;
        max_height = g_attributes[i][0];
        max_index = i;
      }
      else if (attempts == max_attempts) {
        if (max_height > g_attributes[i][0]) {
          max_accuracy = accuracy;
          max_attempts = attempts;
          max_height = g_attributes[i][0];
          max_index = i;
        }
      }
    }
  }
  return max_index;
} /* find_most_accurate_player() */

/*
 *  return the number of players with height in the range
 */

int count_players_in_weight_range(int lower_bound, int upper_bound) {
  if (g_player_count == 0) {
    return NO_DATA_POINTS;
  }
  if (lower_bound > upper_bound) {
    return BAD_RANGE;
  }
  int count = 0;
  for (int i = 0; i < g_player_count; i++) {
    if ((g_attributes[i][1] >= lower_bound) &&
        (g_attributes[i][1] <= upper_bound)) {
      count++;
    }
  }
  if (count == 0) {
    return NO_SUCH_DATA;
  }
  return count;
} /* count_players_in_weight_range() */

/*
 *  return the index of player with the most attempts
 */

int find_most_attempts_player(char *given_position) {
  if (g_player_count == 0) {
    return NO_DATA_POINTS;
  }
  int max_attempts = 0;
  for (int i = 0; i < g_player_count; i++) {
    if (strcasecmp(given_position, g_player_name[i][1]) == 0) {
      int attempts = g_shooting[i][0] + g_shooting[i][2] + g_shooting[i][4];
      if (attempts > max_attempts) {
        max_attempts = attempts;
      }
    }
  }
  if (max_attempts == 0) {
    return NO_SUCH_DATA;
  }
  return max_attempts;
} /* find_most_attempts_player() */

/*
 *  calculate the grade of a player
 */

float calculate_coach_grade(char *player_name) {
  if (g_player_count == 0) {
    return NO_DATA_POINTS;
  }
  for (int x = 0; x < g_player_count; x++) {
    if (strcasecmp(player_name, g_player_name[x][0]) == 0) {
      float rebounds = g_statistics[x][0];
      float assists = g_statistics[x][1];
      float steals = g_statistics[x][2];
      float blocks = g_statistics[x][3];
      float turnovers = g_statistics[x][4];
      float fouls = g_statistics[x][5];
      float grade = (rebounds * 25 + assists * 20 + steals * 35 + blocks * 25
                     - turnovers * 15 - fouls * 20) /
                    (rebounds + assists + steals + blocks + turnovers + fouls +
                     1);
      if (grade < 0) {
        return BAD_PLAYER;
      }
      return grade;
    }
  }
  return NO_SUCH_DATA;
} /* calculate_coach_grade() */

/*
 *  calculate the grade of all players
 */

int calculate_all_players_grade(char *out_file) {
  if (g_player_count == 0) {
    return NO_DATA_POINTS;
  }
  if (out_file == NULL) {
    return FILE_WRITE_ERR;
  }
  FILE *file = fopen(out_file, "w");
  if (file == NULL) {
    return FILE_WRITE_ERR;
  }
  for (int i = 0; i < g_player_count; i++) {
    float grade = calculate_coach_grade(g_player_name[i][0]);
    if (grade == BAD_PLAYER) {
      grade = 0;
    }
    fprintf(file, "%s: %.2f\n", g_player_name[i][0], grade);
  }
  fclose(file);
  file = NULL;
  return OK;
} /* calculate_all_players_grade() */

/*
 *  write the table of players data
 */

int write_tables(char *out_file, int start_row, int end_row) {
  if (g_player_count == 0) {
    return NO_DATA_POINTS;
  }
  if ((start_row < 0) ||
      (end_row >= g_player_count) ||
      (end_row < start_row)) {
    return BAD_ROW;
  }
  if (out_file == NULL) {
    return FILE_WRITE_ERR;
  }

  FILE *file = fopen(out_file, "w");
  if (file == NULL) {
    return FILE_WRITE_ERR;
  }
  for (int x = start_row; x <= end_row; x++) {
    char *player_name = g_player_name[x][0];
    char *position = g_player_name[x][1];
    int height = g_attributes[x][0];
    char *height_unit = "in";
    int weight = g_attributes[x][1];
    char *weight_unit = "lbs";
    int att_2 = g_shooting[x][0];
    int made_2 = g_shooting[x][1];
    int att_3 = g_shooting[x][2];
    int made_3 = g_shooting[x][3];
    int att_ft = g_shooting[x][4];
    int made_ft = g_shooting[x][5];
    int rebound = g_statistics[x][0];
    int assist = g_statistics[x][1];
    int steal = g_statistics[x][2];
    int block = g_statistics[x][3];
    int turnover = g_statistics[x][4];
    int foul = g_statistics[x][5];
    fprintf(file, "%s,%s|%d%s,%d%s|%d,%d,%d,%d,%d,%d|"
                  "%d,%d,%d,%d,%d,%d\n", player_name, position, height,
            height_unit, weight, weight_unit, att_2, made_2, att_3, made_3,
            att_ft, made_ft, rebound, assist, steal, block, turnover,
            foul);
  }
  fclose(file);
  file = NULL;
  return OK;
} /* write_tables() */