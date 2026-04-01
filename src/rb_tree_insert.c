#include "rb_tree_internal.h"

static void insertFixup(RB_Tree *tree, RB_Node *x)
{
    while (x != tree->root && x->parent->color == RED)
    {
        if (x->parent == x->parent->parent->left)
        {
            RB_Node *y = x->parent->parent->right;
            if (y->color == RED)
            {
                x->parent->color = BLACK;
                y->color = BLACK;
                x->parent->parent->color = RED;
                x = x->parent->parent;
            }
            else
            {
                if (x == x->parent->right)
                {
                    x = x->parent;
                    rb_rotate_left(tree, x);
                }

                x->parent->color = BLACK;
                x->parent->parent->color = RED;
                rb_rotate_right(tree, x->parent->parent);
            }
        }
        else
        {
            RB_Node *y = x->parent->parent->left;
            if (y->color == RED)
            {
                x->parent->color = BLACK;
                y->color = BLACK;
                x->parent->parent->color = RED;
                x = x->parent->parent;
            }
            else
            {
                if (x == x->parent->left)
                {
                    x = x->parent;
                    rb_rotate_right(tree, x);
                }
                x->parent->color = BLACK;
                x->parent->parent->color = RED;
                rb_rotate_left(tree, x->parent->parent);
            }
        }
    }
    tree->root->color = BLACK;
}

RB_Node *rb_insert(RB_Tree *tree, T data)
{
    RB_Node *current, *parent, *x;

    if (!tree)
    {
        return NULL;
    }

    current = tree->root;
    parent = 0;
    while (current != &tree->nil)
    {
        if (compEQ(data, current->data))
        {
            return (current);
        }
        parent = current;
        current = compLT(data, current->data) ? current->left : current->right;
    }

    if ((x = malloc(sizeof(*x))) == NULL)
    {
        fprintf(stderr, "insufficient memory (rb_insert)\n");
        return NULL;
    }
    x->data = data;
    x->parent = parent;
    x->left = &tree->nil;
    x->right = &tree->nil;
    x->color = RED;

    if (parent)
    {
        if (compLT(data, parent->data))
        {
            parent->left = x;
        }
        else
        {
            parent->right = x;
        }
    }
    else
    {
        tree->root = x;
    }

    insertFixup(tree, x);
    if (tree->root != &tree->nil)
    {
        tree->root->parent = NULL;
    }
    return (x);
}
