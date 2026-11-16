#ifndef LIST_H
#define LIST_H


#include <stdio.h>
#include <stddef.h>



#define VOID_PTR 1


#if VOID_PTR
typedef void *ValueType;
#else
typedef int ValueType;
#endif


typedef enum {
  LIST_OK = 0,
  LIST_ERROR_NULL,
  LIST_ERROR_INDEX,
  LIST_ERROR_ALLOC,
  LIST_ERROR_OVERFLOW
} ListStatus;


struct RemoveResult {
  ListStatus status;
  ValueType value;
};


struct List;


struct List *list_create(void);
struct RemoveResult list_pop(struct List*, size_t);
size_t list_length(const struct List*);
ValueType list_get(const struct List*, size_t);

ListStatus list_sget(const struct List*, size_t, ValueType*);
ListStatus list_append(struct List*, ValueType);
ListStatus list_insert(struct List*, size_t, ValueType);
ListStatus list_free(struct List*, void (*)(ValueType));
ListStatus list_print(const struct List*, FILE*, void (*)(FILE*, ValueType), char *end);


#endif

