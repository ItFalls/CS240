/* Name, hw9.c, CS24000, Spring 2023
 * Last updated Feb 11, 2023
 */

/* Add any includes here */

#include "hw9.h"

#include <assert.h>
#include <limits.h>
#include <malloc.h>
#include <string.h>

/* Define your global variables here */

/* Define your functions here */

/*
 * This function should create a collection.
 */

void create_collection(char *name, collection_t **collection) {
  assert(name != NULL);
  assert(collection != NULL);
  assert(*collection == NULL);
  *collection = (collection_t *) malloc(sizeof(collection_t));
  assert(*collection != NULL);
  (*collection)->collection_name = (char *) malloc(
      sizeof(char) * (strlen(name) + 1));
  assert((*collection)->collection_name != NULL);
  strcpy((*collection)->collection_name, name);
  (*collection)->book_count = 0;
  (*collection)->books = NULL;
  (*collection)->prev_collection = NULL;
  (*collection)->next_collection = NULL;
} /* create_collection() */

/*
 * This function should add a book to the collection.
 */

void add_book(collection_t *collection, char *title, char *author,
              enum genre_t genre, enum condition_t condition) {
  assert(collection != NULL);
  assert(title != NULL);
  assert(author != NULL);
  assert(genre >= NOVEL);
  assert(genre <= ADVENTURE);
  assert(condition >= NEW);
  assert(condition <= POOR);
  book_t *new_book = malloc(sizeof(book_t));
  assert(new_book != NULL);
  new_book->title = malloc(sizeof(char) * (strlen(title) + 1));
  assert(new_book->title != NULL);
  strcpy(new_book->title, title);
  new_book->author = malloc(sizeof(char) * (strlen(author) + 1));
  assert(new_book->author != NULL);
  strcpy(new_book->author, author);
  new_book->genre = genre;
  new_book->condition = condition;
  new_book->check_out_count = 0;
  new_book->prev_book = NULL;
  if (collection->books != NULL) {
    new_book->next_book = collection->books;
    new_book->next_book->prev_book = new_book;
  }
  else {
    new_book->next_book = NULL;
  }
  collection->books = new_book;
  collection->book_count++;
} /* add_book() */

/*
 * This function should add a collection to the list.
 */

void add_collection(collection_t **list, collection_t *collection) {
  assert(list != NULL);
  assert(collection != NULL);
  while (((*list)->prev_collection) != NULL) {
    *list = (*list)->prev_collection;
  }
  collection_t *temp = *list;
  while (temp->next_collection != NULL) {
    if (strcmp(temp->collection_name, collection->collection_name) > 0) {
      if (temp == *list) {
        temp->prev_collection = collection;
        collection->next_collection = temp;
        *list = collection;
      }
      else if ((temp->prev_collection != NULL) &&
               (temp->next_collection != NULL)) {
        collection->prev_collection = temp->prev_collection;
        temp->prev_collection->next_collection = collection;
        temp->prev_collection = collection;
        collection->next_collection = temp;
      }
      return;
    }
    temp = temp->next_collection;
  }
  if (temp->next_collection == NULL) {
    if (strcmp(temp->collection_name, collection->collection_name) > 0) {
      collection->prev_collection = temp->prev_collection;
      temp->prev_collection->next_collection = collection;
      temp->prev_collection = collection;
      collection->next_collection = temp;
    }
    else {
      temp->next_collection = collection;
      collection->prev_collection = temp;
      collection->next_collection = NULL;
    }
  }
} /* add_collection() */

/*
 * This function should delete a collection from the list.
 */

void delete_collection(collection_t **collection) {
  assert(collection != NULL);
  assert(*collection != NULL);
  collection_t *remove = *collection;
  if ((remove->next_collection != NULL) &&
      (remove->prev_collection != NULL)) {
    remove->prev_collection->next_collection = remove->next_collection;
    remove->next_collection->prev_collection = remove->prev_collection;
  }
  else if (remove->next_collection != NULL) { //prev is null
    remove->next_collection->prev_collection = NULL;
    *collection = remove->next_collection;
  }
  else if (remove->prev_collection != NULL) { //next is null
    remove->prev_collection->next_collection = NULL;
  }
  else {
    *collection = NULL;
  }
  if (*collection != NULL) {
    while (((*collection)->prev_collection) != NULL) {
      *collection = (*collection)->prev_collection;
    }
  }
  book_t *book = remove->books;
  while (book != NULL) {
    book_t *temp = book;
    book = book->next_book;
    free(temp->title);
    free(temp->author);
    free(temp);
  }
  free(remove->collection_name);
  free(remove);
} /* delete_collection() */

/*
 * This function should move a book from one collection to another.
 */

int move_book(collection_t *collection, char *book_name,
              char *collection_name) {
  assert(collection != NULL);
  assert(book_name != NULL);
  assert(collection_name != NULL);
  while ((collection->prev_collection) != NULL) {
    collection = collection->prev_collection;
  }

  collection_t *destination = collection;
  while (destination != NULL) {
    if (strcmp(destination->collection_name, collection_name) == 0) {
      break;
    }
    destination = destination->next_collection;
  }
  if (destination == NULL) {
    return NO_SUCH_COLLECTION;
  }

  collection_t *old = collection;
  book_t *move = NULL;
  int found = 0;
  while (1) {
    move = old->books;
    if (move == NULL) {
      if (old->next_collection == NULL) {
        break;
      }
      old = old->next_collection;
      continue;
    }
    while (1) {
      if (move == NULL) {
        break;
      }
      if (strcmp(move->title, book_name) == 0) {
        if (move->next_book != NULL) {
          move->next_book->prev_book = move->prev_book;
        }
        if (move->prev_book != NULL) {
          move->prev_book->next_book = move->next_book;
        }
        else {
          old->books->prev_book = NULL;
          old->books = move->next_book;
        }
        old->book_count--;
        move->next_book = NULL;
        move->prev_book = NULL;
        found = 1;
        break;
      }
      if (move->next_book == NULL) {
        break;
      }
      move = move->next_book;
    }
    if ((old->next_collection == NULL) || (found == 1)) {
      break;
    }
    old = old->next_collection;
  }

  if (found == 0) {
    return NO_SUCH_BOOK;
  }

  move->prev_book = NULL;
  move->next_book = destination->books;
  if (move->next_book != NULL) {
    move->next_book->prev_book = move;
  }
  destination->book_count++;
  destination->books = move;
  return OK;
} /* move_book() */

/*
 * This function should alphabetize the books in the collection.
 */

void alphabetize_books(book_t **list) {
  assert(list != NULL);
  assert(*list != NULL);
  while (((*list)->prev_book) != NULL) {
    *list = (*list)->prev_book;
  }
  book_t *temp = *list;
  while (temp->next_book != NULL) {
    if (strcmp(temp->title, temp->next_book->title) > 0) {
      book_t *swap = temp->next_book;
      if (temp->prev_book != NULL) {
        temp->prev_book->next_book = swap;
      }
      else {
        *list = swap;
      }
      if (swap->next_book != NULL) {
        swap->next_book->prev_book = temp;
      }
      temp->next_book = swap->next_book;
      swap->prev_book = temp->prev_book;
      temp->prev_book = swap;
      swap->next_book = temp;
      temp = *list;
    }
    else {
      temp = temp->next_book;
    }
  }
} /* alphabetize_books() */

/*
 * Gets the total number of checkouts for a given condition.
 */

int count_total_checkouts(collection_t *collection,
                          enum condition_t condition) {
  assert(collection != NULL);
  assert(condition >= NEW);
  assert(condition <= POOR);
  int total = 0;
  book_t *temp = collection->books;
  while (temp != NULL) {
    if (temp->condition == condition) {
      total += temp->check_out_count;
    }
    temp = temp->next_book;
  }
  return total;
} /* count_total_checkouts() */

/*
 * Gets the most common genre in the collection.
 */

enum genre_t most_common_genre(collection_t *collection) {
  assert(collection != NULL);
  if (collection->books == NULL) {
    return NO_SUCH_BOOK;
  }
  int novel = 0;
  int narrative = 0;
  int sci_fi = 0;
  int nonfiction = 0;
  int adventure = 0;
  int max = -1;
  book_t *temp2 = collection->books;
  while (temp2 != NULL) {
    if (temp2->genre == NOVEL) {
      novel++;
      if (novel > max) {
        max = novel;
      }
    }
    else if (temp2->genre == NARRATIVE) {
      narrative++;
      if (narrative > max) {
        max = narrative;
      }
    }
    else if (temp2->genre == SCI_FI) {
      sci_fi++;
      if (sci_fi > max) {
        max = sci_fi;
      }
    }
    else if (temp2->genre == NON_FICTION) {
      nonfiction++;
      if (nonfiction > max) {
        max = nonfiction;
      }
    }
    else if (temp2->genre == ADVENTURE) {
      adventure++;
      if (adventure > max) {
        max = adventure;
      }
    }
    temp2 = temp2->next_book;
  }
  if (max == novel) {
    return NOVEL;
  }
  else if (max == narrative) {
    return NARRATIVE;
  }
  else if (max == sci_fi) {
    return SCI_FI;
  }
  else if (max == nonfiction) {
    return NON_FICTION;
  }
  else {
    return ADVENTURE;
  }
} /* most_common_genre() */

/*
 * Gets the least common genre in the collection.
 */

int decommission_book(collection_t *collection) {
  assert(collection != NULL);
  if (collection->books == NULL) {
    return NO_SUCH_BOOK;
  }
  book_t *temp = collection->books;
  int min_count = INT_MAX;
  book_t *min_book = NULL;

  while (temp != NULL) {
    if (temp->check_out_count < min_count) {
      min_count = temp->check_out_count;
      min_book = temp;
    }
    temp = temp->next_book;
  }
  if (min_book->prev_book != NULL) {
    min_book->prev_book->next_book = min_book->next_book;
  }
  if (min_book->next_book != NULL) {
    min_book->next_book->prev_book = min_book->prev_book;
  }
  if (min_book == collection->books) {
    collection->books = min_book->next_book;
  }
  free(min_book->title);
  free(min_book->author);
  free(min_book);
  collection->book_count--;
  return min_count;
} /* decommission_book() */

/*
 * Gets the least common genre in the collection.
 */

void delete_collections(collection_t **collection) {
  assert(collection != NULL);
  assert(*collection != NULL);
  while (*collection != NULL) {
    collection_t *remove = *collection;
    if ((remove->next_collection != NULL) &&
        (remove->prev_collection != NULL)) {
      remove->prev_collection->next_collection = remove->next_collection;
      remove->next_collection->prev_collection = remove->prev_collection;
    }
    else if (remove->next_collection != NULL) { //prev is null
      remove->next_collection->prev_collection = NULL;
      *collection = remove->next_collection;
    }
    else if (remove->prev_collection != NULL) { //next is null
      remove->prev_collection->next_collection = NULL;
    }
    else {
      *collection = NULL;
    }
    if (*collection != NULL) {
      while (((*collection)->prev_collection) != NULL) {
        *collection = (*collection)->prev_collection;
      }
    }
    book_t *book = remove->books;
    while (book != NULL) {
      book_t *temp = book;
      book = book->next_book;
      free(temp->title);
      free(temp->author);
      free(temp);
    }
    free(remove->collection_name);
    free(remove);
  }
} /* delete_collections() */