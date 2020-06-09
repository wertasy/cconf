#include "cconf.h"
#include "rbtree.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CCONF_LINE_SIZE 1024

cconf_t *cconf_create()
{
    cconf_t *cconf;

    cconf = (cconf_t *)malloc(sizeof(cconf_t));
    if (cconf == NULL)
    {
        return NULL;
    }

    cconf->rbtree = rbt_create();
    if (cconf->rbtree == NULL)
    {
        free(cconf);
        return NULL;
    }

    return cconf;
}

void cconf_delete(cconf_t *cconf)
{
    rbt_destory(cconf->rbtree);
    free(cconf);
}

const char *cconf_get(cconf_t *cconf, const char *key)
{
    return rbt_search(cconf->rbtree, key);
}

void cconf_set(cconf_t *cconf, const char *key, const char *value)
{
    rbt_insert_or_assign(cconf->rbtree, key, value);
}

int cconf_read(cconf_t *cconf, const char *path)
{
    FILE *conf;
    char line[CCONF_LINE_SIZE];
    char key[CCONF_LINE_SIZE];
    char value[CCONF_LINE_SIZE];
    int i;
    int line_syntax_err;
    int syntax_err_count = 0;

    if (cconf == NULL || path == NULL)
    {
        return -1;
    }

    conf = fopen(path, "r");
    if (conf == NULL)
    {
        return -1;
    }

    fgets(line, CCONF_LINE_SIZE, conf);
    for (i = 1; feof(conf) == 0; i++)
    {
        line_syntax_err = 0;
        if (*line != '\n' && *line != '#')
        {
            if (*line == '\"')
            {
                if (sscanf(line, "\"%[^\"]\"%*[=\t ]\"%[^\"]\"", key, value) != 2)
                {
                    if (sscanf(line, "\"%[^\"]\"%*[=\t ]%[^#\t\n ]", key, value) != 2)
                    {
                        if (sscanf(line, "\"%[^\"]\"", key) == 1)
                        {
                            strcpy(value, "1");
                        }
                        else
                        {
                            line_syntax_err = 1;
                        }
                    }
                }
            }
            else if (*line == '\'')
            {
                if (sscanf(line, "\'%[^\']\'%*[=\t ]\'%[^\']\'", key, value) != 2)
                {
                    if (sscanf(line, "\'%[^\']\'%*[=\t ]%[^#\t\n ]", key, value) != 2)
                    {
                        if (sscanf(line, "\'%[^\']\'", key) == 1)
                        {
                            strcpy(value, "1");
                        }
                        else
                        {
                            line_syntax_err = 1;
                        }
                    }
                }
            }
            else if (sscanf(line, "%[^=\t ]%*[=\t ]\"%[^\"]\"", key, value) != 2)
            {
                if (sscanf(line, "%[^=\t ]%*[=\t ]%[^#\t\n ]", key, value) != 2)
                {
                    if (sscanf(line, "%[^=#\t\n ]", key) == 1)
                    {
                        strcpy(value, "1");
                    }
                    else
                    {
                        line_syntax_err = 1;
                    }
                }
            }

            if (line_syntax_err)
            {
                syntax_err_count++;
                fprintf(stderr, "syntex error at line %d: %s", i, line);
            }

            if (syntax_err_count == 0)
            {
                cconf_set(cconf, key, value);
            }
        }
        fgets(line, CCONF_LINE_SIZE, conf);
    }

    fclose(conf);
    return syntax_err_count;
}