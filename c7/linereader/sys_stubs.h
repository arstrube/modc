#ifndef SYS_LINUX_SYSCALL_SUPPORT_H
#define SYS_LINUX_SYSCALL_SUPPORT_H

extern "C" {
#include <string.h>

inline size_t sys_read (int fd, void* buffer, size_t n) {
    (void)fd;
    (void)buffer;
    (void)n;
    return 0;
}

inline int mkstemp(const char* name) {
    (void)name;
    return 0;
}

} // extern "C"

#endif
