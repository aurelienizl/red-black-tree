#include "../rb_tree.h"

static void rb_delete_node_recursive(RB_Tree *tree, RB_Node *node)
{
    if (node != &tree->nil)
    {
        rb_delete_node_recursive(tree, node->left);
        rb_delete_node_recursive(tree, node->right);
        free(node);
    }
}

void rb_tree_destroy(RB_Tree *tree)
{
    if (tree)
    {
        rb_delete_node_recursive(tree, tree->root);
        free(tree);
    }
}
