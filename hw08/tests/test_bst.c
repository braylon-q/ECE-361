#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
#include "bst.h"

static int passes = 0;
static int fails = 0;

#define CHECK(name, cond) do { \
    if (cond) { \
        printf("PASS: %s\n", name); \
        passes++; \
    } else { \
        printf("FAIL: %s\n", name); \
        fails++; \
    } \
} while (0)

static bst_t *build_tree(const int *keys, size_t count)
{
    bst_t *tree = bst_create();
    if (tree == NULL)
        return NULL;

    for (size_t i = 0; i < count; i++) {
        if (bst_insert(tree, keys[i]) != 0) {
            bst_destroy(tree);
            return NULL;
        }
    }

    return tree;
}

static void test_null_tree_search(void)
{
    /*
     * A null tree represents an empty input to the search API. Searching it
     * should return false, set the visit count to zero, and not crash.
     */
    size_t visits = 0;
    CHECK("null-tree search returns false with zero visits",
          !bst_contains(NULL, 27, &visits) && visits == 0);
}

static void test_original_keys(void)
{
    /*
     * Insert the seven values from the original sample tree. Confirm the
     * resulting node count and height, then verify the specified successful
     * and unsuccessful searches and their visit counts.
     */
    const int keys[] = {27, 14, 35, 10, 19, 31, 42};
    bst_t *tree = build_tree(keys, 7);
    CHECK("original seven keys insert successfully", tree != NULL);
    if (tree == NULL)
        return;

    CHECK("original seven keys produce size 7", bst_size(tree) == 7);
    printf("Size of tree: %zu\n", bst_size(tree));
    CHECK("original seven keys produce height 3", bst_height(tree) == 3);
    printf("Height of tree: %zu\n", bst_height(tree));
    
    size_t visits = 0;
    bool found = bst_contains(tree, 31, &visits);
    CHECK("31 is found in 3 visits", found && visits == 3);

    visits = 0;
    found = bst_contains(tree, 15, &visits);
    CHECK("15 is not found in 3 visits", !found && visits == 3);

    bst_destroy(tree);
}

static void test_duplicate_key(void)
{
    /*
     * Insert the same key twice into a fresh tree. Duplicate values should be
     * retained as separate nodes, so the resulting tree should contain two
     * nodes rather than replacing or dropping the duplicate.
     */
    const int keys[] = {27, 27};
    bst_t *tree = build_tree(keys, sizeof(keys) / sizeof(keys[0]));
    CHECK("duplicate keys insert successfully", tree != NULL);
    if (tree == NULL)
        return;

    CHECK("duplicate key is retained (size 2)", bst_size(tree) == 2);
    printf("Size of duplicate-key tree: %zu\n", bst_size(tree));
    bst_destroy(tree);
}

static void test_size_edge_cases(void)
{
    /*
     * Verify the documented null-pointer result and the size of a tree after
     * inserting one key. These cases isolate the base cases of bst_size from
     * the larger tree shapes exercised by the other tests.
     */
    CHECK("NULL tree has size 0", bst_size(NULL) == 0);

    const int keys[] = {27};
    bst_t *tree = build_tree(keys, sizeof(keys) / sizeof(keys[0]));
    CHECK("single-key tree setup succeeds", tree != NULL);
    if (tree == NULL)
        return;

    CHECK("single-key tree has size 1", bst_size(tree) == 1);
    printf("Size of single-key tree: %zu\n", bst_size(tree));
    bst_destroy(tree);
}

static void test_increasing_keys_height(void)
{
    /*
     * Insert seven keys in strictly increasing order. In an unbalanced BST,
     * each key becomes the right child of the previous one, giving height 7.
     */
    const int keys[] = {1, 2, 3, 4, 5, 6, 7};
    bst_t *tree = build_tree(keys, sizeof(keys) / sizeof(keys[0]));
    CHECK("increasing keys insert successfully", tree != NULL);
    if (tree == NULL)
        return;

    CHECK("increasing keys produce size equal to their count",
          bst_size(tree) == sizeof(keys) / sizeof(keys[0]));
    printf("Size of increasing-key tree: %zu\n", bst_size(tree));
    CHECK("increasing keys produce height equal to their count",
          bst_height(tree) == sizeof(keys) / sizeof(keys[0]));
    bst_destroy(tree);
}

static void test_destroy_full_tree(void)
{
    /*
     * Build a populated tree with nodes on both sides of the root and destroy
     * it. Completing the call without a crash is the test; Valgrind can also
     * check that the recursive destruction releases every allocation.
     */
    const int keys[] = {27, 14, 35, 10, 19, 31, 42};
    bst_t *tree = build_tree(keys, sizeof(keys) / sizeof(keys[0]));
    CHECK("full-tree setup succeeds", tree != NULL);
    if (tree == NULL)
        return;

    bst_destroy(tree);
    CHECK("destroying a populated tree completes", true);
}

int main(void)
{
    test_null_tree_search();
    test_original_keys();
    test_duplicate_key();
    test_size_edge_cases();
    test_increasing_keys_height();
    test_destroy_full_tree();

    return fails != 0;
}