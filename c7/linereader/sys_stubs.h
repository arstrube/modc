#ifndef SYS_LINUX_SYSCALL_SUPPORT_H
#define SYS_LINUX_SYSCALL_SUPPORT_H

extern "C" {
#include <string.h>
#include <fcntl.h>

inline size_t sys_read (int fd, void* buffer, size_t n) {
    return read(fd, buffer, n);
}

inline int mkstemp(const char* name) {
    close(open(name, O_CREAT, S_IRUSR||S_IWUSR));
    return open(name, O_RDWR);
}

} // extern "C"

#endif
