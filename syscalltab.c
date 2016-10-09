#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/utsname.h>

#include "syscalltab.h"
#include "syscalltab_func.h"

char* syscall_krelease()
{
    return SC_KERNEL_RELEASE;
}

char* syscall_karch()
{
    return SC_KERNEL_ARCH;
}

int syscall_count()
{
    return SC_ARRAY_LENGTH;
}

char* syscall_name(syscall_number)
    int syscall_number;
{
    int iterator = 0;
    int map_syscall = 0;

    if (syscall_number < 0 || syscall_number >= SC_ARRAY_LENGTH) {
        return NULL;
        }

    for (; iterator < SC_ARRAY_LENGTH; iterator++) {
        map_syscall = SYSCALL_MAP[iterator].sc_value;

        if (map_syscall == syscall_number) {
            return strdup(SYSCALL_MAP[iterator].sc_name);
            }
        }

    return NULL;
    }

int syscall_number(syscall_name)
    char* syscall_name;
{
    char* map_syscall = NULL;
    int syscall_number = -1;
    int iterator = 0;

    if (syscall_name == NULL) {
        return syscall_number;
        }

    for (; iterator < SC_ARRAY_LENGTH; iterator++) {
        map_syscall = SYSCALL_MAP[iterator].sc_name;

        if (strcmp(map_syscall, syscall_name) == 0) {
            syscall_number = SYSCALL_MAP[iterator].sc_value;
            break;
            }
        }

    return syscall_number;
}

int _syscall_kverify()
{
    char* krelease = SC_KERNEL_RELEASE;
    char* karch = SC_KERNEL_ARCH;
    struct utsname unamebuf;
    int retval;

    retval = uname(&unamebuf);

    if (retval == 0) {
        if (strcmp(krelease, unamebuf.release) == 0) {
            if (strcmp(karch, unamebuf.machine) == 0) {
                return 0;
                }
            }
        }

    return -1;
}
