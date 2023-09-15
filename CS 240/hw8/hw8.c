/* Name, hw8.c, CS 24000, Spring 2023
 * Last updated Feb 4, 2023
 */

/* Add any includes here */
#include "hw8.h"

#include <assert.h>
#include <malloc.h>
#include <stdio.h>
#include <string.h>

/*
 * inserts a player into a club
 */

int insert_player(club_t *list, char *club_name, char *player_name,
                  int player_salary) {
  assert(list != NULL);
  assert(club_name != NULL);
  assert(player_name != NULL);
  assert(player_salary > 0);

  club_t *club = list;
  while (club != NULL) {
    if (strcmp(club->club_name, club_name) == 0) {
      break;
    }
    club = club->next_club;
  }
  if (club == NULL) {
    return NO_SUCH_CLUB;
  }

  player_t *player = malloc(sizeof(player_t));
  assert(player != NULL);
  player->name = malloc(strlen(player_name) + 1);
  assert(player->name != NULL);
  strcpy(player->name, player_name);
  player->salary = player_salary;
  player->next_player = NULL;

  player_t *temp_player = club->players;
  if (player_salary > temp_player->salary) {
    player->next_player = temp_player;
    club->players = player;
    club->player_count++;
    return OK;
  }
  while (temp_player->next_player != NULL) {
    if (player_salary > temp_player->next_player->salary) {
      player->next_player = temp_player->next_player;
      temp_player->next_player = player;
      club->player_count++;
      return OK;
    }
    temp_player = temp_player->next_player;
  }
  temp_player->next_player = player;
  player->next_player = NULL;
  club->player_count++;
  return OK;
} /* insert_player() */

/*
 * Retires a player from a club
 */

int retire_player(club_t *list, char *club_name, char *player_name) {
  assert(list != NULL);
  assert(club_name != NULL);
  assert(player_name != NULL);
  club_t *club = NULL;
  club_t *temp = list;
  while (temp != NULL) {
    if (strcmp(temp->club_name, club_name) == 0) {
      club = temp;
      break;
    }
    temp = temp->next_club;
  }
  if (temp == NULL) {
    return NO_SUCH_CLUB;
  }
  player_t *temp_player = club->players;
  player_t *ret_player = NULL;
  while (temp_player != NULL) {
    if (strcmp(temp_player->name, player_name) == 0) {
      ret_player = temp_player;
      break;
    }
    temp_player = temp_player->next_player;
  }
  if (temp_player == NULL) {
    return NO_SUCH_PLAYER;
  }
  if (ret_player == club->players) {
    club->players = ret_player->next_player;
    free(ret_player->name);
    free(ret_player);
    club->player_count--;
    return OK;
  }
  temp_player = club->players;
  while (temp_player->next_player != ret_player) {
    temp_player = temp_player->next_player;
  }
  temp_player->next_player = ret_player->next_player;
  free(ret_player->name);
  free(ret_player);
  club->player_count--;
  return OK;
} /* retire_player() */

/*
 * Transfers a player from one club to another
 */

int transfer_player(club_t *list, char *club_name, char *new_club_name,
                    char *player_name) {
  assert(list != NULL);
  assert(club_name != NULL);
  assert(new_club_name != NULL);
  assert(player_name != NULL);
  if (strcmp(club_name, new_club_name) == 0) {
    return INVALID_TRANSFER;
  }

  int salary = 0;
  club_t *club = NULL;
  club_t *temp = list;
  while (temp != NULL) {
    if (strcmp(temp->club_name, club_name) == 0) {
      club = temp;
      break;
    }
    temp = temp->next_club;
  }
  if (temp == NULL) {
    return NO_SUCH_CLUB;
  }
  player_t *temp_player = club->players;
  player_t *ret_player = NULL;
  while (temp_player != NULL) {
    if (strcmp(temp_player->name, player_name) == 0) {
      ret_player = temp_player;
      break;
    }
    temp_player = temp_player->next_player;
  }
  if (temp_player == NULL) {
    return NO_SUCH_PLAYER;
  }
  int found = 0;
  if (ret_player == club->players) {
    club->players = ret_player->next_player;
    salary = ret_player->salary;
    free(ret_player->name);
    free(ret_player);
    club->player_count--;
    found = 1;
  }
  if (found == 0) {
    temp_player = club->players;
    while (temp_player->next_player != ret_player) {
      temp_player = temp_player->next_player;
    }
    temp_player->next_player = ret_player->next_player;
    salary = ret_player->salary;
    free(ret_player->name);
    free(ret_player);
    club->player_count--;
  }
  int i = insert_player(list, new_club_name, player_name, salary);
  if (i != OK) {
    return i;
  }
  return OK;
} /* transfer_player() */

/*
 * Finds the club with no players and removes them from the list
 */

club_t *find_dead_clubs(club_t *list) {
  assert(list != NULL);
  club_t *temp = list;
  club_t *prev = NULL;
  while (temp != NULL) {
    if (temp->player_count == 0) {
      if (prev == NULL) {
        list = temp->next_club;
        free(temp->club_name);
        free(temp->coach);
        free(temp);
        temp = list;
      }
      else {
        prev->next_club = temp->next_club;
        free(temp->club_name);
        free(temp->coach);
        free(temp);
        temp = prev->next_club;
      }
    }
    else {
      prev = temp;
      temp = temp->next_club;
    }
  }
  return list;
} /* find_dead_clubs() */

/*
 * Finds the club with the highest score
 */

club_t *most_valuable_club(club_t *list) {
  assert(list != NULL);
  club_t *temp_club = list;
  double value = -1;
  club_t *max = NULL;
  while (temp_club != NULL) {
    player_t *temp_player = temp_club->players;
    double total_salary = 0;
    while (temp_player != NULL) {
      total_salary += temp_player->salary;
      temp_player = temp_player->next_player;
    }
    double score = (temp_club->goals_scored) / (temp_club->goals_lost + 1.0) *
                   total_salary;
    if (score > value) {
      value = score;
      max = temp_club;
    }
    temp_club = temp_club->next_club;
  }
  return max;
} /* most_valuable_club() */

/*
 * Deletes a club from the list
 */

club_t *delete_club(club_t *list, char *club_name) {
  assert(list != NULL);
  assert(club_name != NULL);
  club_t *temp = list;
  club_t *prev = NULL;
  while (temp != NULL) {
    if (strcmp(temp->club_name, club_name) == 0) {
      if (prev == NULL) {
        list = temp->next_club;
        free(temp->club_name);
        free(temp->coach);
        player_t *free_players = temp->players;
        while (free_players != NULL) {
          player_t *temp = free_players;
          free_players = free_players->next_player;
          free(temp->name);
          free(temp);
        }
        free(temp);
        temp = list;
      }
      else {
        prev->next_club = temp->next_club;
        free(temp->club_name);
        free(temp->coach);
        player_t *free_players = temp->players;
        while (free_players != NULL) {
          player_t *temp = free_players;
          free_players = free_players->next_player;
          free(temp->name);
          free(temp);
        }
        free(temp);
        temp = prev->next_club;
      }
    }
    else {
      prev = temp;
      temp = temp->next_club;
    }
  }
  return list;
} /* delete_club() */