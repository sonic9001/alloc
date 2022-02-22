#include "my_alloc.h"

int main(void) {
  void *ptr0 = my_ffalloc(20);
  void *ptr1 = my_ffalloc(50);
  void *ptr2 = my_ffalloc(19);
  void *ptr3 = my_ffalloc(50);

  piece_list_print(&alloced);
  piece_list_print(&freed);

  my_free(ptr0);
  my_free(ptr2);
  piece_list_print(&alloced);
  piece_list_print(&freed);

  ptr2 = my_bfalloc(19);

  piece_list_print(&alloced);
  piece_list_print(&freed);
  void *ptr4 = my_wfalloc(20);

  piece_list_print(&alloced);
  piece_list_print(&freed);

  return 0;
}