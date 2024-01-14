#include <criterion/criterion.h>
#include <criterion/internal/test.h>
#include <stdio.h>

#include "../rb_tree.h"

// Helper function to validate the red-black properties recursively
static int validateRBTree(RB_Tree *tree, RB_Node *node, int blackCount,
                          int *pathBlackCount)
{
    if (node == &tree->nil)
    {
        // When reaching a leaf (nil), check if the number of black nodes is
        // consistent
        if (*pathBlackCount == -1)
        {
            *pathBlackCount = blackCount;
        }
        return *pathBlackCount == blackCount;
    }

    // Red nodes cannot have red children
    if (node->color == RED)
    {
        if ((node->left != NULL && node->left->color == RED)
            || (node->right != NULL && node->right->color == RED))
        {
            return 0;
        }
    }
    else
    {
        // Increment black count for black nodes
        blackCount++;
    }

    // Recursively validate left and right subtrees
    return validateRBTree(tree, node->left, blackCount, pathBlackCount)
        && validateRBTree(tree, node->right, blackCount, pathBlackCount);
}

int rb_validate(RB_Tree *tree)
{
    if (tree == NULL || tree->root == NULL)
    {
        // An empty tree is a valid red-black tree
        return 1;
    }

    // The root must be black
    if (tree->root->color != BLACK)
    {
        return 0;
    }

    int pathBlackCount = -1;
    return validateRBTree(tree, tree->root, 0, &pathBlackCount);
}

int is_unique(int *arr, int size, int num)
{
    for (int i = 0; i < size; i++)
    {
        if (arr[i] == num)
        {
            return 0;
        }
    }
    return 1;
}

int *get_random_array(size_t size, int interval)
{
    int *arr = malloc(size * sizeof(int));
    if (arr == NULL)
    {
        return NULL;
    }

    // Seed the random number generator
    srand(19846329);

    for (size_t i = 0; i < size; ++i)
    {
        int num;
        do
        {
            num = rand() % interval;
        } while (!is_unique(arr, i, num)); // Ensure it's unique in the array

        arr[i] = num; // Add the unique number to the array
    }

    return arr;
}

TestSuite(rb_tree_new, .timeout = 1);

Test(rb_tree_new, should_create_a_new_rb_tree)
{
    RB_Tree *tree = rb_tree_new();

    cr_assert_not_null(tree);
    cr_assert_eq(tree->nil.left, &tree->nil);
    cr_assert_eq(tree->nil.right, &tree->nil);
    cr_assert_eq(tree->nil.parent, &tree->nil);
    cr_assert_eq(tree->nil.color, BLACK);
    cr_assert_eq(tree->nil.data, 0);
    cr_assert_eq(tree->root, &tree->nil);

    rb_tree_destroy(tree);
}

TestSuite(rb_tree_insert, .timeout = 1);

Test(rb_tree_insert, should_insert_a_node_in_the_tree)
{
    RB_Tree *tree = rb_tree_new();

    RB_Node *node = rb_insert(tree, 42);

    cr_assert_not_null(node);
    cr_assert_eq(node->data, 42);
    cr_assert_eq(node->color, BLACK);
    cr_assert_eq(node->left, &tree->nil);
    cr_assert_eq(node->right, &tree->nil);
    cr_assert_eq(node->parent, NULL);
    cr_assert_eq(tree->root, node);

    cr_assert_eq(rb_validate(tree), 1);

    rb_tree_destroy(tree);
}

Test(rb_tree_insert, should_insert_a_node_in_the_tree_2)
{
    RB_Tree *tree = rb_tree_new();

    RB_Node *node = rb_insert(tree, 42);
    RB_Node *node2 = rb_insert(tree, 21);

    cr_assert_not_null(node);
    cr_assert_eq(node->data, 42);
    cr_assert_eq(node->color, BLACK);
    cr_assert_eq(node->left, node2);
    cr_assert_eq(node->right, &tree->nil);
    cr_assert_eq(node->parent, NULL);
    cr_assert_eq(tree->root, node);

    cr_assert_not_null(node2);
    cr_assert_eq(node2->data, 21);
    cr_assert_eq(node2->color, RED);
    cr_assert_eq(node2->left, &tree->nil);
    cr_assert_eq(node2->right, &tree->nil);
    cr_assert_eq(node2->parent, node);
    cr_assert_eq(node->left, node2);

    cr_assert_eq(rb_validate(tree), 1);

    rb_tree_destroy(tree);
}

Test(rb_tree_insert, should_insert_a_node_in_the_tree_3)
{
    RB_Tree *tree = rb_tree_new();

    RB_Node *node = rb_insert(tree, 42);
    RB_Node *node2 = rb_insert(tree, 21);
    RB_Node *node3 = rb_insert(tree, 84);

    cr_assert_not_null(node);
    cr_assert_eq(node->data, 42);
    cr_assert_eq(node->color, BLACK);
    cr_assert_eq(node->left, node2);
    cr_assert_eq(node->right, node3);
    cr_assert_eq(node->parent, NULL);
    cr_assert_eq(tree->root, node);

    cr_assert_not_null(node2);
    cr_assert_eq(node2->data, 21);
    cr_assert_eq(node2->color, RED);
    cr_assert_eq(node2->left, &tree->nil);
    cr_assert_eq(node2->right, &tree->nil);
    cr_assert_eq(node2->parent, node);
    cr_assert_eq(node->left, node2);

    cr_assert_not_null(node3);
    cr_assert_eq(node3->data, 84);
    cr_assert_eq(node3->color, RED);
    cr_assert_eq(node3->left, &tree->nil);
    cr_assert_eq(node3->right, &tree->nil);
    cr_assert_eq(node3->parent, node);
    cr_assert_eq(node->right, node3);

    cr_assert_eq(rb_validate(tree), 1);

    rb_tree_destroy(tree);
}

Test(rb_tree_insert, right_insertion_rotation)
{
    RB_Tree *tree = rb_tree_new();

    // Insert nodes in an order that requires a right rotation
    RB_Node *node1 = rb_insert(tree, 42);
    RB_Node *node2 = rb_insert(tree, 21);
    RB_Node *node3 =
        rb_insert(tree, 10); // Inserting 10 should cause a right rotation

    // Assertions to ensure the structure of the tree after rotation
    cr_assert_not_null(node1);
    cr_assert_not_null(node2);
    cr_assert_not_null(node3);

    // Assertions about the tree structure after the rotation
    // Adjust these assertions based on your tree's rotation logic
    // Example: After rotation, node2 could become the new root
    cr_assert_eq(tree->root, node2);
    cr_assert_eq(node2->left, node3);
    cr_assert_eq(node2->right, node1);

    // Check the properties of each node (color, parent, children)
    // Example: node2 should now be black (as the root), and its children should
    // be red
    cr_assert_eq(node2->color, BLACK);
    cr_assert_eq(node1->color, RED);
    cr_assert_eq(node3->color, RED);

    // Validate the entire tree
    cr_assert_eq(rb_validate(tree), 1);

    rb_tree_destroy(tree);
}

Test(rb_tree_insert, left_insertion_rotation)
{
    RB_Tree *tree = rb_tree_new();

    // Insert nodes in an order that requires a left rotation
    RB_Node *node1 = rb_insert(tree, 10);
    RB_Node *node2 = rb_insert(tree, 21);
    RB_Node *node3 =
        rb_insert(tree, 42); // Inserting 42 should cause a left rotation

    // Assertions to ensure the structure of the tree after rotation
    cr_assert_not_null(node1);
    cr_assert_not_null(node2);
    cr_assert_not_null(node3);

    // Assertions about the tree structure after the rotation
    // Adjust these assertions based on your tree's rotation logic
    // Example: After rotation, node2 could become the new root
    cr_assert_eq(tree->root, node2);
    cr_assert_eq(node2->left, node1);
    cr_assert_eq(node2->right, node3);

    // Check the properties of each node (color, parent, children)
    // Example: node2 should now be black (as the root), and its children should
    // be red
    cr_assert_eq(node2->color, BLACK);
    cr_assert_eq(node1->color, RED);
    cr_assert_eq(node3->color, RED);

    // Validate the entire tree
    cr_assert_eq(rb_validate(tree), 1);

    rb_tree_destroy(tree);
}

Test(rb_tree_insert, random_insertions_10)
{
    RB_Tree *tree = rb_tree_new();

    int values[] = { 42, 21, 10, 84, 1, 2, 3, 4, 5, 6 };
    int size = sizeof(values) / sizeof(int);

    for (int i = 0; i < size; i++)
    {
        rb_insert(tree, values[i]);
    }

    // Validate the entire tree
    cr_assert_eq(rb_validate(tree), 1);

    rb_tree_destroy(tree);
}

Test(rb_tree_insert, random_insertions_100)
{
    RB_Tree *tree = rb_tree_new();

    int *values = get_random_array(100, 100);
    if (values == NULL)
    {
        cr_assert_fail("Failed to allocate memory for random array");
    }

    for (int i = 0; i < 100; i++)
    {
        rb_insert(tree, values[i]);
    }

    // Validate the entire tree
    cr_assert_eq(rb_validate(tree), 1);

    rb_tree_destroy(tree);
    free(values);
}

Test(rb_tree_insert, random_insertions_1000)
{
    RB_Tree *tree = rb_tree_new();

    int *values = get_random_array(1000, 1000);
    if (values == NULL)
    {
        cr_assert_fail("Failed to allocate memory for random array");
    }

    for (int i = 0; i < 1000; i++)
    {
        rb_insert(tree, values[i]);
    }

    // Validate the entire tree
    cr_assert_eq(rb_validate(tree), 1);

    rb_tree_destroy(tree);
    free(values);
}

TestSuite(rb_tree_find, .timeout = 1);

Test(rb_tree_find, should_find_a_node_in_the_tree)
{
    RB_Tree *tree = rb_tree_new();

    RB_Node *node = rb_insert(tree, 42);
    RB_Node *node2 = rb_insert(tree, 21);
    RB_Node *node3 = rb_insert(tree, 84);

    // Cast to void to silence the unused variable warning
    (void)node;
    (void)node2;
    (void)node3;

    RB_Node *found = rb_find(tree, 21);

    cr_assert_not_null(found);
    cr_assert_eq(found, node2);

    rb_tree_destroy(tree);
}

Test(rb_tree_find, should_find_a_node_in_the_tree_2)
{
    RB_Tree *tree = rb_tree_new();

    RB_Node *node = rb_insert(tree, 42);
    RB_Node *node2 = rb_insert(tree, 21);
    RB_Node *node3 = rb_insert(tree, 84);

    // Cast to void to silence the unused variable warning
    (void)node;
    (void)node2;
    (void)node3;

    RB_Node *found = rb_find(tree, 84);

    cr_assert_not_null(found);
    cr_assert_eq(found, node3);

    rb_tree_destroy(tree);
}

Test(rb_tree_find, should_find_a_node_in_the_tree_3)
{
    RB_Tree *tree = rb_tree_new();

    RB_Node *node = rb_insert(tree, 42);
    RB_Node *node2 = rb_insert(tree, 21);
    RB_Node *node3 = rb_insert(tree, 84);

    // Cast to void to silence the unused variable warning
    (void)node;
    (void)node2;
    (void)node3;

    RB_Node *found = rb_find(tree, 42);

    cr_assert_not_null(found);
    cr_assert_eq(found, node);

    rb_tree_destroy(tree);
}

Test(rb_tree_find, should_not_find_a_node_in_the_tree)
{
    RB_Tree *tree = rb_tree_new();

    RB_Node *node = rb_insert(tree, 42);
    RB_Node *node2 = rb_insert(tree, 21);
    RB_Node *node3 = rb_insert(tree, 84);

    // Cast to void to silence the unused variable warning
    (void)node;
    (void)node2;
    (void)node3;

    RB_Node *found = rb_find(tree, 100);

    cr_assert_null(found);

    rb_tree_destroy(tree);
}

Test(rb_tree_find, should_not_find_a_node_in_the_tree_2)
{
    RB_Tree *tree = rb_tree_new();

    RB_Node *node = rb_insert(tree, 42);
    RB_Node *node2 = rb_insert(tree, 21);
    RB_Node *node3 = rb_insert(tree, 84);

    // Cast to void to silence the unused variable warning
    (void)node;
    (void)node2;
    (void)node3;

    RB_Node *found = rb_find(tree, 0);

    cr_assert_null(found);

    rb_tree_destroy(tree);
}

Test(rb_tree_find, should_not_find_a_node_in_the_tree_3)
{
    RB_Tree *tree = rb_tree_new();

    RB_Node *node = rb_insert(tree, 42);
    RB_Node *node2 = rb_insert(tree, 21);
    RB_Node *node3 = rb_insert(tree, 84);

    // Cast to void to silence the unused variable warning
    (void)node;
    (void)node2;
    (void)node3;

    RB_Node *found = rb_find(tree, 50);

    cr_assert_null(found);

    rb_tree_destroy(tree);
}

TestSuite(rb_delete, .timeout = 1);

Test(rb_delete, should_delete_a_node_from_the_tree)
{
    RB_Tree *tree = rb_tree_new();

    RB_Node *node = rb_insert(tree, 42);

    rb_delete(tree, node);

    cr_assert_eq(tree->root, &tree->nil);

    rb_tree_destroy(tree);
}

Test(rb_delete, should_delete_a_node_from_the_tree_2)
{
    RB_Tree *tree = rb_tree_new();

    RB_Node *node = rb_insert(tree, 42);
    RB_Node *node2 = rb_insert(tree, 21);

    rb_delete(tree, node);

    cr_assert_eq(tree->root, node2);

    rb_tree_destroy(tree);
}

Test(rb_delete, should_delete_a_node_from_the_tree_3)
{
    RB_Tree *tree = rb_tree_new();

    RB_Node *node = rb_insert(tree, 42);
    RB_Node *node2 = rb_insert(tree, 21);
    RB_Node *node3 = rb_insert(tree, 84);

    // Cast to void to silence the unused variable warning
    (void)node;
    (void)node2;
    (void)node3;

    rb_delete(tree, node);

    cr_assert_eq(tree->root->data, 84);
    cr_assert_eq(tree->root->left->data, 21);
    cr_assert_eq(tree->root->right, &tree->nil);
    
    rb_tree_destroy(tree);
}

Test(rb_delete, should_delete_a_node_from_the_tree_4)
{
    RB_Tree *tree = rb_tree_new();

    RB_Node *node = rb_insert(tree, 42);
    RB_Node *node2 = rb_insert(tree, 21);
    RB_Node *node3 = rb_insert(tree, 84);

    // Cast to void to silence the unused variable warning
    (void)node;
    (void)node2;
    (void)node3;

    rb_delete(tree, node2);

    cr_assert_eq(tree->root->data, 42);
    cr_assert_eq(tree->root->left, &tree->nil);
    cr_assert_eq(tree->root->right->data, 84);
    
    rb_tree_destroy(tree);
}

Test(rb_delete, should_delete_a_node_from_the_tree_5)
{
    RB_Tree *tree = rb_tree_new();

    RB_Node *node = rb_insert(tree, 42);
    RB_Node *node2 = rb_insert(tree, 21);
    RB_Node *node3 = rb_insert(tree, 84);
    RB_Node *node4 = rb_insert(tree, 10);
    RB_Node *node5 = rb_insert(tree, 1);
    RB_Node *node6 = rb_insert(tree, 2);
    RB_Node *node7 = rb_insert(tree, 3);
    RB_Node *node8 = rb_insert(tree, 4);
    RB_Node *node9 = rb_insert(tree, 5);
    RB_Node *node10 = rb_insert(tree, 6);

    // Cast to void to silence the unused variable warning
    (void)node;
    (void)node2;
    (void)node3;
    (void)node4;
    (void)node5;
    (void)node6;
    (void)node7;
    (void)node8;
    (void)node9;
    (void)node10;

    rb_delete(tree, node2);

    rb_todot(tree, "rb_tree_delete_5.dot");

    cr_assert_eq(tree->root->data, 4);
    cr_assert_eq(tree->root->left->data, 2);
    cr_assert_eq(tree->root->right->data, 10);
    cr_assert_eq(tree->root->left->left->data, 1);
    cr_assert_eq(tree->root->left->right->data, 3);
    cr_assert_eq(tree->root->right->left->data, 5);
    cr_assert_eq(tree->root->right->right->data, 42);
    cr_assert_eq(tree->root->right->left->right->data, 6);
    cr_assert_eq(tree->root->right->right->right->data, 84);

    cr_assert_eq(rb_validate(tree), 1);

    rb_tree_destroy(tree);
}

TestSuite(random_operations, .timeout = 1);

Test(random_operations, should_insert_and_delete_randomly_10)
{
    RB_Tree *tree = rb_tree_new();

    int *values = get_random_array(10, 10);
    if (values == NULL)
    {
        cr_assert_fail("Failed to allocate memory for random array");
    }

    for (int i = 0; i < 10; i++)
    {
        cr_assert_eq(rb_validate(tree), 1);
        rb_insert(tree, values[i]);
    }

    for (int i = 0; i < 10; i++)
    {
        cr_assert_eq(rb_validate(tree), 1);
        rb_delete(tree, rb_find(tree, values[i]));
    }

    // Validate the entire tree
    cr_assert_eq(rb_validate(tree), 1);

    rb_tree_destroy(tree);
    free(values);
}

Test(random_operations, should_insert_and_delete_randomly_100)
{
    RB_Tree *tree = rb_tree_new();

    int *values = get_random_array(100, 100);
    if (values == NULL)
    {
        cr_assert_fail("Failed to allocate memory for random array");
    }

    for (int i = 0; i < 100; i++)
    {
        cr_assert_eq(rb_validate(tree), 1);
        rb_insert(tree, values[i]);
    }

    for (int i = 0; i < 100; i++)
    {
        cr_assert_eq(rb_validate(tree), 1);
        rb_delete(tree, rb_find(tree, values[i]));
    }

    // Validate the entire tree
    cr_assert_eq(rb_validate(tree), 1);

    rb_tree_destroy(tree);
    free(values);
}

Test(random_operations, should_insert_and_delete_randomly_1000)
{
    RB_Tree *tree = rb_tree_new();

    int *values = get_random_array(1000, 1000);
    if (values == NULL)
    {
        cr_assert_fail("Failed to allocate memory for random array");
    }

    for (int i = 0; i < 1000; i++)
    {
        cr_assert_eq(rb_validate(tree), 1);
        rb_insert(tree, values[i]);
    }

    for (int i = 999; i >= 0; i--)
    {
        cr_assert_eq(rb_validate(tree), 1);
        rb_delete(tree, rb_find(tree, values[i]));
    }

    // Validate the entire tree
    cr_assert_eq(rb_validate(tree), 1);

    rb_tree_destroy(tree);
    free(values);
}

