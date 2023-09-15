/* Name, hw6.c, CS 24000, Spring 2023
 * Last updated Jan 2, 2023
 */

/* Add any includes here */
#include "hw7.h"

#include <assert.h>
#include <stdio.h>
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
    for (int i = 0; i < MAX_PREREQ; i++) {
      c.prerequisite_course_ptrs[i] = NULL;
    }

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
 * Function to link courses together
 */

void link_courses() {
  for (int i = 0; i < g_course_count; i++) {
    course_t *course = &g_course_array[i];
    for (int j = 0; j < MAX_PREREQ; j++) {
      char *pre = course->prerequisites[j];
      if (strcmp(pre, "") != 0) {
        for (int k = 0; k < g_course_count; k++) {
          if (strncmp(pre, g_course_array[k].course_name, strlen(pre)) == 0) {
            course->prerequisite_course_ptrs[j] = &g_course_array[k];
          }
        }
      }
    }
  }
} /* link_courses() */

/*
 * Function to recurse through the prerequisites
 */

int recurse(int count, int index, course_t c) {
  if (c.prerequisite_course_ptrs[0] == NULL) {
    return count;
  }
  else {
    if (c.prerequisite_course_ptrs[index] != NULL) {
      count++;
      count = recurse(count, index, *c.prerequisite_course_ptrs[index]);
    }
  }
  return count;
} /* recurse() */

/*
 * Finds the longest chain of prerequisites for the course specified
 */

int find_longest_chain(char *course_code) {
  assert(course_code != NULL);
  int max = -1;
  for (int i = 0; i < g_course_count; i++) {
    if (strstr(g_course_array[i].course_name, course_code)) {
      course_t course = g_course_array[i];
      for (int j = 0; j < MAX_PREREQ; j++) {
        int x = recurse(0, j, course);
        if (x > max) {
          max = x;
        }
      }
    }
  }
  if (max == -1) {
    return NO_COURSES;
  }
  return max;
} /* find_longest_chain() */

/*
 * Finds the maximum number of credit hours for the course chain specified
 */

int find_max_hours_chain(int index) {
  assert(index >= 0);
  assert(index < g_course_count);
  int max = 0;
  for (int x = 0; x < MAX_PREREQ; x++) {
    if (g_course_array[index].prerequisite_course_ptrs[x] != NULL) {
      course_t course = *g_course_array[index].prerequisite_course_ptrs[x];
      int hours = course.credit_hours;
      while (course.prerequisite_course_ptrs[x] != NULL) {
        printf("course: %s, hours: %d, total_hours: %d\n", course.course_name,
               course.credit_hours, hours);
        course = *course.prerequisite_course_ptrs[x];
        hours += course.credit_hours;
      }
      if (hours > max) {
        max = hours;
      }
    }
  }
  return max;
} /* find_max_hours_chain() */