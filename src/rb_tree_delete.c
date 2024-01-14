#include "../rb_tree.h"

// See https://www.codesdope.com/course/data-structures-red-black-trees-deletion/

/* deleteFixup function is used to restore red-black tree properties after a node deletion */
static void deleteFixup(RB_Tree *tree, RB_Node *x) {
    while (x != tree->root && x->color == BLACK) {
        // Case when x is a left child
        if (x == x->parent->left) {
            RB_Node *w = x->parent->right; // sibling of x

            // Case 1: x's sibling w is red
            if (w->color == RED) {
                w->color = BLACK;
                x->parent->color = RED;
                rotateLeft(tree, x->parent);
                w = x->parent->right;
            }

            // Case 2: Both of w's children are black
            if (w->left->color == BLACK && w->right->color == BLACK) {
                w->color = RED;
                x = x->parent;
            } else {
                // Case 3: w's right child is black
                if (w->right->color == BLACK) {
                    w->left->color = BLACK;
                    w->color = RED;
                    rotateRight(tree, w);
                    w = x->parent->right;
                }
                // Case 4: w's right child is red
                w->color = x->parent->color;
                x->parent->color = BLACK;
                w->right->color = BLACK;
                rotateLeft(tree, x->parent);
                x = tree->root;
            }
        } else {
            // Mirror cases when x is a right child
            RB_Node *w = x->parent->left;

            if (w->color == RED) {
                w->color = BLACK;
                x->parent->color = RED;
                rotateRight(tree, x->parent);
                w = x->parent->left;
            }

            if (w->right->color == BLACK && w->left->color == BLACK) {
                w->color = RED;
                x = x->parent;
            } else {
                if (w->left->color == BLACK) {
                    w->right->color = BLACK;
                    w->color = RED;
                    rotateLeft(tree, w);
                    w = x->parent->left;
                }
                w->color = x->parent->color;
                x->parent->color = BLACK;
                w->left->color = BLACK;
                rotateRight(tree, x->parent);
                x = tree->root;
            }
        }
    }
    x->color = BLACK; // Ensure the root remains black
}

/* deleteNode function removes a node from the red-black tree */
void rb_delete(RB_Tree *tree, RB_Node *z) {
    RB_Node *x, *y;

    // Return if the node to delete is NULL or tree's sentinel node
    if (!z || z == &tree->nil) {
        return;
    }

    // Determine the node y to splice out, which is either z or its successor
    if (z->left == &tree->nil || z->right == &tree->nil) {
        y = z;
    } else {
        y = z->right;
        while (y->left != &tree->nil) {
            y = y->left;
        }
    }

    // x is y's only child
    if (y->left != &tree->nil) {
        x = y->left;
    } else {
        x = y->right;
    }

    // Remove y from the parent chain
    x->parent = y->parent;
    if (y->parent) {
        if (y == y->parent->left) {
            y->parent->left = x;
        } else {
            y->parent->right = x;
        }
    } else {
        tree->root = x;
    }

    // If y is a copy of z (successor case), transfer z's data to y
    if (y != z) {
        z->data = y->data;
    }

    // Fix-up any violations of red-black properties
    if (y->color == BLACK) {
        deleteFixup(tree, x);
    }

    // Free the memory of the spliced-out node
    free(y);
}
