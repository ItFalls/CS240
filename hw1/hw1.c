/* Ethan Ling, hw1.c, CS 24000, Spring 2023
 * Last updated Jan 24, 2023
 */

/* Add any includes here */
#include "hw1.h"

/*
 * Function to check if number is in array
 */

int check(int num, int lim) {
  for (int x = 0; x < lim; x++) {
    if (g_recaman_array[x] == num) {
      return 0;
    }
  }
  return 1;
} /* check() */

/* 
 * Creates the reecaman sequence using the first element and number of elements
 */

int create_recaman_sequence(int first, int num) {
  if ((num <= 0) || (num > ARRAY_SIZE) || (first < 0) ||
      (g_recaman_array[0] < 0)) {
    return RECAMAN_ERROR;
  }
  g_recaman_array[0] = first;
  for (int x = 1; x < num; x++) {
    int sub = g_recaman_array[x - 1] - x;
    if ((sub > 0) && (check(sub, x) == 1)) {
      g_recaman_array[x] = sub;
    } else {
      g_recaman_array[x] = g_recaman_array[x - 1] + x;
    }
  }
  return RECAMAN_CORRECT;
} /* create_recaman_sequence() */

/* 
 * Checks the array to see if the numbers form a recaman sequence up to a certain point 
 */

int check_recaman_sequence(int num) {
  if ((num <= 0) || (num > ARRAY_SIZE)) {
    return RECAMAN_ERROR;
  }

  if(g_recaman_array[0] < 0) {
    return 0;
  }
  for (int x = 1; x < num; x++) {
    int sub = g_recaman_array[x - 1] - x;
    if ((sub > 0) && (check(sub, x) == 1)) {
      if(g_recaman_array[x] != sub) {
        return x;
      }
    } else {
      if(g_recaman_array[x] != g_recaman_array[x - 1] + x){
        return x;
      }
    }
  }
  return RECAMAN_CORRECT;
} /* check_recaman_sequence() */

/* Remember, you don't need a main function!
 * It is provided by hw1_main.c or hw1_test.o
 */
