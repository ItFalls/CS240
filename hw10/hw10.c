#include "hw10.h"

#include <assert.h>
#include <malloc.h>
#include <string.h>

/*
 * creates an employee to be used
 */

employee_t *create_employee(char *name) {
  assert(name != NULL);
  employee_t *new_employee = malloc(sizeof(employee_t));
  assert(new_employee != NULL);
  new_employee->name = malloc(strlen(name) + 1);
  assert(new_employee->name != NULL);
  strcpy(new_employee->name, name);
  new_employee->left_child_ptr = NULL;
  new_employee->right_child_ptr = NULL;
  return new_employee;
} /* create_employee() */

/*
 * inserts an employee into the tree
 */

void insert_employee(employee_t **root, employee_t *new) {
  assert(root != NULL);
  assert(new != NULL);
  assert(new->left_child_ptr == NULL);
  assert(new->right_child_ptr == NULL);
  if (*root == NULL) {
    *root = new;
    return;
  }
  if (strcmp(new->name, (*root)->name) < 0) {
    insert_employee(&(*root)->left_child_ptr, new);
  }
  else {
    insert_employee(&(*root)->right_child_ptr, new);
  }
} /* insert_employee() */

/*
 * finds an employee based on the name
 */

employee_t *find_employee(employee_t *root, char *name) {
  assert(name != NULL);
  if (root == NULL) {
    return NULL;
  }
  if (strcmp(name, root->name) == 0) {
    return root;
  }
  if (strcmp(name, root->name) < 0) {
    return find_employee(root->left_child_ptr, name);
  }
  else {
    return find_employee(root->right_child_ptr, name);
  }
} /* find_employee() */

/*
 * deletes the entire tree and frees
 */

void delete_tree(employee_t **root) {
  assert(root != NULL);
  if (*root == NULL) {
    return;
  }
  delete_tree(&(*root)->left_child_ptr);
  delete_tree(&(*root)->right_child_ptr);
  free((*root)->name);
  (*root)->name = NULL;
  free(*root);
  *root = NULL;
} /* delete_tree() */

/*
 * helper function for traverse based on the given order
 */

employee_t *recurse(employee_t *node, int order, int *count) {
  if (node != NULL) {
    employee_t *out = NULL;
    switch (order) {
      case PREFIX:
        if (*count == 0) {
          return node;
        }
        (*count)--;
        out = recurse(node->left_child_ptr, order, count);
        if (out != NULL) {
          return out;
        }
        out = recurse(node->right_child_ptr, order, count);
        if (out != NULL) {
          return out;
        }
        break;
      case POSTFIX:
        out = recurse(node->left_child_ptr, order, count);
        if (out == NULL) {
          out = recurse(node->right_child_ptr, order, count);
        }
        if (out != NULL) {
          return out;
        }
        if (*count == 0) {
          return node;
        }
        (*count)--;
        break;
      case INORDER:
        out = recurse(node->left_child_ptr, order, count);
        if (out != NULL) {
          return out;
        }
        if (*count == 0) {
          return node;
        }
        (*count)--;
        out = recurse(node->right_child_ptr, order, count);
        if (out != NULL) {
          return out;
        }
        break;
      case REVERSE:
        out = recurse(node->right_child_ptr, order, count);
        if (out != NULL) {
          return out;
        }
        if (*count == 0) {
          return node;
        }
        (*count)--;
        out = recurse(node->left_child_ptr, order, count);
        if (out != NULL) {
          return out;
        }
        break;
    }
  }
  return NULL;
} /* recurse() */

/*
 * traverses the employees based on the given order
 */

employee_t *traverse_employees(employee_t *root, int order, int k) {
  assert(order >= PREFIX);
  assert(order <= REVERSE);
  assert(k >= 0);
  if (root == NULL) {
    return NULL;
  }
  int *count = malloc(sizeof(int));
  assert(count != NULL);
  *count = k;
  return recurse(root, order, count);
} /* traverse_employees() */

/*
 * find the employee with the previous in name
 */

employee_t *previous_employee(employee_t *root, char *name) {
  assert(name != NULL);
  if (root == NULL) {
    return NULL;
  }
  int k = 0;
  employee_t *prev = NULL;
  while (1 == 1) {
    employee_t *temp = traverse_employees(root, 3, k);
    if (strcmp(temp->name, name) == 0) {
      return prev;
    }
    prev = temp;
    k++;
  }
  return NULL;
} /* previous_employee() */