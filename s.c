#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "syscalltab_func.h"

int main(argc, args)
    int argc;
    char** args;
{
    char* sc_name = NULL;
    size_t sc_size = -1;

    if (argc != 2) {
        return(1);
        }

    sc_size = (sizeof(char) * (strlen(args[1]) + 1));
    sc_name = (char*) malloc(sc_size);

    if (sc_name == NULL) {
        perror("malloc");

        return 1;
        }

    snprintf(sc_name, sc_size, "%s", args[1]);

    fprintf(stdout, "syscall: %s  number: %d\n",
            sc_name, syscall_number(sc_name));

    fprintf(stdout, "total syscalls: %d\n", syscall_count());

    fprintf(stdout, "syscall_kverify: %d\n", _syscall_kverify());
    fprintf(stdout, "syscall_krelease: %s\n", syscall_krelease());
    fprintf(stdout, "syscall_karch: %s\n", syscall_karch());

    free(sc_name);
    return(0);
}
