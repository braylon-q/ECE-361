#include <stdlib.h>
#include <stddef.h> 
#include <stdbool.h>
#include "bst.h"

struct bst {
    int key;
    struct bst *left;
    struct bst *right;
}; //bst_t

bst_t *bst_create(void) {
    /*
    Creates a new binary search tree and returns a pointer to it. 
    NULL if no memory is available. Takes no parameters.

    Use malloc to allocate memory for new tree. Check for failure, 
    then return pointer to new tree. Initalize all values to respective 
    NULL's.
    */
    bst_t *newTree = (bst_t *)malloc(sizeof(bst_t)); //allocate and cast to pointer
    if (newTree == NULL) 
        return NULL; //check for failure
    newTree->key = 0x80000000; //initialize key to minimum int value.
    newTree->left = NULL; //initialize left to NULL
    newTree->right = NULL; //initialize right to NULL

    return newTree; //return pointer to new tree
}

void bst_destroy(bst_t *t) {
    /*
    Destroys a binary search tree and frees all memory.
    Takes a pointer to the tree to be destroyed as a parameter. Returns nothing.

    Recursively free all child nodes, finally freeing root at end. 
    return if t is NULL.
    */
    if (t == NULL) 
        return; //return if t is NULL or we have reached a leaf node (end of tree path)

    bst_destroy(t->left); //recursively free left child
    bst_destroy(t->right); //recursively free right child
    free(t); //free the current node
}

int bst_insert(bst_t *t, int key)
{
    /*
    Inserts a new key into the bst.
    returns 0 for success, -1 if t is NULL or memory runs out.
    Takes a pointer to the tree and the key to be inserted as parameters.

    Compares key with current node key. If key < node_key, traverse left, else
    traverse right (equal key is kept on right). If we reach a NULL node, insert new key there. 
    */
    if (t == NULL) 
        return -1; //return -1 if t is NULL

    if (t->key == (int)0x80000000) { //check if tree is empty (root node has minimum int value)
        t->key = key; //set root node key to new key
        return 0; //return success
    }
    else if (key < t->key) { //if key is less than current node key, traverse left
        if (t->left == NULL) { //if left child is NULL, insert new key here
            bst_t *newNode = (bst_t *)malloc(sizeof(bst_t)); //allocate memory for new node
            if (newNode == NULL) 
                return -1; //return -1 if memory allocation fails
            newNode->key = key; //set new node key
            newNode->left = NULL; //initialize left child to NULL
            newNode->right = NULL; //initialize right child to NULL
            t->left = newNode; //link new node to left child of current node
            return 0; //return success
        } else {
            return bst_insert(t->left, key); //recursively insert in left subtree
        }
    } else { //if key is greater than or equal to current node key, traverse right
        if (t->right == NULL) { //if right child is NULL, insert new key here
            bst_t *newNode = (bst_t *)malloc(sizeof(bst_t)); //allocate memory for new node
            if (newNode == NULL) 
                return -1; //return -1 if memory allocation fails
            newNode->key = key; //set new node key
            newNode->left = NULL; //initialize left child to NULL
            newNode->right = NULL; //initialize right child to NULL
            t->right = newNode; //link new node to right child of current node
            return 0; //return success
        } else {
            return bst_insert(t->right, key); //recursively insert in right subtree
        }
    }
}

bool bst_contains(const bst_t *t, int key, size_t *visits)
{
    /*
    BinarySearchTree search function, without printing; whether a key is in the tree.
    Returns true if key is found, false otherwise. Takes a pointer to the tree,
    the key to be searched for, and a pointer to a size_t variable to count the
    number of nodes visited during the search as parameters.

    Check for invalid inputs first.
    */

    if (t == NULL){
        if (visits != NULL) {
            *visits = 0; //set visits to 0 if t is NULL
        }
        return false; //return false if t or visits is NULL
    }

    const bst_t *current = t; //start at the root of the tree
    *visits = 1; //initialize visits count to 1 for first node

    while(current->key != key) {
	  // key @ current node > key we are looking for
	  // so we need to traverse the path where key < key current node
      //go to left tree
      if(current->key > key) {  //NOTE: will maybe cause problem without >=. By design for current testing.
         current = current->left;
      }
      //else go to right tree
      else {                
         current = current->right;
      }

      //not found
      if(current == NULL) {
         return false;
      }
      if (visits != NULL) *(visits) = *(visits) + 1; //increment visits count
   }

   return true; //key found
}

size_t bst_size(const bst_t *t)
{
    /*
    Returns the number of nodes (keys) in the tree. 
    Takes const pointer to the tree as a parameter. Returns 0 if t is NULL.

    Recursively count the number of nodes in the tree. Search left to right,
    adding 1 in reverse order after leaving each node. Finally increment should be
    the root node, returning the total count.
    */

    if (t == NULL) 
        return 0; //return 0 if t is NULL
    else if (t->left == NULL && t->right == NULL) 
        return 1; //return 0 if t is a leaf node (no children)

    size_t size = 1; //intialize to 0 

    if (t->left != NULL)
        size += bst_size(t->left); //recursively count left subtree
    if (t->right != NULL)
        size += bst_size(t->right); //recursively count right subtree
    
    return size;

}

size_t bst_height(const bst_t *t)
{
    /*
    Returns the height of the tree; the longest path. Takes const pointer to the tree as a parameter.
    Returns 0 if t is NULL.

    Recursively find the height of the left and right subtrees, returning the greater
    of the two plus 1 for the current node.
    */

    if (t == NULL) 
        return 0; //return 0 if t is NULL
    else if (t->left == NULL && t->right == NULL) 
        return 1; //return 1 if t is a leaf node (no children)

    size_t leftHeight = 0; //initialize left height to 0
    size_t rightHeight = 0; //initialize right height to 0

    /*These calls will eventually collapse to 0. The leaf where left & right = NULL
    returns 0 for both, then node will return 1, next node returns 2, etc. until we reach 
    the root node, which will return the total height of the tree.
    */
    if (t->left != NULL)
        leftHeight = bst_height(t->left); //get height of left subtree
    if (t->right != NULL)
        rightHeight = bst_height(t->right); //get height of right subtree

    return (leftHeight > rightHeight ? leftHeight : rightHeight) + 1; //return greater height plus 1 for current node
}
