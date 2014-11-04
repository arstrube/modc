#ifndef SYS_STUBS_H
#define SYS_STUBS_H

extern "C" {
#include <string.h>
#include <fcntl.h>

inline size_t sys_read (int fd, void* buffer, size_t n) { return read(fd, buffer, n); }

} // extern "C"

#endif // SYS_STUBS_H
