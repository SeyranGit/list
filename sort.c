#include <stdlib.h>
#include "list.h"


void swap(struct List *list, size_t i) {
  struct RemoveResult res;
  res = list_pop(list, i);
  if (res.status != LIST_OK)
    abort();
  list_insert(list, i + 1, res.value);
}


void sort(struct List *list, int (*cmp)(ValueType, ValueType)) {
  size_t len = list_length(list);
  for (size_t j = 0; j < (len - 1); ++j) {
    for (size_t i = 0; i < (len - 1); ++i) {
      ValueType v0 = list_get(list, i);  
      ValueType v1 = list_get(list, i + 1);
      if (cmp(v0, v1)) {
        swap(list, i);
      }
    }
  }
}

