#ifndef __CCONF_H_
#define __CCONF_H_

typedef struct cconf_s cconf_t;
struct rbt_root_s;

struct cconf_s {
    struct rbt_root_s *rbtree;
};

cconf_t *cconf_create();

void cconf_delete(cconf_t *cconf);

int cconf_read(cconf_t *cconf, const char *path);

const char *cconf_get(cconf_t *cconf, const char *key);

void cconf_set(cconf_t *cconf, const char *key, const char *value);

#endif /* __CCONF_H_ */
