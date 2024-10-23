#define _GNU_SOURCE
#include <dlfcn.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>

int
unlink(const char *pathname)
{
    static int (*real_unlink)(const char *pathname) = NULL;
    if (!real_unlink) {
        real_unlink = dlsym(RTLD_NEXT, "unlink");
    }

    if (strstr(pathname, "PROTECT") != NULL) {
        fprintf(stderr, "Debug: Preventing deletion of PROTECTed file '%s'\n", pathname);
        errno = EPERM;
        return -1;
    }

    return real_unlink(pathname);
}
