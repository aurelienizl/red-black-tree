#ifndef RB_TREE_H
#define RB_TREE_H

// Standard libraries

#include <stdio.h>
#include <stdlib.h>

// User specific parameters

/**
 * @brief Type of data stored in the tree (int, float, char, etc.)
 * @note This type must be comparable using the compLT and compEQ macros
 * @note This typedef must be user specific
 */
typedef int T;

/**
 * @brief Function to compare two elements of type T
 * @param a First element
 * @param b Second element
 * @return 1 if a < b, 0 if a == b, -1 if a > b
 * @note This function must be user specific
 */
#define compLT(a, b) (a < b)

/**
 * @brief Function to compare two elements of type T
 * @param a First element
 * @param b Second element
 * @return 1 if a == b, 0 otherwise
 * @note This function must be user specific
 */
#define compEQ(a, b) (a == b)

// Red black tree structure

/**
 * @brief Color of a node
 * @note This enum is NOT user specific
 */
typedef enum
{
    BLACK,
    RED
} RB_Color;

/**
 * @brief Node of the red black tree
 * @param left Left child
 * @param right Right child
 * @param parent Parent node
 * @param color Color of the node
 * @param data Data stored in the node
 * @note This struct is NOT user specific
 */
typedef struct RB_Node_
{
    struct RB_Node_ *left;
    struct RB_Node_ *right;
    struct RB_Node_ *parent;
    RB_Color color;
    T data;
} RB_Node;

/**
 * @brief Red black tree
 * @param root Root node of the tree
 * @param nil Nil node of the tree
 * @note This struct is NOT user specific
 * @note The nil node is used to avoid special cases when a node has no child or
 * no parent
 */
typedef struct RB_Tree_
{
    RB_Node *root;
    RB_Node nil;
} RB_Tree;

// Functions

/**
 * @brief Allocate a new tree and initialize it
 * @return (RB_Tree*) Pointer to the new tree
 */
RB_Tree *rb_tree_new(void);

/**
 * @brief Destroy a tree and free the memory
 * @param tree Tree to destroy
 * @note This function does not free the data stored in the tree
 * @return (void)
 */
void rb_tree_destroy(RB_Tree *tree);

/**
 * @brief Insert a new node in the tree
 * @param tree Tree in which the node will be inserted
 * @param data Data to store in the new node
 * @return (RB_Node*) Pointer to the inserted node
 * @note This function returns a pointer to the inserted node
 * @note If an existing node has the same data, the function returns a pointer
 * to this node
 */
RB_Node *rb_insert(RB_Tree *tree, T data);

/**
 * @brief Delete a node from the tree
 * @param tree Tree from which the node will be deleted
 * @param data Data of the node to delete
 * @return (void)
 * @note This function does not free the data stored in the node
 * @note The user is expected to call findNode before calling this function, in
 * order to check if the node exists
 */
void rb_delete(RB_Tree *tree, RB_Node *z);

/**
 * @brief Find a node in the tree
 * @param tree Tree in which the node will be searched
 * @param data Data of the node to find
 * @return (RB_Node*) Pointer to the found node, or NULL if the node does not
 * exist
 */
RB_Node *rb_find(RB_Tree *tree, T data);

// Utils

/**
 * @brief This function rotates the tree to the left from the given node
 * @param tree Tree to rotate
 * @param x Node from which the tree will be rotated
 * @return (void)
 */
void rotateLeft(RB_Tree *tree, RB_Node *x);

/**
 * @brief This function rotates the tree to the right from the given node
 * @param tree Tree to rotate
 * @param x Node from which the tree will be rotated
 * @return (void)
 */
void rotateRight(RB_Tree *tree, RB_Node *x);

/**
 * @brief This function writes the tree in the dot format in the given file
 * @param tree Tree to write
 * @param filename Name of the file in which the tree will be written
 * @return (void)
 */
void rb_todot(RB_Tree *tree, char *filename);

#endif // RB_TREE_H
