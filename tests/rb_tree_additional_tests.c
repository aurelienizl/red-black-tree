#include <criterion/criterion.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "../rb_tree.h"

typedef struct
{
    int ok;
    int black_height;
} ValidationResult;

static ValidationResult validate_node(RB_Tree *tree, RB_Node *node, int has_min,
                                      T min_value, int has_max, T max_value)
{
    ValidationResult result = { 0, 0 };

    if (node == &tree->nil)
    {
        result.ok = 1;
        result.black_height = 1;
        return result;
    }

    if (!node || !node->left || !node->right)
    {
        return result;
    }

    if ((has_min && !compLT(min_value, node->data))
        || (has_max && !compLT(node->data, max_value)))
    {
        return result;
    }

    if ((node->left != &tree->nil && node->left->parent != node)
        || (node->right != &tree->nil && node->right->parent != node))
    {
        return result;
    }

    if (node->color == RED
        && (node->left->color == RED || node->right->color == RED))
    {
        return result;
    }

    ValidationResult left =
        validate_node(tree, node->left, has_min, min_value, 1, node->data);
    ValidationResult right =
        validate_node(tree, node->right, 1, node->data, has_max, max_value);

    if (!left.ok || !right.ok || left.black_height != right.black_height)
    {
        return result;
    }

    result.ok = 1;
    result.black_height = left.black_height + (node->color == BLACK ? 1 : 0);
    return result;
}

static int validate_tree_strict(RB_Tree *tree)
{
    if (!tree)
    {
        return 0;
    }

    if (tree->nil.left != &tree->nil || tree->nil.right != &tree->nil
        || tree->nil.color != BLACK)
    {
        return 0;
    }

    if (tree->root == &tree->nil)
    {
        return 1;
    }

    if (!tree->root || tree->root->parent != NULL || tree->root->color != BLACK)
    {
        return 0;
    }

    ValidationResult root_check = validate_node(tree, tree->root, 0, 0, 0, 0);
    return root_check.ok;
}

static void swap_int(int *a, int *b)
{
    int tmp = *a;
    *a = *b;
    *b = tmp;
}

static int next_permutation_int(int *values, int size)
{
    int i = size - 2;

    while (i >= 0 && values[i] >= values[i + 1])
    {
        i--;
    }
    if (i < 0)
    {
        return 0;
    }

    int j = size - 1;
    while (values[j] <= values[i])
    {
        j--;
    }

    swap_int(&values[i], &values[j]);

    int left = i + 1;
    int right = size - 1;
    while (left < right)
    {
        swap_int(&values[left], &values[right]);
        left++;
        right--;
    }

    return 1;
}

static void fill_range(int *arr, int size, int start)
{
    for (int i = 0; i < size; i++)
    {
        arr[i] = start + i;
    }
}

static void shuffle_int_array(int *arr, int size, unsigned int seed)
{
    srand(seed);
    for (int i = size - 1; i > 0; i--)
    {
        int j = rand() % (i + 1);
        swap_int(&arr[i], &arr[j]);
    }
}

TestSuite(rb_tree_additional_api, .timeout = 3);

Test(rb_tree_additional_api, handles_null_inputs_without_crashing)
{
    cr_assert_null(rb_insert(NULL, 123));
    cr_assert_null(rb_find(NULL, 123));

    rb_delete(NULL, NULL);
    rb_todot(NULL, "/tmp/rb_tree_unused.dot");

    RB_Tree *tree = rb_tree_new();
    cr_assert_not_null(tree);

    rb_delete(tree, NULL);
    rb_todot(tree, NULL);

    cr_assert_eq(validate_tree_strict(tree), 1);
    rb_tree_destroy(tree);
}

Test(rb_tree_additional_api, deleting_missing_node_is_noop)
{
    RB_Tree *tree = rb_tree_new();
    cr_assert_not_null(tree);

    rb_insert(tree, 10);
    rb_insert(tree, 20);
    rb_insert(tree, 30);

    rb_delete(tree, rb_find(tree, 99));

    cr_assert_not_null(rb_find(tree, 10));
    cr_assert_not_null(rb_find(tree, 20));
    cr_assert_not_null(rb_find(tree, 30));
    cr_assert_eq(validate_tree_strict(tree), 1);

    rb_tree_destroy(tree);
}

Test(rb_tree_additional_api, writes_dot_file_with_valid_arguments)
{
    RB_Tree *tree = rb_tree_new();
    cr_assert_not_null(tree);

    rb_insert(tree, 7);
    rb_insert(tree, 3);
    rb_insert(tree, 11);

    const char *path = "/tmp/rb_tree_additional.dot";
    rb_todot(tree, path);

    FILE *fp = fopen(path, "r");
    cr_assert_not_null(fp);

    char header[32] = { 0 };
    char *line = fgets(header, sizeof(header), fp);
    fclose(fp);
    remove(path);

    cr_assert_not_null(line);
    cr_assert_not_null(strstr(header, "digraph G"));

    rb_tree_destroy(tree);
}

TestSuite(rb_tree_additional_insert, .timeout = 8);

Test(rb_tree_additional_insert, duplicate_insert_returns_same_node_and_no_extra)
{
    RB_Tree *tree = rb_tree_new();
    cr_assert_not_null(tree);

    RB_Node *n1 = rb_insert(tree, 42);
    RB_Node *n2 = rb_insert(tree, 42);

    cr_assert_not_null(n1);
    cr_assert_eq(n1, n2);
    cr_assert_eq(validate_tree_strict(tree), 1);

    rb_delete(tree, n1);
    cr_assert_null(rb_find(tree, 42));
    cr_assert_eq(tree->root, &tree->nil);
    cr_assert_eq(validate_tree_strict(tree), 1);

    rb_tree_destroy(tree);
}

Test(rb_tree_additional_insert, handles_extreme_integer_values)
{
    RB_Tree *tree = rb_tree_new();
    cr_assert_not_null(tree);

    int values[] = { INT_MIN, -1, 0, 1, INT_MAX };
    int size = (int)(sizeof(values) / sizeof(values[0]));

    for (int i = 0; i < size; i++)
    {
        cr_assert_not_null(rb_insert(tree, values[i]));
        cr_assert_eq(validate_tree_strict(tree), 1);
    }

    for (int i = 0; i < size; i++)
    {
        RB_Node *node = rb_find(tree, values[i]);
        cr_assert_not_null(node);
        cr_assert_eq(node->data, values[i]);
    }

    rb_tree_destroy(tree);
}

Test(rb_tree_additional_insert, ascending_insertions_remain_valid)
{
    RB_Tree *tree = rb_tree_new();
    cr_assert_not_null(tree);

    int invalid_insert_step = -1;
    for (int i = 0; i < 800; i++)
    {
        rb_insert(tree, i);
        if (!validate_tree_strict(tree))
        {
            invalid_insert_step = i;
            break;
        }
    }
    cr_assert_eq(invalid_insert_step, -1, "Tree became invalid at insert step %d",
                 invalid_insert_step);

    int missing_value = -1;
    for (int i = 0; i < 800; i++)
    {
        if (rb_find(tree, i) == NULL)
        {
            missing_value = i;
            break;
        }
    }
    cr_assert_eq(missing_value, -1, "Missing value after ascending inserts: %d",
                 missing_value);

    rb_tree_destroy(tree);
}

Test(rb_tree_additional_insert, descending_insertions_remain_valid)
{
    RB_Tree *tree = rb_tree_new();
    cr_assert_not_null(tree);

    int invalid_insert_step = -1;
    for (int i = 799; i >= 0; i--)
    {
        rb_insert(tree, i);
        if (!validate_tree_strict(tree))
        {
            invalid_insert_step = i;
            break;
        }
    }
    cr_assert_eq(invalid_insert_step, -1, "Tree became invalid at insert key %d",
                 invalid_insert_step);

    int missing_value = -1;
    for (int i = 0; i < 800; i++)
    {
        if (rb_find(tree, i) == NULL)
        {
            missing_value = i;
            break;
        }
    }
    cr_assert_eq(missing_value, -1, "Missing value after descending inserts: %d",
                 missing_value);

    rb_tree_destroy(tree);
}

TestSuite(rb_tree_additional_delete, .timeout = 8);

Test(rb_tree_additional_delete, deletes_root_with_only_left_child)
{
    RB_Tree *tree = rb_tree_new();
    cr_assert_not_null(tree);

    RB_Node *root = rb_insert(tree, 20);
    RB_Node *left = rb_insert(tree, 10);
    (void)left;

    rb_delete(tree, root);

    cr_assert_not_null(rb_find(tree, 10));
    cr_assert_null(rb_find(tree, 20));
    cr_assert_eq(validate_tree_strict(tree), 1);
    cr_assert_eq(tree->root->parent, NULL);

    rb_tree_destroy(tree);
}

Test(rb_tree_additional_delete, deletes_root_with_only_right_child)
{
    RB_Tree *tree = rb_tree_new();
    cr_assert_not_null(tree);

    RB_Node *root = rb_insert(tree, 10);
    RB_Node *right = rb_insert(tree, 20);
    (void)right;

    rb_delete(tree, root);

    cr_assert_not_null(rb_find(tree, 20));
    cr_assert_null(rb_find(tree, 10));
    cr_assert_eq(validate_tree_strict(tree), 1);
    cr_assert_eq(tree->root->parent, NULL);

    rb_tree_destroy(tree);
}

Test(rb_tree_additional_delete, deletes_root_with_two_children_and_deep_successor)
{
    RB_Tree *tree = rb_tree_new();
    cr_assert_not_null(tree);

    rb_insert(tree, 20);
    rb_insert(tree, 10);
    rb_insert(tree, 30);
    rb_insert(tree, 25);
    rb_insert(tree, 40);
    rb_insert(tree, 22);

    RB_Node *root = rb_find(tree, 20);
    cr_assert_not_null(root);
    rb_delete(tree, root);

    cr_assert_null(rb_find(tree, 20));
    cr_assert_not_null(rb_find(tree, 10));
    cr_assert_not_null(rb_find(tree, 22));
    cr_assert_not_null(rb_find(tree, 25));
    cr_assert_not_null(rb_find(tree, 30));
    cr_assert_not_null(rb_find(tree, 40));
    cr_assert_eq(validate_tree_strict(tree), 1);
    cr_assert_eq(tree->root->parent, NULL);

    rb_tree_destroy(tree);
}

Test(rb_tree_additional_delete, deletes_all_nodes_in_shuffled_order)
{
    RB_Tree *tree = rb_tree_new();
    cr_assert_not_null(tree);

    const int count = 256;
    int values[count];
    fill_range(values, count, 0);

    for (int i = 0; i < count; i++)
    {
        rb_insert(tree, values[i]);
    }

    shuffle_int_array(values, count, 0xA11CEU);

    int missing_at_delete = -1;
    int invalid_after_delete = -1;
    for (int i = 0; i < count; i++)
    {
        RB_Node *node = rb_find(tree, values[i]);
        if (node == NULL)
        {
            missing_at_delete = i;
            break;
        }

        rb_delete(tree, node);
        if (!validate_tree_strict(tree))
        {
            invalid_after_delete = i;
            break;
        }
    }
    cr_assert_eq(missing_at_delete, -1, "Missing node at delete index %d",
                 missing_at_delete);
    cr_assert_eq(invalid_after_delete, -1,
                 "Tree invalid after delete index %d", invalid_after_delete);

    cr_assert_eq(tree->root, &tree->nil);
    cr_assert_eq(validate_tree_strict(tree), 1);

    rb_tree_destroy(tree);
}

TestSuite(rb_tree_additional_exhaustive, .timeout = 40);

Test(rb_tree_additional_exhaustive,
     all_insert_delete_permutations_of_five_keys_stay_valid)
{
    int sorted_values[5] = { 1, 2, 3, 4, 5 };
    int insert_order[5];
    int delete_order[5];
    int scenario_count = 0;
    int failed_scenario = -1;
    int failed_insert_step = -1;
    int failed_delete_step = -1;
    int failure_phase = 0;

    memcpy(insert_order, sorted_values, sizeof(sorted_values));
    do
    {
        memcpy(delete_order, sorted_values, sizeof(sorted_values));
        do
        {
            RB_Tree *tree = rb_tree_new();
            if (!tree)
            {
                failed_scenario = scenario_count;
                failure_phase = 1;
                break;
            }

            for (int i = 0; i < 5; i++)
            {
                rb_insert(tree, insert_order[i]);
                if (!validate_tree_strict(tree))
                {
                    failed_scenario = scenario_count;
                    failed_insert_step = i;
                    failure_phase = 2;
                    break;
                }
            }
            if (failure_phase != 0)
            {
                rb_tree_destroy(tree);
                break;
            }

            for (int i = 0; i < 5; i++)
            {
                RB_Node *node = rb_find(tree, delete_order[i]);
                if (!node)
                {
                    failed_scenario = scenario_count;
                    failed_delete_step = i;
                    failure_phase = 3;
                    break;
                }
                rb_delete(tree, node);
                if (!validate_tree_strict(tree))
                {
                    failed_scenario = scenario_count;
                    failed_delete_step = i;
                    failure_phase = 4;
                    break;
                }
            }
            if (failure_phase != 0)
            {
                rb_tree_destroy(tree);
                break;
            }

            if (tree->root != &tree->nil)
            {
                failed_scenario = scenario_count;
                failure_phase = 5;
            }
            rb_tree_destroy(tree);
            scenario_count++;
            if (failure_phase != 0)
            {
                break;
            }

        } while (next_permutation_int(delete_order, 5));
        if (failure_phase != 0)
        {
            break;
        }

    } while (next_permutation_int(insert_order, 5));

    cr_assert_eq(failure_phase, 0,
                 "Exhaustive test failure: phase=%d scenario=%d insert_step=%d "
                 "delete_step=%d",
                 failure_phase, failed_scenario, failed_insert_step,
                 failed_delete_step);
    cr_assert_eq(scenario_count, 120 * 120);
}
