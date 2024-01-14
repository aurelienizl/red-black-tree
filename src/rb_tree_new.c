#include "../rb_tree.h"

RB_Tree *rb_tree_new(void)
{
    RB_Tree *tree = malloc(sizeof(RB_Tree));
    if (!tree)
    {
        return NULL;
    }

    tree->nil.left = &tree->nil;
    tree->nil.right = &tree->nil;
    tree->nil.parent = &tree->nil;
    tree->nil.color = BLACK;
    tree->nil.data = 0;

    tree->root = &tree->nil;

    return tree;
}
