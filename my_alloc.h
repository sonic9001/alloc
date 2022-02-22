#ifndef MYALLOC_H_
#define MYALLOC_H_

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#define HEAP_CAP 512000
#define LIST_CAP 64000

#define TODO                                                                   \
  do {                                                                         \
    fprintf(stderr, "%s:%d: %s is not implemented \n", __FILE__, __LINE__,     \
            __func__);                                                         \
    abort();                                                                   \
  } while (0)

typedef struct {
  char *ptr;
  size_t size;
} piece;

typedef struct {
  size_t count;
  piece pieces[LIST_CAP];
} piece_list;

extern piece_list alloced;
extern piece_list freed;

void piece_list_print(const piece_list *list);
void piece_list_insert(piece_list *list, void *ptr, size_t size);
int piece_list_find(const piece_list *list, void *ptr);
void piece_list_remove(piece_list *list, size_t index);
void piece_list_merge(piece_list *list);
void *my_ffalloc(size_t size);
void *my_bfalloc(size_t size);
void *my_wfalloc(size_t size);
void my_free(void *ptr);

#endif