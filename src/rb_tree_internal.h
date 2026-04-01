#ifndef RB_TREE_INTERNAL_H
#define RB_TREE_INTERNAL_H

#include "../rb_tree.h"

void rb_rotate_left(RB_Tree *tree, RB_Node *x);
void rb_rotate_right(RB_Tree *tree, RB_Node *x);

#endif // RB_TREE_INTERNAL_H
