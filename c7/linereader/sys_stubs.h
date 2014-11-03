#ifndef SYS_STUBS_H
#define SYS_STUBS_H

extern "C" {
#include <string.h>
#include <fcntl.h>

inline size_t sys_read (int fd, void* buffer, size_t n) {
    return read(fd, buffer, n);
}

/** Note: This is not a 'real' mkstemp() implementation.
 * Therfore, test must explicitly call unlink(name) when done.
 */
inline int mkstemp(const char* name) {
    close(open(name, O_CREAT, S_IRWXU|S_IRWXG|S_IRWXO));
    return open(name, O_RDWR);
}

} // extern "C"

#endif // SYS_STUBS_H
