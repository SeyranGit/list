#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
#include "list.h"


#define previous_index(index) ((index) == 0 ? 0 : (index) - 1)


struct Node {
  ValueType value;
  struct Node *next;
};


struct List {
  struct Node *first;
  struct Node *last;
  size_t length;
};


static struct Node *get_node(const struct List *list, size_t index) {
  struct Node *node;
  assert(list != NULL);
  assert(index < list->length);
  if (index == (list->length - 1))
    return list->last;
  node = list->first;
  for (size_t i = 0; i != index; ++i)
    node = node->next;
  return node;
}


static const struct Node *get_node_read(const struct List *list, size_t index) {
  return get_node(list, index);
}


static struct Node *create_node(ValueType value) {
  struct Node *node = malloc(sizeof(struct Node));
  if (node != NULL) {
    node->value = value;
    node->next = NULL; 
  }
  return node; 
}


ListStatus list_insert(struct List *list, size_t index, ValueType value) {
  struct Node *previous_node;
  struct Node *node;
  if (list == NULL)
    return LIST_ERROR_NULL;
  if (index > list->length)
    return LIST_ERROR_INDEX;
  if (list->length == SIZE_MAX)
    return LIST_ERROR_OVERFLOW;
  node = create_node(value);
  if (node == NULL)
    return LIST_ERROR_ALLOC;
  if (index == 0) {
    node->next = list->first; 
    list->first = node;
    if (list->length == 0) {
      list->last = node;
    }
  } else {
    previous_node = get_node(list, previous_index(index)); 
    node->next = previous_node->next; 
    previous_node->next = node;
    if (previous_node == list->last) {
      list->last = node;
    }
  } 
  list->length += 1;
  return LIST_OK;
}


ListStatus list_append(struct List *list, ValueType value) {
  if (list == NULL)
    return LIST_ERROR_NULL;
  return list_insert(list, list->length, value);
}


struct RemoveResult list_pop(struct List *list, size_t index) {
  struct Node *previous_node = NULL;
  struct Node *current_node = NULL;
  ValueType value;
  if (list == NULL)
    return (struct RemoveResult){LIST_ERROR_NULL, (ValueType)0};
  if (index >= list->length)
    return (struct RemoveResult){LIST_ERROR_INDEX, (ValueType)0};
  if (index == 0) {
    current_node = list->first;
    list->first = current_node->next;
  } else {
    previous_node = get_node(list, previous_index(index));
    current_node = previous_node->next;
    previous_node->next = current_node->next;
  }
  if (current_node == list->last)
    list->last = previous_node;
  value = current_node->value;
  free(current_node);
  list->length -= 1;
  return (struct RemoveResult){LIST_OK, value};
}


ListStatus list_sget(const struct List *list, size_t index, ValueType *value) {
  if (list == NULL || value == NULL)
    return LIST_ERROR_NULL;
  if (index >= list->length)
    return LIST_ERROR_INDEX; 
  *value = get_node_read(list, index)->value;
  return LIST_OK;
}


// UB
ValueType list_get(const struct List *list, size_t index) {
  return get_node_read(list, index)->value;
}


// UB
size_t list_length(const struct List *list) { 
  return list->length;
}


struct List *list_create(void) {
  struct List *list = malloc(sizeof(struct List));
  if (list == NULL)
    return NULL;
  list->first = NULL;
  list->last = NULL;
  list->length = 0;
  return list;
}


ListStatus list_free(struct List **list, void (*destructor)(ValueType)) {
  struct RemoveResult result;
  if (list == NULL)
    return LIST_ERROR_NULL;
  if (*list == NULL)
    return LIST_ERROR_NULL;
  while ((*list)->length) {
    result = list_pop((*list), 0);
    assert(result.status == LIST_OK);
    if (destructor != NULL) {
      destructor(result.value);
    }
  }
  free(*list);
  *list = NULL;
  return LIST_OK;
}


ListStatus list_print(const struct List *list, FILE *out, void (*print_value)(FILE*, ValueType), char *end) { 
  const struct Node *node;
  if (list == NULL || out == NULL || print_value == NULL)
    return LIST_ERROR_NULL;
  node = list->first;
  fprintf(out, "[");
  while (node) {
    print_value(out, node->value);
    node = node->next;
    if (node) {
      fprintf(out, ", ");
    }
  }
  fprintf(out, "]%s", end);
  return LIST_OK;
}

