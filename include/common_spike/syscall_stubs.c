// Minimal newlib syscall stubs for bare-metal environment.
// These satisfy undefined references when linking with -lc / newlib pieces.
// They either return error (-1) or implement a trivial heap via _end symbol.

#include <sys/stat.h>
#include <stdint.h>

// Provided by linker script
extern char _end;            // end of bss
static char *heap_end;       // current heap pointer

int _close(int fd) {
    (void)fd; return -1;     // no files
}

int _fstat(int fd, struct stat *st) {
    (void)fd; if (st) { st->st_mode = S_IFCHR; } return 0; // pretend char device
}

int _isatty(int fd) {
    (void)fd; return 1;      // yes, treat as tty
}

off_t _lseek(int fd, off_t offset, int whence) {
    (void)fd; (void)offset; (void)whence; return 0; // not supported
}

int _open(const char *path, int flags, int mode) {
    (void)path; (void)flags; (void)mode; return -1; // fail
}

int _read(int fd, void *buf, size_t cnt) {
    (void)fd; (void)buf; (void)cnt; return 0; // EOF
}

// Simple bump allocator. NOT thread-safe. No heap limit enforcement besides arbitrary max.
void* _sbrk(ptrdiff_t incr) {
    if (heap_end == 0) heap_end = &_end;
    char *prev = heap_end;
    heap_end += incr;
    return prev; // no out-of-memory check
}

int _write(int fd, const void *buf, size_t cnt) {
    // Reuse existing proxy write (putchar based) if available.
    // We declare printstr here weakly to avoid circular deps.
    extern void printstr(const char*) __attribute__((weak));
    if (printstr) {
        // crude: assume buf is text without NUL, allocate temp with NUL termination
        const char *c = (const char*)buf;
        for (size_t i=0;i<cnt;i++) {
            char tmp[2] = { c[i], 0 };
            printstr(tmp);
        }
    }
    return (int)cnt;
}

// Optional errno (newlib may look for it)
int *__errno(void) {
    static int e; return &e;
}
