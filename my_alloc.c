#include "my_alloc.h"

char heap[HEAP_CAP] = {0};

piece_list alloced = {0};
piece_list freed = {
    .count = 1,
    .pieces[0] = {.ptr = heap, .size = sizeof(heap)},
};

void piece_list_print(const piece_list *list) {
  printf("Piece_list cap: %lu \n", list->count);
  for (size_t i = 0; i < list->count; ++i) {
    printf("%p : size = %lu\n", list->pieces[i].ptr, list->pieces[i].size);
  }
  puts("\n");
}

void piece_list_insert(piece_list *list, void *ptr, size_t size) {
  assert(list->count < LIST_CAP);
  list->pieces[list->count].ptr = ptr;
  list->pieces[list->count].size = size;

  for (size_t i = list->count;
       i > 0 && list->pieces[i].ptr < list->pieces[i - 1].ptr; i--) {
    const piece tmp = list->pieces[i];
    list->pieces[i] = list->pieces[i - 1];
    list->pieces[i - 1] = tmp;
  }
  list->count++;
}

int piece_list_find(const piece_list *list, void *ptr) {
  for (size_t i = 0; i < list->count; ++i) {
    if (list->pieces[i].ptr == ptr) {
      return i;
    }
  }
  return -1;
}

void piece_list_remove(piece_list *list, size_t index) {
  assert(index < list->count);
  for (size_t i = index; i < list->count - 1; ++i) {
    list->pieces[i] = list->pieces[i + 1];
  }
  list->count -= 1;
}

void piece_list_merge(piece_list *list) {
  for (size_t i = 0; i < list->count - 1; ++i) {
    if (list->pieces[i].ptr + list->pieces[i].size == list->pieces[i + 1].ptr) {
      list->pieces[i].size += list->pieces[i + 1].size;
      piece_list_remove(list, i + 1);
    }
  }
}

void *my_ffalloc(size_t size) {
  if (size > 0) {
    for (size_t i = 0; i < freed.count; ++i) {
      if (freed.pieces[i].size > size) {
        void *tmp = (void *)freed.pieces[i].ptr;
        piece_list_insert(&alloced, tmp, size);
        freed.pieces[i].ptr = (char *)tmp + (int)size;
        freed.pieces[i].size -= size;
        return tmp;
      }
    }
  }
  return NULL;
}

void *my_bfalloc(const size_t size) {
  char found_flag = 0;
  size_t pick = 0;
  if (size > 0) {
    size_t dsize = size;
    for (size_t i = 0; i < freed.count; ++i) {
      if (freed.pieces[i].size - size < dsize) {
        pick = i;
        dsize = freed.pieces[i].size - size;
        found_flag = 1;
      }
    }
  }
  if (found_flag) {
    void *tmp = (void *)freed.pieces[pick].ptr;
    piece_list_insert(&alloced, tmp, size);
    if (size == freed.pieces[pick].size) {
      piece_list_remove(&freed, pick);
    } else {
      freed.pieces[pick].ptr = (char *)tmp + (int)size;
      freed.pieces[pick].size -= size;
    }

    return tmp;
  }
  return NULL;
}

void *my_wfalloc(const size_t size) {
  char found_flag = 0;
  size_t pick = 0;
  if (size > 0) {
    size_t dsize = 0;
    for (size_t i = 0; i < freed.count; ++i) {
      if (freed.pieces[i].size > dsize) {
        pick = i;
        dsize = freed.pieces[i].size;
        found_flag = 1;
      }
    }
  }
  if (found_flag) {
    void *tmp = (void *)freed.pieces[pick].ptr;
    piece_list_insert(&alloced, tmp, size);
    if (size == freed.pieces[pick].size) {
      piece_list_remove(&freed, pick);
    } else {
      freed.pieces[pick].ptr = (char *)tmp + (int)size;
      freed.pieces[pick].size -= size;
    }

    return tmp;
  }
  return NULL;
}

void my_free(void *ptr) {
  if (ptr != NULL) {
    const int i = piece_list_find(&alloced, ptr);
    assert(i >= 0);
    piece_list_insert(&freed, alloced.pieces[i].ptr, alloced.pieces[i].size);
    piece_list_remove(&alloced, (size_t)i);
    piece_list_merge(&freed);
  }
}
