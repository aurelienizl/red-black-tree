#include "../rb_tree.h"

void generateDot(RB_Tree *tree, RB_Node *node, FILE *fp)
{
    if (node == &tree->nil)
        return;

    if (node->parent != NULL)
    {
        fprintf(fp, "  \"%d\" -> \"%d\";\n", node->parent->data, node->data);
    }

    if (node->color == RED)
    {
        fprintf(fp, "  \"%d\" [color=red];\n", node->data);
    }

    generateDot(tree, node->left, fp);
    generateDot(tree, node->right, fp);
}

void rb_todot(RB_Tree *tree, char *filename)
{
    FILE *fp = fopen(filename, "w");
    if (!fp)
        return;

    fprintf(fp, "digraph G {\n");
    generateDot(tree, tree->root, fp);
    fprintf(fp, "}\n");
    fclose(fp);
}
