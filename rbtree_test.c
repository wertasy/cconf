#include <stdio.h>
#include <stdlib.h>
#include "rbtree.h"

int main()
{
    rbt_root_t *root = rbt_create();

    rbt_insert(root, "file", "tmpfile");
    rbt_insert(root, "cha li ju", "LiJu Cha");
    rbt_insert(root, "you yin mei", "YinMei You");
    rbt_insert(root, "wang wu", "Wu Wang");
    rbt_insert(root, "you hong ze", "HongZe You");
    rbt_insert(root, "cha li li", "LiLi Cha");

    rbt_print(root);

    const char *s;
    if ((s = rbt_search(root, "tmpfile")))
        printf("value: %s\n", s);

    rbt_destory(root);
    exit(0);
}
