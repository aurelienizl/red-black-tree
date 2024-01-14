#include "../rb_tree.h"


void rotateLeft(RB_Tree *tree, RB_Node *x)
{
    RB_Node *y = x->right;

    x->right = y->left;
    if (y->left != &tree->nil)
    {
        y->left->parent = x;
    }
    if (y != &tree->nil)
    {
        y->parent = x->parent;
    }
    if (x->parent)
    {
        if (x == x->parent->left)
        {
            x->parent->left = y;

        }
        else
        {
            x->parent->right = y;
        }
    }
    else
    {
        tree->root = y;
    }

    y->left = x;
    if (x != &tree->nil)
    {
        x->parent = y;
    }
}

void rotateRight(RB_Tree *tree, RB_Node *x)
{
    RB_Node *y = x->left;

    x->left = y->right;
    if (y->right != &tree->nil)
    {
        y->right->parent = x;
    }

    if (y != &tree->nil)
    {
        y->parent = x->parent;
    }
    if (x->parent)
    {
        if (x == x->parent->right)
        {
            x->parent->right = y;
        }
        else
        {
            x->parent->left = y;
        }
    }
    else
    {
        tree->root = y;
    }

    y->right = x;
    if (x != &tree->nil)
    {
        x->parent = y;
    }
}
