#include "rbtree.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static rbt_node_t *rbt_node_create(const char *key, const char *value, rbt_node_t *parent, rbt_node_t *lchld, rbt_node_t *rchld);
static rbt_node_t *rbt_node_minimum(rbt_node_t *node);
static rbt_node_t *rbt_node_maximum(rbt_node_t *node);
// static rbt_node_t *rbt_node_successor(rbt_node_t *node);
// static rbt_node_t *rbt_node_predecessor(rbt_node_t *node);
static void rbt_left_rotate(rbt_root_t *root, rbt_node_t *node);
static void rbt_right_rotate(rbt_root_t *root, rbt_node_t *node);
static void rbt_insert_fixup(rbt_root_t *root, rbt_node_t *node);
static void rbt_delete_fixup(rbt_root_t *root, rbt_node_t *node, rbt_node_t *parent);
static void rbt_node_insert(rbt_root_t *root, rbt_node_t *node);
static void rbt_node_delete(rbt_root_t *root, rbt_node_t *node);
static void rbt_node_destory(rbt_node_t *node);
static void rbt_node_print(rbt_node_t *node, const char *key, int dir);

#define RBT_RED 'R'
#define RBT_BLACK 'B'

#define rbt_node_is_red(x) \
    ((x) != NULL && (x)->color == RBT_RED)

#define rbt_node_is_black(x) \
    ((x) == NULL || (x)->color == RBT_BLACK)

#define rbt_node_set_red(x) \
    (x)->color = RBT_RED

#define rbt_node_set_black(x) \
    (x)->color = RBT_BLACK

rbt_root_t *rbt_create()
{
    rbt_root_t *root = (rbt_root_t *)malloc(sizeof(rbt_root_t));
    root->node = NULL;
    return root;
}

rbt_node_t *rbt_node_search(rbt_node_t *node, const char *key)
{
    if (node == NULL || strcmp(node->key, key) == 0)
    {
        return node;
    }
    if (strcmp(key, node->key) < 0)
    {
        return rbt_node_search(node->lchld, key);
    }
    else
    {
        return rbt_node_search(node->rchld, key);
    }
}

const char *rbt_search(rbt_root_t *root, const char *key)
{
    rbt_node_t *node;
    node = rbt_node_search(root->node, key);

    if (node != NULL)
    {
        return node->value;
    }

    return NULL;
}

int rbt_exist(rbt_root_t *root, const char *key)
{
    if (root == NULL)
    {
        return 0;
    }
    return rbt_node_search(root->node, key) ? 1 : 0;
}

static rbt_node_t *rbt_node_minimum(rbt_node_t *node)
{
    if (node == NULL)
    {
        return NULL;
    }
    while (node->lchld != NULL)
    {
        node = node->lchld;
    }
    return node;
}

static rbt_node_t *rbt_node_maximum(rbt_node_t *node)
{
    if (node == NULL)
    {
        return NULL;
    }
    while (node->rchld != NULL)
    {
        node = node->rchld;
    }
    return node;
}

int rbt_minimum(rbt_root_t *root, char *out)
{
    rbt_node_t *node;

    if (root != NULL)
    {
        node = rbt_node_minimum(root->node);
    }

    if (node == NULL)
    {
        return -1;
    }

    out = node->key;
    return 0;
}

int rbt_maximum(rbt_root_t *root, char *out)
{
    rbt_node_t *node;

    if (root != NULL)
    {
        node = rbt_node_maximum(root->node);
    }

    if (node == NULL)
    {
        return -1;
    }

    out = node->key;
    return 0;
}

// static rbt_node_t *rbt_node_successor(rbt_node_t *node)
// {
//     if (node->rchld != NULL)
//     {
//         return rbt_node_minimum(node->rchld);
//     }

//     rbt_node_t *parent = node->parent;
//     while (parent != NULL && node == parent->rchld)
//     {
//         node = parent;
//         parent = parent->parent;
//     }

//     return parent;
// }

// static rbt_node_t *rbt_node_predecessor(rbt_node_t *node)
// {
//     if (node->lchld != NULL)
//     {
//         return rbt_node_maximum(node->rchld);
//     }

//     rbt_node_t *parent = node->parent;
//     while (parent != NULL && node == parent->lchld)
//     {
//         node = parent;
//         parent = parent->parent;
//     }

//     return parent;
// }

static void rbt_left_rotate(rbt_root_t *root, rbt_node_t *node)
{
    rbt_node_t *tmp = node->rchld;

    node->rchld = tmp->lchld;
    if (tmp->lchld != NULL)
    {
        tmp->lchld->parent = node;
    }

    tmp->parent = node->parent;

    if (node->parent == NULL)
    {
        root->node = tmp;
    }
    else
    {
        if (node->parent->lchld == node)
        {
            node->parent->lchld = tmp;
        }
        else
        {
            node->parent->rchld = tmp;
        }
    }

    tmp->lchld = node;
    node->parent = tmp;
}

static void rbt_right_rotate(rbt_root_t *root, rbt_node_t *node)
{
    rbt_node_t *tmp = node->lchld;

    node->lchld = tmp->rchld;
    if (tmp->rchld != NULL)
    {
        tmp->rchld->parent = node;
    }

    tmp->parent = node->parent;

    if (node->parent == NULL)
    {
        root->node = tmp;
    }
    else
    {
        if (node->parent->rchld == node)
        {
            node->parent->rchld = tmp;
        }
        else
        {
            node->parent->lchld = tmp;
        }
    }

    tmp->rchld = node;
    node->parent = tmp;
}

static void rbt_insert_fixup(rbt_root_t *root, rbt_node_t *node)
{
    rbt_node_t *parent, *gparent;

    while (rbt_node_is_red(node->parent))
    {
        parent = node->parent;
        gparent = parent->parent;

        if (parent == gparent->lchld)
        {
            rbt_node_t *uncle = gparent->rchld;

            if (rbt_node_is_red(uncle))
            {
                rbt_node_set_black(uncle);
                rbt_node_set_red(gparent);
                node = gparent;
            }
            else
            {
                if (node == parent->rchld)
                {
                    rbt_node_t *tmp;
                    rbt_left_rotate(root, parent);
                    tmp = parent;
                    parent = node;
                    node = tmp;
                }
                else
                {
                    rbt_node_set_black(parent);
                    rbt_node_set_red(gparent);
                    rbt_right_rotate(root, gparent);
                }
            }
        }
        else
        {
            rbt_node_t *uncle = gparent->lchld;

            if (rbt_node_is_red(uncle))
            {
                rbt_node_set_black(uncle);
                rbt_node_set_red(gparent);
                node = gparent;
            }
            else
            {
                if (node == parent->lchld)
                {
                    rbt_node_t *tmp;
                    rbt_right_rotate(root, parent);
                    tmp = parent;
                    parent = node;
                    node = tmp;
                }
                else
                {
                    rbt_node_set_black(parent);
                    rbt_node_set_red(gparent);
                    rbt_left_rotate(root, gparent);
                }
            }
        }
    }
    rbt_node_set_black(root->node);
}

static void rbt_node_insert(rbt_root_t *root, rbt_node_t *node)
{
    rbt_node_t *y = NULL;
    rbt_node_t *x = root->node;

    while (x != NULL)
    {
        y = x;
        if (strcmp(node->key, x->key) < 0)
        {
            x = x->lchld;
        }
        else
        {
            x = x->rchld;
        }
    }

    node->parent = y;

    if (y != NULL)
    {
        if (strcmp(node->key, y->key) < 0)
        {
            y->lchld = node;
        }
        else
        {
            y->rchld = node;
        }
    }
    else
    {
        root->node = node;
    }

    rbt_node_set_red(node);
    rbt_insert_fixup(root, node);
}

static rbt_node_t *rbt_node_create(const char *key, const char *value, rbt_node_t *parent, rbt_node_t *lchld, rbt_node_t *rchld)
{
    rbt_node_t *p;
    size_t len_key, len_value;

    p = (rbt_node_t *)malloc(sizeof(rbt_node_t));
    if (p == NULL)
    {
        return NULL;
    }

    len_key = strlen(key);
    len_value = strlen(value);

    p->key = (char *)malloc(len_key + 1);
    if (p->key == NULL)
    {
        free(p);
        return NULL;
    }

    p->value = (char *)malloc(len_value + 1);
    if (p->value == NULL)
    {
        free(p);
        free(p->key);
        return NULL;
    }

    memset(p->key, 0, len_key + 1);
    memset(p->value, 0, len_value + 1);

    memcpy(p->key, key, len_key);
    memcpy(p->value, value, len_value);

    p->parent = parent;
    p->lchld = lchld;
    p->rchld = rchld;

    rbt_node_set_black(p);
    return p;
}

int rbt_insert(rbt_root_t *root, const char *key, const char *value)
{
    rbt_node_t *node;

    if (rbt_exist(root, key))
    {
        return -1;
    }

    node = rbt_node_create(key, value, NULL, NULL, NULL);
    if (node == NULL)
    {
        return -1;
    }

    rbt_node_insert(root, node);
    return 0;
}

int rbt_insert_or_assign(rbt_root_t *root, const char *key, const char *value)
{
    rbt_node_t *node;

    node = rbt_node_search(root->node, key);
    if (node != NULL)
    {
        size_t newlen = strlen(value);
        if (strlen(node->value) < newlen)
        {
            node->value = realloc(node->value, newlen + 1);
        }
        memset(node->value, 0, newlen + 1);
        memcpy(node->value, value, newlen);
        return 0;
    }

    node = rbt_node_create(key, value, NULL, NULL, NULL);
    if (node == NULL)
    {
        return -1;
    }

    rbt_node_insert(root, node);
    return 0;
}

static void rbt_delete_fixup(rbt_root_t *root, rbt_node_t *node, rbt_node_t *parent)
{
    rbt_node_t *bro;

    while (rbt_node_is_black(node) && node != root->node)
    {
        if (node == parent->lchld)
        {
            bro = parent->rchld;

            if (rbt_node_is_red(bro))
            {
                rbt_node_set_black(bro);
                rbt_node_set_red(parent);
                rbt_left_rotate(root, parent);
                bro = parent->rchld;
            }

            if (rbt_node_is_black(bro->lchld) && rbt_node_is_black(bro->rchld))
            {
                rbt_node_set_red(bro);
                node = parent;
                parent = node->parent;
            }
            else
            {
                if (rbt_node_is_black(bro->rchld))
                {
                    rbt_node_set_black(bro->lchld);
                    rbt_node_set_red(bro);
                    rbt_right_rotate(root, bro);
                    bro = parent->rchld;
                }

                bro->color = parent->color;
                rbt_node_set_black(parent);
                rbt_node_set_black(bro->rchld);
                rbt_left_rotate(root, parent);
                node = root->node;
                break;
            }
        }
        else
        {
            bro = parent->lchld;

            if (rbt_node_is_red(bro))
            {
                rbt_node_set_black(bro);
                rbt_node_set_red(parent);
                rbt_right_rotate(root, parent);
                bro = parent->lchld;
            }

            if (rbt_node_is_black(bro->lchld) && rbt_node_is_black(bro->rchld))
            {
                rbt_node_set_red(bro);
                node = parent;
                parent = node->parent;
            }
            else
            {
                if (rbt_node_is_black(bro->lchld))
                {
                    rbt_node_set_black(bro->rchld);
                    rbt_node_set_red(bro);
                    rbt_left_rotate(root, bro);
                    bro = parent->lchld;
                }

                bro->color = parent->color;
                rbt_node_set_black(parent);
                rbt_node_set_black(bro->lchld);
                rbt_right_rotate(root, parent);
                node = root->node;
                break;
            }
        }
    }

    if (node != NULL)
    {
        rbt_node_set_black(node);
    }
}

static void rbt_node_delete(rbt_root_t *root, rbt_node_t *node)
{
    int color;
    rbt_node_t *chld, *parent;

    if (node->lchld != NULL && node->rchld != NULL)
    {
        rbt_node_t *replace = node->rchld;

        while (replace->lchld != NULL)
        {
            replace = replace->lchld;
        }

        if (node->parent != NULL)
        {
            if (node == node->parent->lchld)
            {
                node->parent->lchld = replace;
            }
            else
            {
                node->parent->rchld = replace;
            }
        }
        else
        {
            root->node = replace;
        }

        chld = replace->rchld;
        parent = replace->parent;
        color = replace->color;

        if (parent == node)
        {
            parent = replace;
        }
        else
        {
            if (chld != NULL)
            {
                chld->parent = parent;
            }
            parent->lchld = chld;
            replace->rchld = node->rchld;
            node->rchld->parent = replace;
        }

        replace->parent = node->parent;
        replace->color = node->color;
        replace->lchld = node->lchld;
        node->lchld->parent = replace;
    }
    else
    {
        if (node->lchld != NULL)
        {
            chld = node->lchld;
        }
        else
        {
            chld = node->rchld;
        }

        parent = node->parent;
        color = node->color;

        if (chld != NULL)
        {
            chld->parent = parent;
        }

        if (parent != NULL)
        {
            if (parent->lchld == node)
            {
                parent->lchld = chld;
            }
            else
            {
                parent->rchld = chld;
            }
        }
        else
        {
            root->node = chld;
        }
    }

    if (color == RBT_BLACK)
    {
        rbt_delete_fixup(root, chld, parent);
    }

    free(node->key);
    free(node->value);
    free(node);
}

void rbt_delete(rbt_root_t *root, const char *key)
{
    rbt_node_t *target;

    target = rbt_node_search(root->node, key);
    if (target != NULL)
    {
        rbt_node_delete(root, target);
    }
}

static void rbt_node_destory(rbt_node_t *node)
{
    if (node == NULL)
    {
        return;
    }

    if (node->lchld != NULL)
    {
        rbt_node_destory(node->lchld);
    }

    if (node->rchld != NULL)
    {
        rbt_node_destory(node->rchld);
    }

    free(node->key);
    free(node->value);
    free(node);
}

void rbt_destory(rbt_root_t *root)
{
    if (root != NULL)
    {
        rbt_node_destory(root->node);
    }

    free(root);
}

static void rbt_node_print(rbt_node_t *node, const char *key, int dir)
{
    if (node != NULL)
    {
        if (dir == 0)
        {
            printf("(B) [%s] is Root\n", node->key);
        }
        else
        {
            printf("(%c) [%s] is [%s]'s %s Child\n", node->color, node->key, key, dir == 1 ? "Right" : "Left");
        }
        rbt_node_print(node->lchld, node->key, -1);
        rbt_node_print(node->rchld, node->key, 1);
    }
}

void rbt_print(rbt_root_t *root)
{
    if (root != NULL && root->node != NULL)
    {
        rbt_node_print(root->node, root->node->key, 0);
    }
}
