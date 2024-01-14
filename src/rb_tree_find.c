#include "../rb_tree.h"

RB_Node *rb_find(RB_Tree *tree, T data)
{
    RB_Node *current = tree->root;
    while (current != &tree->nil)
    {
        if (compEQ(data, current->data))
        {
            return (current);
        }
        else
        {
            current = compLT(data, current->data) ? current->left : current->right;
        }
    }
    return (0);
}
