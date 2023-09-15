/* Name, hw6.c, CS 24000, Spring 2023
 * Last updated Jan 2, 2023
 */

/* Add any includes here */
#include "hw6.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int g_course_count = 0;
course_t g_course_array[MAX_COURSES] = {0};

/*
 * Function to read in courses
 */

int read_courses(char *filename) {
  assert(filename != NULL);
  FILE *file = fopen(filename, "r");
  if (file == NULL) {
    return NON_READABLE_FILE;
  }
  int scan = 0;
  course_t c = {};

  fseek(file, 0, SEEK_SET);
  fseek(file, 0, SEEK_END);
  int size = ftell(file);
  if (size == 0) {
    fclose(file);
    file = NULL;
    return NO_COURSES;
  }
  fseek(file, 0, SEEK_SET);

  while (feof(file) == 0) {
    char buffer_name[MAX_BUFF_LEN + 1] = {'\0'};
    char buffer_pre[MAX_BUFF_LEN + 1] = {'\0'};
    int type = 0;
    int hours = 0;

    int no_pre = 0;

    int first = ftell(file);
    scan = fscanf(file, "%[^|]|%[^|]|%d|%d\n",
                  buffer_name, buffer_pre, &type, &hours);

    if (scan != 4) {
      fseek(file, 0 - (ftell(file) - first), SEEK_CUR);
      scan = fscanf(file, "%[^|]||%d|%d\n",
                    buffer_name, &type, &hours);
      no_pre = 1;
      scan++;
    }
    if ((strlen(buffer_name) >= 200) || (strlen(buffer_pre) >= 200)) {
      fclose(file);
      file = NULL;
      return BAD_RECORD;
    }
    if ((no_pre == 0) && (buffer_pre[0] != '?')) {
      fclose(file);
      file = NULL;
      return BAD_RECORD;
    }

    if (scan != 4) {
      fclose(file);
      file = NULL;
      return BAD_RECORD;
    }

    strncpy(c.course_name, buffer_name, 79);
    c.course_name[79] = '\0';

    if (no_pre == 0) {
      char *tok = NULL;
      int preq_count = 0;
      tok = strtok(buffer_pre, "?");
      while (tok != NULL) {
        if (preq_count >= MAX_PREREQ) {
          fclose(file);
          file = NULL;
          return BAD_RECORD;
        }
        strncpy(c.prerequisites[preq_count], tok, 79);
        tok = strtok(NULL, "?");
        preq_count++;
      }
      for (int i = preq_count; i < MAX_PREREQ; i++) {
        strcpy(c.prerequisites[i], "");
      }
    }
    else {
      for (int i = 0; i < MAX_PREREQ; i++) {
        strcpy(c.prerequisites[i], "");
      }
    }

    c.course_type = type;
    c.credit_hours = hours;

    g_course_array[g_course_count] = c;

    g_course_count++;
    if (g_course_count > MAX_COURSES) {
      fclose(file);
      file = NULL;
      return TOO_MUCH_DATA;
    }
  }
  fclose(file);
  file = NULL;
  return g_course_count;
} /* read_courses() */

/*
 * Function to find the highest credit hour course of a given type
 */

int find_prerequisites(char *name, enum course_type_t type) {
  assert(name != NULL);
  assert(type >= LECTURE);
  assert(type <= INDEPENDENT);
  if (g_course_count == 0) {
    return NO_COURSES;
  }

  printf("input name: %s\n", name);
  printf("input type: %d\n", type);

  int course_found = 0;
  int max = -1;
  for (int i = 0; i < g_course_count; i++) {
    if (strstr(g_course_array[i].course_name, name)) {

      printf("course found: %s\n", g_course_array[i].course_name);

      course_found = 1;
      course_t course = g_course_array[i];
      int pre_num = 0;
      for (int j = 0; j < MAX_PREREQ; j++) {
        char *pre = course.prerequisites[j];
        if (strcmp(pre, "") != 0) {
          pre_num++;
          printf("pre: %s\n", pre);
        }
      }
      course_t pre_array[3] = {0};
      int pre_exists = 0;
      for (int j = 0; j < pre_num; j++) {
        char *pre = course.prerequisites[j];
        for (int k = 0; k < g_course_count; k++) {
          if (strstr(g_course_array[k].course_name, pre)) {
            pre_array[j] = g_course_array[k];
            pre_exists++;
            printf("pre found %s\n", pre_array[j].course_name);
          }
        }
      }

      if (pre_num == 0) {
        return 0;
      }

      if (pre_num != pre_exists) {
        printf("pre not founqwdqdqwdqwdqwd\n");
        return NOT_FOUND;
      }

      for (int j = 0; j < pre_num; j++) {
        if (pre_array[j].course_type == type) {
          if (pre_array[j].credit_hours > max) {
            max = pre_array[j].credit_hours;
          }
        }
      }
    }
  }

  if (course_found == 0) {
    return NOT_FOUND;
  }

  if (max == -1) {
    return 0;
  }

  return max;
} /* find_prerequisites() */


