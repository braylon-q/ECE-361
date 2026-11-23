#pragma once
#include <stddef.h>
#include <stdbool.h>

typedef struct bst bst_t; /* opaque */
bst_t *bst_create(void);
void bst_destroy(bst_t *t);
int bst_insert(bst_t *t, int key);
bool bst_contains(const bst_t *t, int key, size_t *visits);
size_t bst_size(const bst_t *t);
size_t bst_height(const bst_t *t);