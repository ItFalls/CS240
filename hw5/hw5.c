/* Ethan Ling, hw5.c, CS 24000, Spring 2023
 * Last updated Feb 22, 2023
 */

/* Add any includes here */
#include "hw5.h"
#include <assert.h>
#include <ctype.h>
#include <math.h>
#include <string.h>

/* Define your functions here */

/*
 * Read a cupboard from a file
 */

cupboard_t read_cupboard(FILE *file, int index) {
  assert((file != NULL) && (index >= 0));
  cupboard_t cupboard = BAD_CUPBOARD;

  fseek(file, 0, SEEK_SET);
  fseek(file, 0, SEEK_END);
  int size = ftell(file);
  if (index >= size / sizeof(cupboard_t)) {
    return BAD_CUPBOARD;
  }

  fseek(file, index * sizeof(cupboard_t), SEEK_SET);
  int stat = fread(&cupboard, sizeof(cupboard_t), 1, file);

  if (stat != 1) {
    return BAD_CUPBOARD;
  }

  return cupboard;
} /* read_cupboard() */

/*
 * Write a cupboard to a file
 */

int write_cupboard(FILE *file, cupboard_t cupboard, int index) {
  assert((file != NULL) && (index >= 0));
  assert(strlen(cupboard.id) == MAX_ID_LEN - 1);
  for (int i = 0; i < strlen(cupboard.id); i++) {
    assert(isalnum(cupboard.id[i]));
  }
  assert(cupboard.max_weight > 0);
  assert(cupboard.age >= 0);
  assert((cupboard.material >= 0) && (cupboard.material <= 3));
  assert((cupboard.content >= 0) && (cupboard.content <= 2));
  for (int i = 0; i < N_HEIGHT; i++) {
    for (int j = 0; j < N_WIDTH; j++) {
      assert(cupboard.storage[i][j] >= 0);
    }
  }

  fseek(file, 0, SEEK_SET);
  fseek(file, 0, SEEK_END);
  int size = ftell(file);
  if (index >= size / sizeof(cupboard_t)) {
    return WRITE_ERROR;
  }
  fseek(file, index * sizeof(cupboard_t), SEEK_SET);
  if (fwrite(&cupboard, sizeof(cupboard_t), 1, file) != 1) {
    return WRITE_ERROR;
  }
  return OK;
} /* write_cupboard() */

/*
 * Find the actual max weight of a cupboard
 */

float find_actual_max_weight(FILE *file, char *id) {
  assert((file != NULL) && (id != NULL));
  fseek(file, 0, SEEK_SET);
  fseek(file, 0, SEEK_END);
  int size = ftell(file);
  for (int x = 0; x < size / sizeof(cupboard_t); x++) {
    cupboard_t cupboard = read_cupboard(file, x);
    if (strcmp(cupboard.id, id) == 0) {
      double rate = 0;
      int year = 0;
      switch (cupboard.material) {
        case STEEL:
          rate = 0.95;
          year = 5;
          break;
        case WOOD:
          rate = 0.9;
          year = 7;
          break;
        case PLASTIC:
          rate = 0.88;
          year = 3;
          break;
        case CARDBOARD:
          rate = 0.75;
          year = 2;
          break;
      }
      int det = cupboard.age / year;
      if (cupboard.age % year != 0) {
        det++;
      }
      return cupboard.max_weight * pow(rate, det);
    }
  }
  return NO_SUCH_CUPBOARD;
} /* find_actual_max_weight() */

/*
 * Finds the amount of storage space available for a given content type
 */

float potential_storage_space(FILE *file, enum content_t content) {
  assert(file != NULL);
  assert((content >= 0) && (content <= 2));
  fseek(file, 0, SEEK_SET);
  fseek(file, 0, SEEK_END);
  int size = ftell(file);

  float out = 0;
  for (int i = 0; i < size / sizeof(cupboard_t); i++) {
    cupboard_t cupboard = read_cupboard(file, i);
    if (cupboard.content == content) {
      float max = find_actual_max_weight(file, cupboard.id);
      float stored = 0;
      for (int x = 0; x < N_HEIGHT; x++) {
        for (int y = 0; y < N_WIDTH; y++) {
          stored += cupboard.storage[x][y];
        }
      }
      if (stored < max) {
        out += max - stored;
      }
    }
  }
  if (out == 0) {
    return NO_SUCH_CONTENT;
  }
  else {
    return out;
  }
} /* potential_storage_space() */

/*
 * Find the index of the first cupboard that is overloaded
 */

int find_overloaded_cupboard(FILE *file) {
  assert(file != NULL);
  fseek(file, 0, SEEK_SET);
  fseek(file, 0, SEEK_END);
  int size = ftell(file);
  for (int i = 0; i < size / sizeof(cupboard_t); i++) {
    cupboard_t cupboard = read_cupboard(file, i);
    float max = find_actual_max_weight(file, cupboard.id);
    float weight = 0;
    for (int x = 0; x < N_HEIGHT; x++) {
      for (int y = 0; y < N_WIDTH; y++) {
        weight += cupboard.storage[x][y];
      }
    }
    if (max < weight) {
      return i;
    }
  }
  return NO_SUCH_CUPBOARD;
} /* find_overloaded_cupboard() */

/*
 * Find the index of the first cupboard that is empty
 */

int place_object(FILE *file, int width, int height, int mass, char *id,
                 int x, int y) {
  assert(file != NULL);
  assert(width > 0);
  assert(height > 0);
  assert(mass > 0);
  assert(id != NULL);
  assert(strlen(id) == MAX_ID_LEN - 1);
  for (int i = 0; i < strlen(id); i++) {
    assert(isalnum(id[i]));
  }
  assert((x >= 0) && (x < N_WIDTH));
  assert((y >= 0) && (y < N_HEIGHT));


  fseek(file, 0, SEEK_SET);
  fseek(file, 0, SEEK_END);
  int size = ftell(file);
  for (int i = 0; i < size / sizeof(cupboard_t); i++) {
    cupboard_t cupboard = read_cupboard(file, i);
    if (strcmp(cupboard.id, id) == 0) {
      float max = find_actual_max_weight(file, cupboard.id);
      float weight = 0;

      for (int x = 0; x < N_HEIGHT; x++) {
        for (int y = 0; y < N_WIDTH; y++) {
          weight += cupboard.storage[x][y];
        }
      }
      if (mass + weight > max) {
        return WEIGHT_ISSUE;
      }


      if ((x + width > N_WIDTH) || (y + height > N_HEIGHT)) {
        return SPACE_ISSUE;
      }

      float element = (float) mass / ((float) height * width);
      for (int col = x; col < x + width; col++) {
        for (int row = y; row < y + height; row++) {
          if (cupboard.storage[row][col] != 0) {
            return SPACE_ISSUE;
          }
          else {
            cupboard.storage[row][col] = element;
          }
        }
      }
      write_cupboard(file, cupboard, i);
      return OK;
    }
  }
  return NO_SUCH_CUPBOARD;
} /* place_object() */