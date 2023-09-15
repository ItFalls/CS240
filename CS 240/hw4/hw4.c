/* Ethan Ling, hw4.c, CS 24000, Spring 2023
 * Last updated Feb 8, 2022
 */

/* Add any includes here */
#include "hw4.h"
#include <math.h>

/* Define your functions here */

/*
 * adds two complex numbers together
 */

complex_t add_complex(complex_t arg1, complex_t arg2) {
  complex_t result = {0.0, 0.0};
  result.x = arg1.x + arg2.x;
  result.y = arg1.y + arg2.y;
  return result;
} /* add_complex() */

/*
 * negates a complex number
 */

complex_t neg_complex(complex_t arg) {
  arg.x = -arg.x;
  arg.y = -arg.y;
  return arg;
} /* neg_complex() */

/*
 * subtracts two complex numbers
 */

complex_t sub_complex(complex_t arg1, complex_t arg2) {
  return add_complex(arg1, neg_complex(arg2));
} /* sub_complex() */

/*
 * calculates the dot product of two complex numbers
 */

double dot_complex(complex_t arg1, complex_t arg2) {
  return (arg1.x * arg2.x) + (arg1.y * arg2.y);
} /* dot_complex() */

/*
 * returns the reciprocal of a complex number
 */

complex_t inv_complex(complex_t arg) {
  double denominator = dot_complex(arg, arg);
  complex_t result = {0.0, 0.0};
  result.x = arg.x / denominator;
  result.y = -arg.y / denominator;
  return result;
} /* inv_complex() */

/*
 * returns the complex product of two complex arguments
 */

complex_t mul_complex(complex_t arg1, complex_t arg2) {
  complex_t result = {0.0, 0.0};
  result.x = (arg1.x * arg2.x) - (arg1.y * arg2.y);
  result.y = (arg1.x * arg2.y) + (arg1.y * arg2.x);
  return result;
} /* mul_complex() */

/*
 * returns the complex division of two complex arguments
 */

complex_t div_complex(complex_t arg1, complex_t arg2) {
  return mul_complex(arg1, inv_complex(arg2));
} /* div_complex() */

/*
 * returns the complex exponential of a complex argument
 */

complex_t exp_complex(complex_t arg) {
  complex_t result = {0.0, 0.0};
  result.x = exp(arg.x) * cos(arg.y);
  result.y = exp(arg.x) * sin(arg.y);
  return result;
} /* exp_complex() */

/*
 * returns the number of iterations of the mandelbrot function
 */

int mandelbrot(complex_t arg) {
  complex_t z = {0.0, 0.0};
  for (int x = 0; x < MAX_MANDELBROT; x++) {
    z = add_complex(mul_complex(z, z), arg);
    if (dot_complex(z, z) > 4.0) {
      return x + 1;
    }

  }
  return MAX_MANDELBROT;
} /* mandelbrot() */