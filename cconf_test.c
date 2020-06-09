#include "cconf.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
    cconf_t *conf = cconf_create();
    if (conf == NULL)
    {
        exit(1);
    }

    int err = cconf_read(conf, argv[1]);
    if (err != 0)
    {
        perror("cconf_read()");
    }

    for (int i = 2; i < argc; i++)
    {
        printf("%s: %s\n", argv[i], cconf_get(conf, argv[i]));
    }

    cconf_delete(conf);
    exit(err);
}
