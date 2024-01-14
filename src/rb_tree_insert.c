#include "../rb_tree.h"

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
                    rotateLeft(tree, x);
                }

                x->parent->color = BLACK;
                x->parent->parent->color = RED;
                rotateRight(tree, x->parent->parent);
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
                    rotateRight(tree, x);
                }
                x->parent->color = BLACK;
                x->parent->parent->color = RED;
                rotateLeft(tree, x->parent->parent);
            }
        }
    }
    tree->root->color = BLACK;
}

RB_Node *rb_insert(RB_Tree *tree, T data)
{
    RB_Node *current, *parent, *x;

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

    if ((x = malloc(sizeof(*x))) == 0)
    {
        printf("insufficient memory (insertNode)\n");
        return (0);
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
    return (x);
}
