/* Ethan Ling, hw2.c, CS 24000, Spring 2023
 * Last updated Januaray 31, 2023
 */

/* Add any includes here */
#include "hw2.h"

#include <ctype.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

/* Define your functions here */

/*
 * Function to find the average speed of a given manufacturer
 */

float average_speed_of_manufacturer(char *file_name, char *manufacturer) {
  if ((file_name == NULL) || (manufacturer == NULL)) {
    return NO_DATA_POINTS;
  }

  FILE *file = fopen(file_name, "r");
  if (file == NULL) {
    return FILE_READ_ERR;
  }

  float total_speeds = 0;
  float manufacturer_count = 0;

  char car_manufacturer[20] = {""};
  char id[5] = {""};
  float pitstops = 0;
  float pitstop_distance = 0;
  float pitstop_time = 0;

  float test_dist = 0;
  float test_time = 0;
  float price = 0;
  float down = 0;
  int scan = fscanf(file, "%[^,],%[^,],%fkm,%fs,%f/%f/%f,$%f,%f%%\n",
                    id, car_manufacturer, &test_dist, &test_time, &pitstops, &pitstop_distance, &pitstop_time, &price,
                    &down);

  bool match = false;
  while (scan != EOF) {
    for (int x = 0; x < strlen(id); x++) {
      if ((!isdigit(id[x])) || (x > 5)) { //checks if is a digit and length is at most 5
        fclose(file);
        file = NULL;
        return BAD_ID;
      }
    }
    if (scan != 9) {
      fclose(file);
      file = NULL;
      return BAD_RECORD;
    }

    if (strcmp(car_manufacturer, manufacturer) == 0) {
      match = true;
      manufacturer_count++;
      total_speeds += (test_dist * 1000 / test_time);
    }
    scan = fscanf(file, "%[^,],%[^,],%fkm,%fs,%f/%f/%f,$%f,%f%%\n",
                  id, car_manufacturer, &test_dist, &test_time, &pitstops, &pitstop_distance, &pitstop_time, &price,
                  &down);
  }

  fclose(file);
  file = NULL;
  if (!match) {
    return NO_DATA_POINTS;
  }
  return total_speeds / manufacturer_count;
} /* average_speed_of_manufacturer() */

/*
 * Function to find the expected number of pitstops for a given car id
 */

int expected_pitstops(char *file_name, char *id) {
  if ((file_name == NULL) || (id == NULL)) {
    return NO_DATA_POINTS;
  }

  FILE *file = fopen(file_name, "r");
  if (file == NULL) {
    return FILE_READ_ERR;
  }

  char car_manufacturer[20] = {""};
  char id2[5] = {""};
  float pitstops = 0;
  float pitstop_distance = 0;
  float pitstop_time = 0;

  float test_dist = 0;
  float test_time = 0;
  float price = 0;
  float down = 0;
  int out = 0;
  int scan = fscanf(file, "%[^,],%[^,],%fkm,%fs,%f/%f/%f,$%f,%f%%\n",
                    id2, car_manufacturer, &test_dist, &test_time, &pitstops, &pitstop_distance, &pitstop_time,
                    &price,
                    &down);

  bool match = false;
  while (scan != EOF) {
    for (int x = 0; x < strlen(id); x++) {
      if ((!isdigit(id[x])) || (x > 5)) { //checks if is a digit and length is at most 5
        fclose(file);
        file = NULL;
        return BAD_ID;
      }
    }
    if (scan != 9) {
      fclose(file);
      file = NULL;
      return BAD_RECORD;
    }

    if (strcmp(id, id2) == 0) {
      match = true;
      out = ceil(RACE_LENGTH / pitstop_distance * pitstops);
    }
    scan = fscanf(file, "%[^,],%[^,],%fkm,%fs,%f/%f/%f,$%f,%f%%\n",
                  id2, car_manufacturer, &test_dist, &test_time, &pitstops, &pitstop_distance, &pitstop_time,
                  &price,
                  &down);
  }

  fclose(file);
  file = NULL;
  if (!match) {
    return NO_DATA_POINTS;
  }
  return out;
} /* expected_pitstops() */

/*
 * Function to find the maximum total pitstop time for a given manufacturer
 */

float find_max_total_pitstop(char *file_name, char *manufacturer) {
  if ((file_name == NULL) || (manufacturer == NULL)) {
    return NO_DATA_POINTS;
  }

  FILE *file = fopen(file_name, "r");
  if (file == NULL) {
    return FILE_READ_ERR;
  }

  float max_total_pitstop = 0;

  char car_manufacturer[20] = {""};
  char id[5] = {""};
  float pitstops = 0;
  float pitstop_distance = 0;
  float pitstop_time = 0;

  float test_dist = 0;
  float test_time = 0;
  float price = 0;
  float down = 0;
  int scan = fscanf(file, "%[^,],%[^,],%fkm,%fs,%f/%f/%f,$%f,%f%%\n",
                    id, car_manufacturer, &test_dist, &test_time, &pitstops, &pitstop_distance, &pitstop_time, &price,
                    &down);

  bool match = false;
  while (scan != EOF) {
    if (scan != 9) {
      fclose(file);
      file = NULL;
      return BAD_RECORD;
    }

    if (strcmp(car_manufacturer, manufacturer) == 0) {
      match = true;
      float total_pitstop = ceil(RACE_LENGTH / pitstop_distance * pitstops) * pitstop_time;
      if (total_pitstop > max_total_pitstop) {
        max_total_pitstop = total_pitstop;
      }
    }
    scan = fscanf(file, "%[^,],%[^,],%fkm,%fs,%f/%f/%f,$%f,%f%%\n",
                  id, car_manufacturer, &test_dist, &test_time, &pitstops, &pitstop_distance, &pitstop_time, &price,
                  &down);
  }

  fclose(file);
  file = NULL;
  if (!match) {
    return NO_DATA_POINTS;
  }
  return max_total_pitstop;
} /* find_max_total_pitstop() */

/*
 * Function that writes out relevent data by reading from a file and writing to another file
 */

int generate_expected_cost_report(char *in_file, char *out_file) {
  if ((in_file == NULL) || (out_file == NULL)) {
    return NO_DATA_POINTS;
  }

  FILE *in = fopen(in_file, "r");
  if (in == NULL) {
    return FILE_READ_ERR;
  }

  FILE *out = fopen(out_file, "w");
  if (out == NULL) {
    fclose(in);
    return FILE_WRITE_ERR;
  }

  fseek(in, 0, SEEK_END);
  int len = ftell(in);
  if (len == 0) {
    fclose(in);
    in = NULL;
    fclose(out);
    out = NULL;
    return NO_DATA_POINTS;
  } else
    fseek(in, 0, SEEK_SET);

  char car_manufacturer[20] = {""};
  char id[5] = {""};
  float pitstops = 0;
  float pitstop_distance = 0;
  float pitstop_time = 0;

  float test_dist = 0;
  float test_time = 0;
  float price = 0;
  float down = 0;

  int scan = fscanf(in, "%[^,],%[^,],%fkm,%fs,%f/%f/%f,$%f,%f%%\n",
                    id, car_manufacturer, &test_dist, &test_time, &pitstops, &pitstop_distance, &pitstop_time, &price,
                    &down);
  while (scan != EOF) {
    if (scan != 9) {
      fclose(in);
      in = NULL;
      fclose(out);
      out = NULL;
      return BAD_RECORD;
    }
    for (int x = 0; x < strlen(id); x++) {
      if (!isdigit(id[x]) || (x > 5)) { //checks if is a digit and length is at most 5
        fclose(in);
        in = NULL;
        fclose(out);
        out = NULL;
        return BAD_ID;
      }
    }

    float cost = price * (down / 100.0);
    fprintf(out, "Car number %s made by %s has a breakdown cost expectancy of $%.2f.\n", id, car_manufacturer,
            cost);
    fflush(out);
    scan = fscanf(in, "%[^,],%[^,],%fkm,%fs,%f/%f/%f,$%f,%f%%\n",
                  id, car_manufacturer, &test_dist, &test_time, &pitstops, &pitstop_distance, &pitstop_time, &price,
                  &down);
  }
  fclose(in);
  in = NULL;
  fclose(out);
  out = NULL;
  return OK;
} /* generate_expected_cost_report() */