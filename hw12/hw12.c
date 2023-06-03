#include "hw12.h"

#include <assert.h>
#include <malloc.h>
#include <stdio.h>
#include <string.h>


/*
 * Function to print the internal data of a app_t struct passed as a void
 * pointer to standard out
 */

void print_app_data(void *p) {
  app_t *ptr = (app_t *) p;
  assert(ptr && ptr->name);

  printf("%s: %dMB, $%.2f, %.2f/5\n", ptr->name, ptr->size,
         ptr->price, ptr->rating);
} /* print_app_data() */

/*
 * Function to print the internal data of a phone_t struct passed as a void
 * pointer to standard out
 */

void print_phone_data(void *p) {
  phone_t *ptr = (phone_t *) p;
  assert(ptr && ptr->model);

  printf("%s: %dGB, released %d, $%.2f, %.2f hours\n", ptr->model,
         ptr->storage, ptr->release_year, ptr->price, ptr->battery_life);
} /* print_phone_data() */

/*
 * Function to print a tree node by dynamically dispatching the print function
 * stored in the node on the data stored in the node
 */

void print_node(node_t *n_ptr) {
  assert(n_ptr && n_ptr->data && n_ptr->print);

  (n_ptr->print)(n_ptr->data);
} /* print_node() */

/*
 * Function to recursively print the tree using an in-order traversal
 */

void print_tree(node_t *root) {
  if (!root) {
    return;
  }

  print_tree(root->left_child);
  print_node(root);
  print_tree(root->right_child);

} /* print_tree() */

/*
 * Functino to create a node_t struct and initialize its data
 */

void create_node(node_t **node, void *data, void (*print)(void *),
                 void (*delete)(void **), int (*compare)(void *, void *)) {
  assert(node != NULL);
  assert(*node == NULL);
  assert(data != NULL);
  assert(print != NULL);
  assert(delete != NULL);
  assert(compare != NULL);
  *node = (node_t *) malloc(sizeof(node_t));
  assert(*node != NULL);
  (*node)->data = data;
  (*node)->print = print;
  (*node)->delete = delete;
  (*node)->compare = compare;
  (*node)->left_child = NULL;
  (*node)->right_child = NULL;
} /* create_node() */

/*
 * Function to recursively delete a single node
 */

void delete_node(node_t **node) {
  assert(node != NULL);
  assert(*node != NULL);
  assert((*node)->left_child == NULL);
  assert((*node)->right_child == NULL);
  (*node)->delete(&(*node)->data);
  free(*node);
  *node = NULL;
} /* delete_node() */

/*
 * Function to insert a node into a tree
 */

void insert_node(node_t **root, node_t *node) {
  assert(root != NULL);
  assert(node != NULL);
  if (*root == NULL) {
    *root = node;
  }
  else {
    if ((*root)->compare((*root)->data, node->data) >= 0) {
      insert_node(&(*root)->left_child, node);
    }
    else {
      insert_node(&(*root)->right_child, node);
    }
  }
} /* insert_node() */

/*
 * Function to recursively delete a tree
 */

int count_nodes(node_t *root, void *data) {
  int count = 0;
  if (root->compare(root->data, data) == 0) {
    count++;
  }
  if (root->left_child != NULL) {
    count += count_nodes(root->left_child, data);
  }
  if (root->right_child != NULL) {
    count += count_nodes(root->right_child, data);
  }
  return count;
} /* count_nodes() */

/*
 * Function help find_nodes to traverse the tree
 */

node_t **traverse(node_t *root, void *data, int count, node_t **out) {
  if (root == NULL) {
    return NULL;
  }
  if (root->compare(root->data, data) == 0) {
    out += count;
    *out = root;
    out -= count;
    count++;
  }
  traverse(root->left_child, data, count, out);
  traverse(root->right_child, data, count, out);
  return out;
} /* traverse() */

/*
 * Function to find all nodes in a tree that match a given data value
 */

struct node **find_nodes(node_t *root, void *data, int *count) {
  assert(root != NULL);
  assert(data != NULL);
  assert(count != NULL);
  *count = count_nodes(root, data);
  if (*count == 0) {
    return NULL;
  }
  node_t **nodes = (node_t **) malloc(sizeof(node_t *) * (*count));
  assert(nodes != NULL);
  nodes = traverse(root, data, 0, nodes);
  return nodes;
} /* find_nodes() */

/*
 * Function to recursively delete a tree
 */

void delete_tree(node_t **root) {
  assert(root != NULL);
  if (*root == NULL) {
    return;
  }
  delete_tree(&((*root)->left_child));
  delete_tree(&((*root)->right_child));
  (*root)->delete(&(*root)->data);
  free(*root);
  *root = NULL;
} /* delete_tree() */

/*
 * Function to recursively delete a single node
 */

void helper(node_t *node, node_t *delete, node_t *previous, int is_right) {
  if (node == NULL) {
    return;
  }
  if (node->compare(node->data, delete->data) == 0) {
    if (delete == node) {
      if ((node->left_child == NULL) && (node->right_child == NULL)) {
        if (is_right) {
          previous->right_child = NULL;
        }
        else {
          previous->left_child = NULL;
        }
        node->right_child = NULL;
        node->left_child = NULL;
        delete_node(&node);
      }
      else if (node->left_child == NULL) {
        if (is_right) {
          previous->right_child = node->right_child;
        }
        else {
          previous->left_child = node->right_child;
        }
        node->right_child = NULL;
        node->left_child = NULL;
        delete_node(&node);
      }
      else if (node->right_child == NULL) {
        if (is_right) {
          previous->right_child = node->left_child;
        }
        else {
          previous->left_child = node->left_child;
        }
        node->right_child = NULL;
        node->left_child = NULL;
        delete_node(&node);
      }
      else {
        node_t *temp = node->left_child;
        if (is_right) {
          previous->right_child = node->left_child;
        }
        else {
          previous->left_child = node->left_child;
        }
        while (temp->right_child != NULL) {
          temp = temp->right_child;
        }
        temp->right_child = node->right_child;
        node->right_child = NULL;
        node->left_child = NULL;
        delete_node(&node);
      }
    }
    else {
      helper(node->left_child, delete, node, 0);
    }
  }
  else if (node->compare(node->data, delete->data) > 0) {
    helper(node->left_child, delete, node, 0);
  }
  else {
    helper(node->right_child, delete, node, 1);
  }
} /* helper() */

/*
 * Function to delete a node from a tree
 */

void remove_node(node_t **root, node_t *remove) {
  assert(root != NULL);
  assert(*root != NULL);
  assert(remove != NULL);
  if (*root == remove) {
    if (((*root)->right_child == NULL) && ((*root)->left_child == NULL)) {
      *root = NULL;
      delete_node(&remove);
    }
    else if ((*root)->left_child == NULL) {
      *root = (*root)->right_child;
      remove->right_child = NULL;
      delete_node(&remove);
    }
    else if ((*root)->right_child == NULL) {
      *root = (*root)->left_child;
      remove->left_child = NULL;
      delete_node(&remove);
    }
    else {
      node_t *temp = remove->left_child;
      *root = temp;
      while (temp->right_child != NULL) {
        temp = temp->right_child;
      }
      temp->right_child = remove->right_child;
      remove->right_child = NULL;
      remove->left_child = NULL;
      delete_node(&remove);
    }
  }
  else {
    helper(*root, remove, NULL, 0);
  }
} /* remove_node() */

/*
 * Function to delete a single node
 */

void create_app_data(void **arg, const char *name, int size, float price,
                     float rating) {
  assert(arg != NULL);
  assert(*arg == NULL);
  assert(name != NULL);
  *arg = (app_t *) malloc(sizeof(app_t));
  assert(*arg != NULL);
  app_t *app = (app_t *) *arg;
  app->name = (char *) malloc(sizeof(char) * (strlen(name) + 1));
  assert(app->name != NULL);
  strcpy(app->name, name);
  app->size = size;
  app->price = price;
  app->rating = rating;
  *arg = app;
} /* create_app_data() */

/*
 * Function to delete app data
 */

void delete_app_data(void **arg) {
  assert(arg != NULL);
  assert(*arg != NULL);
  app_t *app = (app_t *) *arg;
  free(app->name);
  app->name = NULL;
  free(app);
  app = NULL;
} /* delete_app_data() */

/*
 * Function to compare app data
 */

int compare_app_data(void *arg1, void *arg2) {
  assert(arg1 != NULL);
  assert(arg2 != NULL);
  app_t *app1 = (app_t *) arg1;
  app_t *app2 = (app_t *) arg2;
  float rating1 = (app1->rating) / ((float) app1->size * (app1->price + 1));
  float rating2 = (app2->rating) / ((float) app2->size * (app2->price + 1));
  if (rating1 > rating2) {
    return 1;
  }
  else if (rating1 < rating2) {
    return -1;
  }
  else {
    return 0;
  }
} /* compare_app_data() */

/*
 * Function to print app data
 */

void create_phone_data(void **node, const char *model, int storage,
                       int release_year, float price, float battery_life) {
  assert(node != NULL);
  assert(*node == NULL);
  assert(model != NULL);
  *node = (phone_t *) malloc(sizeof(phone_t));
  assert(*node != NULL);
  phone_t *phone = (phone_t *) *node;
  phone->model = (char *) malloc(sizeof(char) * (strlen(model) + 1));
  assert(phone->model != NULL);
  strcpy(phone->model, model);
  phone->storage = storage;
  phone->release_year = release_year;
  phone->price = price;
  phone->battery_life = battery_life;
  *node = phone;
} /* create_phone_data() */

/*
 * Function to delete phone data
 */

void delete_phone_data(void **arg) {
  assert(arg != NULL);
  assert(*arg != NULL);
  phone_t *phone = (phone_t *) *arg;
  free(phone->model);
  phone->model = NULL;
  free(phone);
  phone = NULL;
} /* delete_phone_data() */

/*
 * Function to compare phone data
 */

int compare_phone_data(void *arg1, void *arg2) {
  assert(arg1 != NULL);
  assert(arg2 != NULL);
  phone_t *phone1 = (phone_t *) arg1;
  phone_t *phone2 = (phone_t *) arg2;
  float rating1 = (phone1->battery_life) * ((float) phone1->storage / (
      (phone1->price * ((float) CURRENT_YEAR - (float) phone1->release_year))));
  float rating2 = (phone2->battery_life) * ((float) phone2->storage / (
      (phone2->price * ((float) CURRENT_YEAR - (float) phone2->release_year))));
  if (rating1 > rating2) {
    return 1;
  }
  else if (rating1 < rating2) {
    return -1;
  }
  else {
    return 0;
  }
} /* compare_phone_data() */