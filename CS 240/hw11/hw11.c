//
// Created by ethan on 4/12/2023.
//

#include "hw11.h"

#include <assert.h>
#include <malloc.h>
#include <stdio.h>
#include <string.h>


/*
 * calculates the number of minutes
 */

unsigned long long minutes(time_struct_t time) {
  return time.year * 360 * 24 * 60 +
         time.month * 30 * 24 * 60 +
         time.day * 24 * 60 +
         time.hour * 60 +
         time.minute;
} /* minutes() */

/*
 * Creates an event
 */

void create_event(event_info_t **event, char *name, time_struct_t start,
                  time_struct_t end) {
  assert(event != NULL);
  assert(*event == NULL);
  assert(name != NULL);

  *event = malloc(sizeof(event_info_t));
  assert(*event != NULL);
  (*event)->name = malloc(sizeof(char) * (strlen(name) + 1));
  strcpy((*event)->name, name);
  (*event)->start_time = start;
  (*event)->end_time = end;
} /* create_event() */

/*
 * Adds an event to the binary search tree
 */

void add_event(tree_node_t **root, event_info_t *event) {
  assert(root != NULL);
  assert(event != NULL);
  if (*root == NULL) {
    *root = malloc(sizeof(tree_node_t));
    assert(*root != NULL);
    (*root)->event_info = event;
    (*root)->left_child_ptr = NULL;
    (*root)->right_child_ptr = NULL;
  }
  else {
    int root_minute = minutes((*root)->event_info->start_time);
    int event_minute = minutes(event->start_time);
    if (root_minute >= event_minute) {
      if ((*root)->left_child_ptr == NULL) {
        (*root)->left_child_ptr = malloc(sizeof(tree_node_t));
        assert((*root)->left_child_ptr != NULL);
        (*root)->left_child_ptr->event_info = event;
        (*root)->left_child_ptr->left_child_ptr = NULL;
        (*root)->left_child_ptr->right_child_ptr = NULL;
      }
      else {
        add_event(&(*root)->left_child_ptr, event);
      }
    }
    else {
      if ((*root)->right_child_ptr == NULL) {
        (*root)->right_child_ptr = malloc(sizeof(tree_node_t));
        assert((*root)->right_child_ptr != NULL);
        (*root)->right_child_ptr->event_info = event;
        (*root)->right_child_ptr->left_child_ptr = NULL;
        (*root)->right_child_ptr->right_child_ptr = NULL;
      }
      else {
        add_event(&(*root)->right_child_ptr, event);
      }
    }
  }
} /* add_event() */

/*
 * counts the number of events that have expired
 */

int count_expired_events(tree_node_t *head, time_struct_t time) {
  if (head == NULL) {
    return 0;
  }
  int count = 0;
  int head_minute = minutes(head->event_info->end_time);
  int time_minute = minutes(time);
  if (head_minute < time_minute) {
    count++;
  }
  if (head->left_child_ptr != NULL) {
    count += count_expired_events(head->left_child_ptr, time);
  }
  if (head->right_child_ptr != NULL) {
    count += count_expired_events(head->right_child_ptr, time);
  }
  return count;
} /* count_expired_events() */


/*
 * Adds calendar events to a linked list in order
 */

void add_to_calendar(tree_node_t *event, list_node_t **cal) {
  assert(cal != NULL);
  if (event == NULL) {
    return;
  }
  add_to_calendar(event->left_child_ptr, cal);

  if (*cal == NULL) {
    *cal = malloc(sizeof(list_node_t));
    assert(*cal != NULL);
    (*cal)->event_info = event->event_info;
    (*cal)->next_event = NULL;
    (*cal)->prev_event = NULL;
  }
  while ((*cal)->prev_event != NULL) {
    *cal = (*cal)->prev_event;
  }
  list_node_t *temp = *cal;
  do {
    if ((temp->prev_event == NULL) && (minutes(event->event_info->end_time) <
                                       minutes(
                                           temp->event_info->start_time))) {
      list_node_t *new = malloc(sizeof(list_node_t));
      assert(new != NULL);
      new->event_info = event->event_info;
      new->next_event = temp;
      new->prev_event = NULL;
      temp->prev_event = new;
      *cal = new;
      break;
    }
    else if (temp->next_event != NULL) {
      if ((minutes(event->event_info->start_time) >
           minutes(temp->event_info->end_time))
          && (minutes(event->event_info->end_time) <
              minutes(temp->next_event->event_info->start_time))) {
        list_node_t *new = malloc(sizeof(list_node_t));
        assert(new != NULL);
        new->event_info = event->event_info;
        new->next_event = temp->next_event;
        new->prev_event = temp;
        temp->next_event->prev_event = new;
        temp->next_event = new;
        break;
      }
    }
    else if ((minutes(event->event_info->start_time) >
             minutes(temp->event_info->end_time)) &&
             (temp->next_event == NULL)) {
      list_node_t *new = malloc(sizeof(list_node_t));
      assert(new != NULL);
      new->event_info = event->event_info;
      new->next_event = NULL;
      new->prev_event = temp;
      temp->next_event = new;
      break;
    }
    temp = temp->next_event;
  } while (temp != NULL);

  add_to_calendar(event->right_child_ptr, cal);

  while ((*cal)->prev_event != NULL) {
    *cal = (*cal)->prev_event;
  }
} /* add_to_calendar() */

/*
 * delete every event that starts less than a certain number of minutes
 */

void free_up_schedule(list_node_t **cal, int min) {
  assert(cal != NULL);
  if (*cal == NULL) {
    return;
  }
  list_node_t *temp = *cal;
  if ((temp == NULL) || (temp->next_event == NULL)) {
    return;
  }
  list_node_t *prev = *cal;
  temp = temp->next_event;

  do {
    unsigned long long prev_minute = minutes(prev->event_info->end_time);
    unsigned long long temp_minute = minutes(temp->event_info->start_time);

    if (temp_minute - prev_minute < min) {
      list_node_t *delete = temp;
      temp = delete->next_event;
      prev = delete->prev_event;

      if (delete->next_event != NULL) {
        delete->next_event->prev_event = delete->prev_event;
      }
      if (delete->prev_event != NULL) {
        delete->prev_event->next_event = delete->next_event;
      }
      delete->next_event = NULL;
      delete->prev_event = NULL;

      free(delete);
      delete = NULL;
    }
    else {
      prev = temp;
      temp = temp->next_event;
    }
  } while ((temp != NULL));
} /* free_up_schedule() */

/*
 * Deletes the entire event schedule
 */

void delete_event_schedule(tree_node_t **tree) {
  assert(tree != NULL);
  if (*tree != NULL) {
    if ((*tree)->left_child_ptr != NULL) {
      delete_event_schedule(&(*tree)->left_child_ptr);
    }
    if ((*tree)->right_child_ptr != NULL) {
      delete_event_schedule(&(*tree)->right_child_ptr);
    }
    free((*tree)->event_info->name);
    (*tree)->event_info->name = NULL;
    free((*tree)->event_info);
    (*tree)->event_info = NULL;
    free(*tree);
    *tree = NULL;
  }
  tree = NULL;
} /* delete_event_schedule() */
