#include <stdio.h>

#include "bst.h"

int main(void)
{
    bst_t *tree = bst_create();
    if (tree == NULL) {
        fprintf(stderr, "treestats: unable to create tree\n");
        return 1;
    }

    size_t count = 0;
    int key;
    int result;
    while ((result = scanf("%d", &key)) == 1) {
        if (bst_insert(tree, key) != 0) {
            fprintf(stderr, "treestats: unable to insert value\n");
            bst_destroy(tree);
            return 1;
        }
        count++;
    }

    if (result != EOF) {
        printf("not a number after %zu values\n", count);
        bst_destroy(tree);
        return 1;
    }

    if (ferror(stdin)) {
        fprintf(stderr, "treestats: failed to read input\n");
        bst_destroy(tree);
        return 1;
    }

    if (count == 0) {
        printf("empty tree\n");
    } else {
        printf("n %zu, height %zu\n", count, bst_height(tree));
    }

    bst_destroy(tree);
    return 0;
}