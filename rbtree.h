#ifndef __CCONF_RBTREE_
#define __CCONF_RBTREE_

typedef struct rbt_node_s rbt_node_t;
typedef struct rbt_root_s rbt_root_t;

struct rbt_node_s {
    char color;
    char *key;
    char *value;
    rbt_node_t *parent;
    rbt_node_t *lchld;
    rbt_node_t *rchld;
};

struct rbt_root_s {
    rbt_node_t *node;
};

rbt_root_t *rbt_create();

int rbt_exist(rbt_root_t *root, const char *key);

int rbt_minimum(rbt_root_t *root, char *out);

int rbt_maximum(rbt_root_t *root, char *out);

int rbt_insert(rbt_root_t *root, const char *key, const char *value);

int rbt_insert_or_assign(rbt_root_t *root, const char *key, const char *value);

void rbt_delete(rbt_root_t *root, const char *key);

rbt_node_t *rbt_node_search(rbt_node_t *node, const char *key);

const char *rbt_search(rbt_root_t *root, const char *key);

void rbt_destory(rbt_root_t *root);

void rbt_print(rbt_root_t *root);

#endif /* __CCONF_RBTREE_ */
