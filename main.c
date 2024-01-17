#include "list.h"
#include <stdio.h>

MAKE_LIST_OF(int);

int main(void) {
    LinkedList_int_t ll = LinkedList_int_make();
    LinkedList_int_append(12, ll);
    int fst_elem;
    printf("%d\n", LinkedList_int_get(0, ll, &fst_elem));
    printf("%d", fst_elem);
    LinkedList_int_free(ll);
}